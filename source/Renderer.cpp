#include "Renderer.hpp"
#include "Snitch.hpp"
#include <cmath>
#include <execution>
#include <unordered_set>

constexpr float fFOV = M_PI / 4.0f;
constexpr float fDepth = 32;

constexpr float fRayResolution = 0.01f;
const std::string sWallTexture("wall");

const sf::Color cFloor(0x32, 0x70, 0x34);
const std::unordered_set<std::string> valid_ext = {".jpg", ".png"};

Renderer::Renderer(const Player &player, const Map &map, Coords<unsigned> size,
                   const std::string &assets)
    : pool(2), size(std::move(size)), qDepthBuffer(size.x), player(player), map(map)
{
    try {
        for (auto &f: std::filesystem::directory_iterator(assets)) {
            if (!valid_ext.contains(f.path().extension())) continue;
            try {
                sf::Image img;
                img.loadFromFile(f.path());
                sprite_list.insert({f.path().stem(), std::move(img)});
            } catch (const std::exception &e) {
                Snitch::warn("RENDERER") << e.what() << Snitch::endl;
            }
        }
        qObject.push_back({{47.5f, 22.5f}, "pogger"});
        qObject.push_back({{47.5f, 19.5f}, "pogger"});
    } catch (const std::exception &e) {
        Snitch::err("RENDERER") << e.what() << Snitch::endl;
    }
    img.create(size.x, size.y);
}

Renderer::~Renderer() {}

const sf::Image &Renderer::update()
{
    auto computeCo = [this](int, unsigned x) {
        Coords<float> fSample;
        for (; x < size.x; x += 2) {
            float fDistanceToWall = this->computeColumn(x, fSample);
            qDepthBuffer.at(x) = fDistanceToWall;
            this->drawColumn(fDistanceToWall, x, fSample);
        }
    };

    std::deque<std::future<void>> fur;
    fur.resize(pool.size());
    for (unsigned i = 0; i < pool.size(); i++) { fur.at(i) = pool.push(computeCo, i); }
    std::for_each(std::execution::par, fur.begin(), fur.end(), [](auto &i) { i.wait(); });
    std::for_each(qObject.begin(), qObject.end(),
                  [this](auto &i) { this->drawObject(i); });
    return img;
}

float Renderer::computeColumn(const unsigned &x, Coords<float> &fSample)
{
    float fDistanceToWall = 0;
    float fRayAngle = (player.angle - (fFOV / 2.0f)) + (float(x) / size.x) * fFOV;
    Coords<float> fEye(std::sin(fRayAngle), std::cos(fRayAngle));

    while (fDistanceToWall < fDepth) {
        fDistanceToWall += fRayResolution;

        Coords<unsigned> nTest = player.getPlayerPos<float>() + fEye * fDistanceToWall;

        if (nTest.x >= map.width || nTest.y >= map.height) {
            return fDepth;
        } else {
            if (map.at(nTest) == '#') {
                Coords<float> fBlockMid(nTest);
                fBlockMid += 0.5f;
                Coords<float> fTestPoint(player.getPlayerPos<float>() +
                                         fEye * fDistanceToWall);
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
    return fDistanceToWall;
}

void Renderer::drawColumn(const float &fDistanceToWall, const unsigned x,
                          Coords<float> &fSample)
{
    const float fCeiling = (size.y / 2.0f) - (size.y / fDistanceToWall);
    const float fFloor = size.y - fCeiling;

    const float fShade = 1.0f - std::min(fDistanceToWall / fDepth, 1.0f);
    for (unsigned y = 0; y < size.y; ++y) {
        if (y <= fCeiling) {
            img.setPixel(x, y, sf::Color::Blue);
        } else if (y > fCeiling && y <= fFloor) {
            if (fDistanceToWall < fDepth && fShade > 0) {
                fSample.y = (y - fCeiling) / (fFloor - fCeiling);
                sf::Color sampled = this->sampleTexture(fSample, sWallTexture);
                sampled.b = sampled.b * fShade;
                sampled.r = sampled.r * fShade;
                sampled.g = sampled.g * fShade;
                img.setPixel(x, y, sampled);
            } else {
                img.setPixel(x, y, sf::Color::Black);
            }
        } else {
            img.setPixel(x, y, cFloor);
        }
    }
}

void Renderer::drawObject(Object &obj)
{
    if (obj.bRemove) return;
    if (!sprite_list.contains(obj.sTexture)) {
        Snitch::err("RENDERER") << "Texture not found : " << obj.sTexture << Snitch::endl;
        return;
    }
    Coords<float> fVec(obj.fPos - player.getPlayerPos<float>());
    float fDistanceToPlayer(fVec.mag());

    Coords<float> fEye(std::sin(player.angle), std::cos(player.angle));
    float fObjectAngle(fEye.atan() - fVec.atan());

    if (!(fabs(fObjectAngle) < fFOV / 2.0f) && fDistanceToPlayer < 0.5f &&
        fDistanceToPlayer >= fDepth) {
        return;
    }
    const sf::Image &iSprite = sprite_list.at(obj.sTexture);
    const sf::Vector2u imgSize = iSprite.getSize();
    Coords<float> fImgSize(imgSize.x, imgSize.y);
    float fObjCeiling = size.y / 2.0f - size.y / fDistanceToPlayer;
    float fObjFloor = size.y - fObjCeiling;

    if (fObjCeiling < 0 || fObjFloor < 0) return;
    Coords<float> fObject;
    fObject.y = fObjFloor - fObjCeiling;
    fObject.x = fObject.y / (imgSize.y / imgSize.x);
    float fMiddleOfObject =
        (0.5f * (fObjectAngle / (fFOV / 2.0f)) + 0.5f) * (float(size.x));

    Coords<float> fObj;
    for (fObj.x = 0; fObj.x < fObject.x; fObj.x++) {
        for (fObj.y = 0; fObj.y < fObject.y; fObj.y++) {
            unsigned uObjectColumn = fMiddleOfObject + fObj.x - (fObject.x / 2.0f);
            if (uObjectColumn < size.x) {
                Coords<unsigned> uSample =
                    this->sampleTextureCoords(fObj / fObject, imgSize);
                sf::Color sample = iSprite.getPixel(uSample.x, uSample.y);
                if (sample.a != 0 &&
                    qDepthBuffer.at(uObjectColumn) >= fDistanceToPlayer) {
                    img.setPixel(uObjectColumn, fObjCeiling + fObj.y, sample);
                    qDepthBuffer.at(uObjectColumn) = fDistanceToPlayer;
                }
            }
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
        Snitch::err("RENDERER") << "Missing texture : " << texture << "!" << Snitch::endl;
        return sf::Color::Black;
    }
}

const Coords<unsigned> Renderer::sampleTextureCoords(const Coords<float> &fSample,
                                                     const Coords<float> &fSize) const
{
    return {std::min(unsigned(fSample.x * fSize.x), unsigned(fSize.x) - 1),
            std::min(unsigned(fSample.y * fSize.y), unsigned(fSize.y) - 1)};
}

const Coords<unsigned> Renderer::sampleTextureCoords(const Coords<float> &fSample,
                                                     const sf::Vector2u &fSize) const
{
    return {std::min(unsigned(fSample.x * fSize.x), unsigned(fSize.x) - 1),
            std::min(unsigned(fSample.y * fSize.y), unsigned(fSize.y) - 1)};
}
