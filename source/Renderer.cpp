#include "Renderer.hpp"
#include "Snitch.hpp"
#include <cmath>
#include <unordered_set>

constexpr float fFOV = M_PI / 4.0f;
constexpr float fDepth = 32;

constexpr float fRayResolution = 0.01f;
const std::string sWallTexture("wall");

const sf::Color cFloor(0x32, 0x70, 0x34);
const std::unordered_set<std::string> valid_ext = { ".jpg", ".png" };

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
    } catch (const std::filesystem::filesystem_error &fe) {
        Snitch::err("RENDERER") << fe.what() << Snitch::endl;
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
