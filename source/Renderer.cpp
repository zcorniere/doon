#include "Renderer.hpp"
#include "Snitch.hpp"
#include <cmath>
#include <unordered_set>

constexpr float fFOV = M_PI / 4.0f;
constexpr float fDepth = 32;

constexpr float fRayResolution = 0.01f;
const std::string sWallTexture("wall");

const sf::Color cFloor(0x32, 0x70, 0x34);
const std::unordered_set<std::string> valid_ext = {".jpg", ".png"};

Renderer::Renderer(const Player &player, const Map &map, Coords<unsigned> size,
                   const std::string &assets)
    : AThreaded(), size(std::move(size)), player(player), map(map)
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
        qObject.push_back({{8.5f, 8.5f}, "pogger"});
        // qObject.push_back({{7.5f, 7.5f}, "pogger"});
        // qObject.push_back({{10.5f, 3.5f}, "pogger"});
    } catch (const std::exception &e) {
        Snitch::err("RENDERER") << e.what() << Snitch::endl;
    }
}

Renderer::~Renderer() { this->stop(); }

void Renderer::run()
{
    while (!bQuit) {
        Coords<float> fSample;
        sf::Image img;
        img.create(size.x, size.y);

        for (unsigned x = 0; x < size.x; ++x) {
            float fDistanceToWall = this->computeColumn(x, fSample);
            this->drawColumn(fDistanceToWall, x, fSample, img);
        }
        for (auto &i: qObject) { this->drawObject(i, img); }
        rendered.push_back(img);
        this->wait();
    }
}

void Renderer::stop()
{
    bQuit = true;
    this->AThreaded::stop();
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
                          Coords<float> &fSample, sf::Image &img)
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

void Renderer::drawObject(Object &obj, sf::Image &img)
{
    if (!sprite_list.contains(obj.sTexture)) {
        Snitch::err("RENDERER") << "Texture not found : " << obj.sTexture << Snitch::endl;
        return;
    }
    if (obj.bRemove) return;
    Coords<float> fVec(obj.fPos - player.getPlayerPos<float>());
    float fDistanceToPlayer(fVec.mag());

    Coords<float> fEye(std::sin(player.angle), std::cos(player.angle));
    float fObjectAngle(fEye.atan() - fVec.atan());
    if (fObjectAngle < -M_PI)
        fObjectAngle += 2.0f * M_PI;
    else if (fObjectAngle > M_PI)
        fObjectAngle -= 2.f * M_PI;

    if (!(fabs(fObjectAngle) < fFOV / 2.0f) && fDistanceToPlayer < 0.5f &&
        fDistanceToPlayer >= fDepth) {
        return;
    }
    const sf::Image &iSprite = sprite_list.at(obj.sTexture);
    const auto imgSize = iSprite.getSize();
    const Coords<float> fImgSize(imgSize.x, imgSize.y);
    float fObjCeiling = size.y / 2.0f - size.y / fDistanceToPlayer;
    float fObjFloor = size.y - fObjCeiling;

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
                sf::Color sample = this->sampleTexture(fObj / fObject, obj.sTexture);
                if (sample.a > 0)
                    img.setPixel(uObjectColumn, fObjCeiling + fObj.y, sample);
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
