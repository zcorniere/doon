#include "RenderManager.hpp"
#include "Logger.hpp"
#include "MapManager.hpp"
#include "ObjectManager.hpp"
#include "Ray.hpp"
#include "StorageManager.hpp"
#include "ThreadManager.hpp"
#include <cmath>
#include <unordered_set>

constexpr const float fFOV = M_PI / 4.0f;
constexpr const char sSkyTexture[] = "sky";
constexpr const char sFloorTexture[] = "greystone";

RenderManager::RenderManager(): bDrawDebug(std::getenv("DOON_BUG_BOX"))
{
    if (bDrawDebug) { logger->warn("RenderManager") << "Starting in debug mode"; }
}

RenderManager::~RenderManager() {}

const Frame &RenderManager::update(const unsigned uPovIndex)
{
    const auto &pPov = object_manager->at(uPovIndex);
    const auto &map = map_manager->get();
    Vector<float> fEye(std::sin(pPov->getAngle()), std::cos(pPov->getAngle()));
    float fEyeAngle = fEye.atan();
    std::deque<std::future<void>> fur(size.x);

    Ray ray(pPov->getPosition());
    for (unsigned x = 0; x < size.x; ++x) {
        fur.at(x) = thread_manager->push(
            [this, &map, x](int, const float fAngle, Ray rayDef) {
                float fSample = 0;
                float fRayAngle = (fAngle - (fFOV / 2.0f)) + (float(x) / size.x) * fFOV;
                rayDef.setRayDirection(fRayAngle);
                rayDef.fFish = std::cos(fRayAngle - fAngle);
                rayDef.shoot(map);
                fSample = sampleColumn(rayDef);

                rayDef.fDistance *= rayDef.fFish;
                qDepthBuffer.fill(x, rayDef.fDistance);
                this->drawColumn(map, x, rayDef, fSample);
            },
            pPov->getAngle(), ray);
    }
    for (const auto &i: object_manager->getObjects()) {
        if (pPov == i || !i->getTextureName()) continue;
        fur.push_back(thread_manager->push(
            [this, &i](int, const Vector<float> &fCamPosition, const float &fEyeAngle) {
                this->drawObject(i, fCamPosition, fEyeAngle);
            },
            pPov->getPosition(), fEyeAngle));
    }
    std::for_each(fur.begin(), fur.end(), [](auto &i) { i.get(); });
    return img;
}

void RenderManager::resize(Vector<unsigned> fNewVector)
{
    size = std::move(fNewVector);
    qDepthBuffer.resize(size);
    img.create(size);
}

float RenderManager::sampleColumn(const Ray &ray) noexcept
{
    Vector<float> fIntersection(ray.fOrigin + ray.fDirection * ray.fDistance);
    Vector<float> fBlockMid(ray.uMap);
    fBlockMid += 0.5f;
    float fTestAngle =
        std::atan2((fIntersection.y - fBlockMid.y), (fIntersection.x - fBlockMid.x));
    if (fTestAngle >= -M_PI * 0.25f && fTestAngle < M_PI * 0.25f)
        return fIntersection.y - ray.uMap.y;
    else if (fTestAngle >= M_PI * 0.25f && fTestAngle < M_PI * 0.75f)
        return fIntersection.x - ray.uMap.x;
    else if (fTestAngle < -M_PI * 0.25f && fTestAngle >= -M_PI * 0.75f)
        return fIntersection.x - ray.uMap.x;
    else if (fTestAngle >= M_PI * 0.75f || fTestAngle < -M_PI * 0.75f)
        return fIntersection.y - ray.uMap.y;
    return 1.0f;
}

void RenderManager::drawColumn(const Map &map, const unsigned x, const Ray &ray,
                               const float sample)
{
    const Frame &iSky(storage_manager->get<Frame>(sSkyTexture));
    const Vector<unsigned> uSkySize(iSky.getSize());
    const Frame &iWall(storage_manager->get<Frame>(map.getTextureName(ray.cHit)));
    const Vector<unsigned> uWallSize(iWall.getSize());
    const Frame &iFloor(storage_manager->get<Frame>(sFloorTexture));
    const Vector<unsigned> uFloorSize(iFloor.getSize());

    float fCeiling = (size.y >> 1) - (size.y / ray.fDistance);
    float fFloor = size.y - fCeiling;

    float fShade = 1.0f - std::min(ray.fDistance / fDepth, 1.0f);
    Vector<float> fSample(sample);
    Pixel sampled;
    for (unsigned y = 0; y < size.y; ++y) {
        if (y <= fCeiling) {
            float fPlaneZ = (size.y >> 1) / ((size.y >> 1) - float(y));
            Vector<float> fPlanePoint =
                ray.fOrigin + ray.fDirection * fPlaneZ * 2.0f / ray.fFish;
            sampled = iSky.getPixel(this->sampleVector(fPlanePoint.modf(), uSkySize));
        } else if (y > fCeiling && y <= fFloor) {
            fSample.y = (y - fCeiling) / (fFloor - fCeiling);
            sampled = iWall.getPixel(this->sampleVector(fSample, uWallSize));
            sampled.shade(fShade);
        } else {
            float fPlaneZ = (size.y >> 1) / (float(y) - (size.y >> 1));
            Vector<float> fPlanePoint =
                ray.fOrigin + ray.fDirection * fPlaneZ * 2.0f / ray.fFish;
            sampled = iFloor.getPixel(this->sampleVector(fPlanePoint.modf(), uFloorSize));
        }
        img.setPixel(x, y, sampled);
    }
}

void RenderManager::drawObject(const std::unique_ptr<AObject> &obj,
                               const Vector<float> &fCamPosition, const float &fEyeAngle)
{
    Vector<float> fVec(obj->getPosition() - fCamPosition);
    float fDistanceToPlayer(fVec.mag());
    float fObjectAngle(fEyeAngle - fVec.atan());
    if (fObjectAngle < -3.14159f) fObjectAngle += 2.0f * 3.14159f;
    if (fObjectAngle > 3.14159f) fObjectAngle -= 2.0f * 3.14159f;

    const float fShade = 1.0f - std::min(fDistanceToPlayer / fDepth, 1.0f);
    const Frame &iSprite(storage_manager->get<Frame>(obj->getTextureName().value()));
    const Vector<unsigned> uImgSize(iSprite.getSize());

    bool bInCamFOV = std::abs(fObjectAngle) < (fFOV + (1.0f / fDistanceToPlayer)) / 2.0f;
    if (!bInCamFOV || fDistanceToPlayer < 0.5f) return;

    Vector<float> fFloorPoint;
    fFloorPoint.x = (0.5f * ((fObjectAngle / (fFOV * 0.5f))) + 0.5f) * size.x;
    fFloorPoint.y =
        (size.y >> 1) + (size.y / fDistanceToPlayer) / std::cos(fObjectAngle / 2.0f);

    Vector<float> fObjectSize((bDrawDebug) ? (obj->getRadius()) : (obj->getSize()));
    fObjectSize *= size.y << 1;
    fObjectSize /= fDistanceToPlayer;

    Vector<float> fObjectTopLeft;
    fObjectTopLeft.x = fFloorPoint.x - fObjectSize.x / 2.0f;
    fObjectTopLeft.y = fFloorPoint.y - fObjectSize.y;

    Pixel sample;
    for (float y = 0; y < fObjectSize.y; ++y) {
        for (float x = 0; x < fObjectSize.x; ++x) {
            if (bDrawDebug) {
                sample = Color::Red;
            } else {
                Vector<float> fSample(x / fObjectSize.x, y / fObjectSize.y);
                sample = iSprite.getPixel(this->sampleVector(fSample, uImgSize));
            }
            Vector<unsigned> a(fObjectTopLeft.x + x, fObjectTopLeft.y + y);
            if (a.x >= 0 && a.x < size.x && a.y >= 0 && a.y < size.y && sample.a == 255)
                if (qDepthBuffer.at(a) >= fDistanceToPlayer) {
                    sample.shade(fShade);
                    img.setPixel(a, sample);
                    qDepthBuffer.at(a) = fDistanceToPlayer;
                }
        }
    }
}
