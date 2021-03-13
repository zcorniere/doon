#include "Renderer.hpp"
#include "Logger.hpp"
#include <cmath>
#include <execution>
#include <unordered_set>

constexpr const float fFOV = M_PI / 4.0f;
constexpr const float fDepth = 100;

constexpr const char sWallTexture[] = "redbrick";
constexpr const char sFloorTexture[] = "greystone";

Renderer::Renderer(ThreadPool &p, const Storage &s, const Coords<unsigned> sze)
    : size(std::move(sze)), pool(p), storage(s)
{
    qDepthBuffer.resize(size);
    img.create(size);
}

Renderer::~Renderer() {}

const uint8_t *Renderer::update(const Map &map, const ObjectManager &obj,
                                const unsigned uPovIndex)
{
    const auto &pPov = obj.at(uPovIndex);
    Coords<float> fEye(std::sin(pPov->getAngle()), std::cos(pPov->getAngle()));
    float fEyeAngle = fEye.atan();
    std::deque<std::future<void>> fur(size.x);
    std::deque<std::future<void>> qObj;

    Renderer::Ray ray;
    ray.fOrigin = pPov->getPosition();
    for (unsigned i = 0; i < size.x; ++i) {
        fur.at(i) = pool.push(
            [this, map](int, const float fAngle, const unsigned x, Renderer::Ray rayDef) {
                rayDef.fRayAngle = (fAngle - (fFOV / 2.0f)) + (float(x) / size.x) * fFOV;
                rayDef.fFish = std::cos(rayDef.fRayAngle - fAngle);
                rayDef.fDirection.x = std::sin(rayDef.fRayAngle);
                rayDef.fDirection.y = std::cos(rayDef.fRayAngle);
                this->computeColumn(map, rayDef);
                std::fill(qDepthBuffer.at(x).begin(), qDepthBuffer.at(x).end(),
                          rayDef.fDistance);
                this->drawColumn(x, rayDef);
            },
            pPov->getAngle(), i, ray);
    }
    std::for_each(std::execution::par, fur.begin(), fur.end(), [](auto &i) { i.wait(); });

    for (const auto &i: obj.getObjects()) {
        if (obj.at(uPovIndex) == i || !i->getTextureName()) continue;
        qObj.push_back(pool.push(
            [this, &i](int, const Coords<float> &fCamPosition, const float &fEyeAngle) {
                this->drawObject(i, fCamPosition, fEyeAngle);
            },
            pPov->getPosition(), fEyeAngle));
    }
    std::for_each(std::execution::par, qObj.begin(), qObj.end(),
                  [](auto &i) { i.wait(); });
    return img.getFramePtr();
}

void Renderer::resize(Coords<unsigned> fNewCoords)
{
    size = std::move(fNewCoords);
    img.create(size);
}

void Renderer::computeColumn(const Map &map, Renderer::Ray &ray) const
{
    Coords<float> fRayDelta(std::abs(1 / ray.fDirection.x),
                            std::abs(1 / ray.fDirection.y));
    Coords<unsigned> uMapCheck(ray.fOrigin);
    Coords<float> fRayLength;
    Coords<int> iStep;

    if (ray.fDirection.x < 0) {
        iStep.x = -1;
        fRayLength.x = (ray.fOrigin.x - float(uMapCheck.x)) * fRayDelta.x;
    } else {
        iStep.x = 1;
        fRayLength.x = (float(uMapCheck.x) + 1.0f - ray.fOrigin.x) * fRayDelta.x;
    }
    if (ray.fDirection.y < 0) {
        iStep.y = -1;
        fRayLength.y = (ray.fOrigin.y - float(uMapCheck.y)) * fRayDelta.y;
    } else {
        iStep.y = 1;
        fRayLength.y = (float(uMapCheck.y) + 1.0f - ray.fOrigin.y) * fRayDelta.y;
    }

    while (map.at(uMapCheck) != '#' && ray.fDistance < fDepth) {
        if (fRayLength.x < fRayLength.y) {
            uMapCheck.x += iStep.x;
            ray.fDistance = fRayLength.x;
            fRayLength.x += fRayDelta.x;
        } else {
            uMapCheck.y += iStep.y;
            ray.fDistance = fRayLength.y;
            fRayLength.y += fRayDelta.y;
        }
    }
    Coords<float> fIntersection(ray.fOrigin + ray.fDirection * ray.fDistance);
    Coords<float> fBlockMid(uMapCheck);
    fBlockMid += 0.5f;
    float fTestAngle =
        std::atan2((fIntersection.y - fBlockMid.y), (fIntersection.x - fBlockMid.x));
    if (fTestAngle >= -M_PI * 0.25f && fTestAngle < M_PI * 0.25f)
        ray.fSample.x = fIntersection.y - uMapCheck.y;
    else if (fTestAngle >= M_PI * 0.25f && fTestAngle < M_PI * 0.75f)
        ray.fSample.x = fIntersection.x - uMapCheck.x;
    else if (fTestAngle < -M_PI * 0.25f && fTestAngle >= -M_PI * 0.75f)
        ray.fSample.x = fIntersection.x - uMapCheck.x;
    else if (fTestAngle >= M_PI * 0.75f || fTestAngle < -M_PI * 0.75f)
        ray.fSample.x = fIntersection.y - uMapCheck.y;
    ray.fDistance *= ray.fFish;
}

void Renderer::drawColumn(const unsigned x, Renderer::Ray &ray)
{
    const Frame &iWall(storage.get<Frame>(sWallTexture));
    const Coords<unsigned> uWallSize(iWall.getSize());
    const Frame &iFloor(storage.get<Frame>(sFloorTexture));
    const Coords<unsigned> uFloorSize(iFloor.getSize());

    float fCeiling = (size.y >> 1) - (size.y / ray.fDistance);
    float fFloor = size.y - fCeiling;

    float fShade = 1.0f - std::min(ray.fDistance / fDepth, 1.0f);
    for (unsigned y = 0; y < size.y; ++y) {
        if (y <= fCeiling) {
            img.setPixel(x, y, Color::LightBlue);
        } else if (y > fCeiling && y <= fFloor) {
            if (ray.fDistance < fDepth && fShade > 0) {
                ray.fSample.y = (y - fCeiling) / (fFloor - fCeiling);
                Coords<unsigned> uSampled = this->sampleCoords(ray.fSample, uWallSize);
                Pixel sampled(iWall.getPixel(uSampled.x, uSampled.y));
                sampled.shade(fShade);
                img.setPixel(x, y, sampled);
            } else {
                img.setPixel(x, y, Color::Black);
            }
        } else {
            float fPlaneZ = (size.y / 2.0f) / (float(y) - (size.y / 2.0f));
            Coords<float> fPlanePoint =
                ray.fOrigin + ray.fDirection * fPlaneZ * 2.0f / ray.fFish;
            Coords<unsigned> uPlane(fPlanePoint);
            Coords<float> fSample = fPlanePoint - uPlane;
            Coords<unsigned> uSampled = this->sampleCoords(fSample, uFloorSize);
            Pixel sampled(iFloor.getPixel(uSampled.x, uSampled.y));
            img.setPixel(x, y, sampled);
        }
    }
}

const Pixel Renderer::sampleTexture(const Coords<float> &fSample,
                                    const std::string &texture) const
{
    try {
        const Frame &img(storage.get<Frame>(texture));
        const Coords<unsigned> imgSize = img.getSize();
        const Coords<unsigned> uSample(
            std::min(unsigned(fSample.x * imgSize.x), imgSize.x - 1),
            std::min(unsigned(fSample.y * imgSize.y), imgSize.y - 1));
        return img.getPixel(uSample.x, uSample.y);
    } catch (const std::out_of_range &oor) {
        logger.err("RENDERER") << "Missing texture : " << texture << "!";
        logger.endl();
        return sf::Color::Black;
    }
}

void Renderer::drawObject(const std::unique_ptr<AObject> &obj,
                          const Coords<float> &fCamPosition, const float &fEyeAngle)
{
    std::string texture(obj->getTextureName().value());
    if (!storage.contains<Frame>(texture)) {
        logger.err("RENDERER") << "Texture not found : " << texture;
        logger.endl();
        obj->setRemove(true);
        return;
    }
    Coords<float> fVec(obj->getPosition<float>() - fCamPosition);
    float fDistanceToPlayer(fVec.mag());
    float fObjectAngle(fEyeAngle - fVec.atan());

    bool bInCamFOV = std::abs(fObjectAngle) < (fFOV + (1.0f / fDistanceToPlayer)) / 2.0f;
    if (!bInCamFOV || fDistanceToPlayer < 0.5f || fDistanceToPlayer >= fDepth) return;
    const float fShade = 1.0f - std::min(fDistanceToPlayer / fDepth, 1.0f);
    const Frame &iSprite(storage.get<Frame>(texture));
    Coords<unsigned> uImgSize(iSprite.getSize());
    float fObjCeiling = (size.y >> 1) - size.y / fDistanceToPlayer;
    float fObjFloor = size.y - fObjCeiling;

    if (fObjCeiling < 0 || fObjFloor < 0) return;
    Coords<float> fObject;
    fObject.y = fObjFloor - fObjCeiling;
    fObject.x = fObject.y / (float(uImgSize.y) / uImgSize.x);
    float fMiddleOfObject =
        (0.5f * (fObjectAngle / (fFOV / 2.0f)) + 0.5f) * (float(size.x));

    Coords<float> fObj;
    for (fObj.x = 0; fObj.x < fObject.x; ++fObj.x) {
        for (fObj.y = 0; fObj.y < fObject.y; ++fObj.y) {
            unsigned uObjectColumn = fMiddleOfObject + fObj.x - (fObject.x / 2.0f);
            if (uObjectColumn < size.x) {
                Coords<unsigned> uSample = this->sampleCoords(fObj / fObject, uImgSize);
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
