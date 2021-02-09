#include "Renderer.hpp"
#include "Logger.hpp"
#include <cmath>
#include <execution>
#include <unordered_set>

constexpr float fRayResolution = 0.01f;
const std::string sWallTexture("wall");

const sf::Color cFloor(0x32, 0x70, 0x34);
const std::unordered_set<std::string> valid_ext = {".jpg", ".png"};

Renderer::Renderer(ThreadPool &p, const Map &map, Coords<unsigned> size,
                   const std::string &assets)
    : size(std::move(size)), pool(p), map(map)
{
    try {
        for (auto &f: std::filesystem::directory_iterator(assets)) {
            if (!valid_ext.contains(f.path().extension())) continue;
            try {
                sf::Image img;
                img.loadFromFile(f.path());
                sprite_list.insert({f.path().stem(), std::move(img)});
            } catch (const std::exception &e) {
                logger.warn("RENDERER") << e.what();
                logger.endl();
            }
        }
    } catch (const std::exception &e) {
        logger.err("RENDERER") << e.what();
        logger.endl();
    }
    qDepthBuffer.resize(size);
    img.create(size.x, size.y);
}

Renderer::~Renderer() {}

const sf::Image &Renderer::update(const ObjectManager &obj, const unsigned uPovIndex)
{
    auto &pPov = obj.at(uPovIndex);
    Coords<float> fEye(std::sin(pPov->getAngle()), std::cos(pPov->getAngle()));
    float fEyeAngle = fEye.atan();
    std::deque<std::future<void>> fur(size.x);
    std::deque<std::future<void>> qObj;

    for (unsigned i = 0; i < size.x; ++i) {
        fur.at(i) = pool.push(
            [this](int, const unsigned x, const float angle,
                   const Coords<float> &fOrigin) {
                Coords<float> fSample;
                float fDistanceToWall = this->computeColumn(x, angle, fOrigin, fSample);
                std::fill(qDepthBuffer.buf.at(x).begin(), qDepthBuffer.buf.at(x).end(),
                          fDistanceToWall);
                this->drawColumn(fDistanceToWall, x, fSample);
            },
            i, pPov->getAngle(), pPov->getPosition());
    }
    std::for_each(std::execution::par, fur.begin(), fur.end(), [](auto &i) { i.wait(); });
    for (const auto &i: obj.getObjects()) {
        if (!i->getTextureName() && obj.at(uPovIndex) == i) continue;
        if (map.at(i->getPosition<unsigned>()) == '#') {
            i->onSceneryCollision(map);
            continue;
        }
        qObj.push_back(pool.push(
            [this, &i](int, const Coords<float> &fCamPosition, const float &fEyeAngle) {
                this->drawObject(i, fCamPosition, fEyeAngle);
            },
            pPov->getPosition(), fEyeAngle));
    }
    std::for_each(std::execution::par, qObj.begin(), qObj.end(),
                  [](auto &i) { i.wait(); });
    return img;
}

void Renderer::resize(Coords<unsigned> fNewCoords)
{
    size = std::move(fNewCoords);
    img.create(size.x, size.y);
}

float Renderer::computeColumn(const unsigned &x, const float angle,
                              const Coords<float> &fOrigin, Coords<float> &fSample) const
{
    float fDistanceToWall = 0;
    float fRayAngle = (angle - (fFOV / 2.0f)) + (float(x) / size.x) * fFOV;
    Coords<float> fEye(std::sin(fRayAngle), std::cos(fRayAngle));

    while (fDistanceToWall < fDepth) {
        fDistanceToWall += fRayResolution;

        Coords<unsigned> nTest = fOrigin + fEye * fDistanceToWall;

        if (nTest.x >= map.width || nTest.y >= map.height) {
            return fDepth;
        } else {
            if (map.at(nTest) == '#') {
                Coords<float> fBlockMid(nTest);
                fBlockMid += 0.5f;
                Coords<float> fTestPoint(fOrigin + fEye * fDistanceToWall);
                float fTestAngle = std::atan2((fTestPoint.y - fBlockMid.y),
                                              (fTestPoint.x - fBlockMid.x));
                if (fTestAngle >= -M_PI * 0.25f && fTestAngle < M_PI * 0.25f)
                    fSample.x = fTestPoint.y - nTest.y;
                else if (fTestAngle >= M_PI * 0.25f && fTestAngle < M_PI * 0.75f)
                    fSample.x = fTestPoint.x - nTest.x;
                else if (fTestAngle < -M_PI * 0.25f && fTestAngle >= -M_PI * 0.75f)
                    fSample.x = fTestPoint.x - nTest.x;
                else if (fTestAngle >= M_PI * 0.75f || fTestAngle < -M_PI * 0.75f)
                    fSample.x = fTestPoint.y - nTest.y;
                break;
            }
        }
    }
    return fDistanceToWall * std::cos(fRayAngle - angle);
}

void Renderer::drawColumn(const float &fDistanceToWall, const unsigned x,
                          Coords<float> &fSample)
{
    float fCeiling = (size.y / 2.0f) - (size.y / fDistanceToWall);
    float fFloor = size.y - fCeiling;

    float fShade = 1.0f - std::min(fDistanceToWall / fDepth, 1.0f);
    for (unsigned y = 0; y < size.y; ++y) {
        if (y <= fCeiling) {
            img.setPixel(x, y, sf::Color::Blue);
        } else if (y > fCeiling && y <= fFloor) {
            if (fDistanceToWall < fDepth && fShade > 0) {
                fSample.y = (y - fCeiling) / (fFloor - fCeiling);
                sf::Color sampled = this->sampleTexture(fSample, sWallTexture);
                sampled.b *= fShade;
                sampled.r *= fShade;
                sampled.g *= fShade;
                img.setPixel(x, y, sampled);
            } else {
                img.setPixel(x, y, sf::Color::Black);
            }
        } else {
            img.setPixel(x, y, cFloor);
        }
    }
}

const sf::Color Renderer::sampleTexture(const Coords<float> &fSample,
                                        const std::string &texture) const
{
    try {
        const sf::Image &img = sprite_list.at(texture);
        const sf::Vector2u imgSize = img.getSize();
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
    if (!sprite_list.contains(texture)) {
        logger.err("RENDERER") << "Texture not found : " << texture;
        logger.endl();
        obj->setRemove(true);
        return;
    }
    Coords<float> fVec(obj->getPosition<float>() - fCamPosition);
    float fDistanceToPlayer(fVec.mag());
    float fObjectAngle(fEyeAngle - fVec.atan());

    if (!(std::abs(fObjectAngle) < (fFOV + (1.0f / fDistanceToPlayer)) / 2.0f) &&
        fDistanceToPlayer < 0.5f && fDistanceToPlayer >= fDepth) {
        return;
    }
    const float fShade = 1.0f - std::min(fDistanceToPlayer / fDepth, 1.0f);
    const sf::Image &iSprite = sprite_list.at(texture);
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
                Coords<unsigned> uSample =
                    this->sampleTextureCoords(fObj / fObject, uImgSize);
                sf::Color sample = iSprite.getPixel(uSample.x, uSample.y);
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
const Coords<unsigned> Renderer::sampleTextureCoords(const Coords<float> &fSample,
                                                     const Coords<float> &fSize) const
{
    return {std::min(unsigned(fSample.x * fSize.x), unsigned(fSize.x) - 1),
            std::min(unsigned(fSample.y * fSize.y), unsigned(fSize.y) - 1)};
}
