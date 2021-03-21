#include "RenderManager.hpp"
#include "Logger.hpp"
#include "MapManager.hpp"
#include "ObjectManager.hpp"
#include "Ray.hpp"
#include "StorageManager.hpp"
#include "ThreadManager.hpp"
#include <cmath>
#include <execution>
#include <unordered_set>

constexpr const float fFOV = M_PI / 4.0f;
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

    Ray ray;
    ray.fOrigin = pPov->getPosition();
    ray.uMap = pPov->getPosition();
    for (unsigned x = 0; x < size.x; ++x) {
        fur.at(x) = thread_manager->push(
            [this, &map, x](int, const float fAngle, Ray rayDef) {
                float fRayAngle = (fAngle - (fFOV / 2.0f)) + (float(x) / size.x) * fFOV;
                rayDef.fFish = std::cos(fRayAngle - fAngle);
                rayDef.fDirection.x = std::sin(fRayAngle);
                rayDef.fDirection.y = std::cos(fRayAngle);
                rayDef.shoot(map);
                rayDef.sample();
                rayDef.fDistance *= rayDef.fFish;
                std::fill(qDepthBuffer.at(x).begin(), qDepthBuffer.at(x).end(),
                          rayDef.fDistance);
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
    std::for_each(std::execution::par, qObj.begin(), qObj.end(),
                  [](auto &i) { i.get(); });

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
    const Frame &iWall(storage_manager->get<Frame>(map.getTextureName(ray.cHit)));
    const Vector<unsigned> uWallSize(iWall.getSize());
    const Frame &iFloor(storage_manager->get<Frame>(sFloorTexture));
    const Vector<unsigned> uFloorSize(iFloor.getSize());

    float fCeiling = (size.y >> 1) - (size.y / ray.fDistance);
    float fFloor = size.y - fCeiling;

    float fShade = 1.0f - std::min(ray.fDistance / fDepth, 1.0f);
    for (unsigned y = 0; y < size.y; ++y) {
        if (y <= fCeiling) {
            img.setPixel(x, y, Color::LightBlue);
        } else if (y > fCeiling && y <= fFloor) {
            ray.fSample.y = (y - fCeiling) / (fFloor - fCeiling);
            Vector<unsigned> uSampled = this->sampleVector(ray.fSample, uWallSize);
            Pixel sampled(iWall.getPixel(uSampled.x, uSampled.y));
            sampled.shade(fShade);
            img.setPixel(x, y, sampled);
        } else {
            float fPlaneZ = (size.y / 2.0f) / (float(y) - (size.y / 2.0f));
            Vector<float> fPlanePoint =
                ray.fOrigin + ray.fDirection * fPlaneZ * 2.0f / ray.fFish;
            Vector<unsigned> uPlane(fPlanePoint);
            Vector<float> fSample = fPlanePoint - uPlane;
            Vector<unsigned> uSampled = this->sampleVector(fSample, uFloorSize);
            Pixel sampled(iFloor.getPixel(uSampled.x, uSampled.y));
            img.setPixel(x, y, sampled);
        }
    }
}

void RenderManager::drawObject(const std::unique_ptr<AObject> &obj,
                               const Vector<float> &fCamPosition, const float &fEyeAngle)
{
    std::string texture(obj->getTextureName().value());
    Vector<float> fVec(obj->getPosition<float>() - fCamPosition);
    float fDistanceToPlayer(fVec.mag());
    float fObjectAngle(fEyeAngle - fVec.atan());

    bool bInCamFOV = std::abs(fObjectAngle) < (fFOV + (1.0f / fDistanceToPlayer)) / 2.0f;
    if (!bInCamFOV || fDistanceToPlayer < 0.5f || fDistanceToPlayer >= fDepth) return;
    const float fShade = 1.0f - std::min(fDistanceToPlayer / fDepth, 1.0f);
    const Frame &iSprite(storage_manager->get<Frame>(texture));
    Vector<unsigned> uImgSize(iSprite.getSize());
    float fObjCeiling = (size.y >> 1) - size.y / fDistanceToPlayer;
    float fObjFloor = size.y - fObjCeiling;

    if (fObjCeiling < 0 || fObjFloor < 0) return;
    Vector<float> fObject;
    fObject.y = fObjFloor - fObjCeiling;
    fObject.x = fObject.y / (float(uImgSize.y) / uImgSize.x);
    float fMiddleOfObject =
        (0.5f * (fObjectAngle / (fFOV / 2.0f)) + 0.5f) * (float(size.x));

    Vector<float> fObj;
    for (fObj.x = 0; fObj.x < fObject.x; ++fObj.x) {
        for (fObj.y = 0; fObj.y < fObject.y; ++fObj.y) {
            unsigned uObjectColumn = fMiddleOfObject + fObj.x - (fObject.x / 2.0f);
            if (uObjectColumn < size.x) {
                Vector<unsigned> uSample = this->sampleVector(fObj / fObject, uImgSize);
                Pixel sample = iSprite.getPixel(uSample.x, uSample.y);
                if (sample.a == 0) continue;
                if (qDepthBuffer.at(uObjectColumn, fObjCeiling + fObj.y) >=
                    fDistanceToPlayer) {
                    sample.b *= fShade;
                    sample.r *= fShade;
                    sample.g *= fShade;
                    img.setPixel(uObjectColumn, fObjCeiling + fObj.y, sample);
                    qDepthBuffer.at(uObjectColumn, fObjCeiling + fObj.y) =
                        fDistanceToPlayer;
                }
            }
        }
    }
}
