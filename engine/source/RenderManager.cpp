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

RenderManager::RenderManager() {}

RenderManager::~RenderManager() {}

const uint8_t *RenderManager::update(const unsigned uPovIndex)
{
    const auto &pPov = object_manager->at(uPovIndex);
    const auto &map = map_manager->get();
    Vector<float> fEye(std::sin(pPov->getAngle()), std::cos(pPov->getAngle()));
    float fEyeAngle = fEye.atan();
    std::deque<std::future<void>> fur(size.x);
    std::deque<std::future<void>> qObj;

    Ray ray(pPov->getPosition());
    for (unsigned x = 0; x < size.x; ++x) {
        fur.at(x) = thread_manager->push(
            [this, &map, x](int, const float fAngle, Ray rayDef) {
                float fRayAngle = (fAngle - (fFOV / 2.0f)) + (float(x) / size.x) * fFOV;
                rayDef.setRayDirection(fRayAngle);
                rayDef.fFish = std::cos(fRayAngle - fAngle);
                rayDef.shoot(map);
                rayDef.sample();
                rayDef.fDistance *= rayDef.fFish;
                qDepthBuffer.fill(x, rayDef.fDistance);
                this->drawColumn(map, x, rayDef);
            },
            pPov->getAngle(), ray);
    }
    std::for_each(fur.begin(), fur.end(), [](auto &i) { i.get(); });

    for (const auto &i: object_manager->getObjects()) {
        if (pPov == i || !i->getTextureName()) continue;
        qObj.push_back(thread_manager->push(
            [this, &i](int, const Vector<float> &fCamPosition, const float &fEyeAngle) {
                this->drawObject(i, fCamPosition, fEyeAngle);
            },
            pPov->getPosition(), fEyeAngle));
    }
    std::for_each(qObj.begin(), qObj.end(), [](auto &i) { i.get(); });

    return img.getFramePtr();
}

void RenderManager::resize(Vector<unsigned> fNewVector)
{
    size = std::move(fNewVector);
    qDepthBuffer.resize(size);
    img.create(size);
}

void RenderManager::drawColumn(const Map &map, const unsigned x, Ray &ray)
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
    for (unsigned y = 0; y < size.y; ++y) {
        if (y <= fCeiling) {
            float fPlaneZ = (size.y >> 1) / ((size.y >> 1) - float(y));
            Vector<float> fPlanePoint =
                ray.fOrigin + ray.fDirection * fPlaneZ * 2.0f / ray.fFish;
            Vector<unsigned> uPlane(fPlanePoint);
            Vector<float> fSample = fPlanePoint - uPlane;
            Vector<unsigned> uSampled = this->sampleVector(fSample, uSkySize);
            Pixel sampled(iSky.getPixel(uSampled));
            img.setPixel(x, y, sampled);
        } else if (y > fCeiling && y <= fFloor) {
            ray.fSample.y = (y - fCeiling) / (fFloor - fCeiling);
            Vector<unsigned> uSampled = this->sampleVector(ray.fSample, uWallSize);
            Pixel sampled(iWall.getPixel(uSampled));
            sampled.shade(fShade);
            img.setPixel(x, y, sampled);
        } else {
            float fPlaneZ = (size.y >> 1) / (float(y) - (size.y >> 1));
            Vector<float> fPlanePoint =
                ray.fOrigin + ray.fDirection * fPlaneZ * 2.0f / ray.fFish;
            Vector<unsigned> uPlane(fPlanePoint);
            Vector<float> fSample = fPlanePoint - uPlane;
            Vector<unsigned> uSampled = this->sampleVector(fSample, uFloorSize);
            Pixel sampled(iFloor.getPixel(uSampled));
            img.setPixel(x, y, sampled);
        }
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

    Vector<float> fObjectSize(obj->getSize());
    fObjectSize *= size.y << 1;
    fObjectSize /= fDistanceToPlayer;

    Vector<float> fObjectTopLeft;
    fObjectTopLeft.x = fFloorPoint.x - fObjectSize.x / 2.0f;
    fObjectTopLeft.y = fFloorPoint.y - fObjectSize.y;

    for (float y = 0; y < fObjectSize.y; y++) {
        for (float x = 0; x < fObjectSize.x; x++) {
            Vector<float> fSample;
            fSample.x = x / fObjectSize.x;
            fSample.y = y / fObjectSize.y;

            Pixel sample = iSprite.getPixel(this->sampleVector(fSample, uImgSize));
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
