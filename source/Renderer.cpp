#include "Renderer.hpp"
#include "Snitch.hpp"
#include <cmath>

constexpr float fFOV = M_PI / 4.0f;
constexpr float fDepth = 32;

constexpr float fRayResolution = 0.01f;
const std::string sWallTexture("wall");

const sf::Color cFloor(0x32, 0x70, 0x34);

Renderer::Renderer(const Player &player, const Map &map, Coords<unsigned> size,
                   const std::string &assets)
    : AThreaded(), size(std::move(size)), player(player), map(map)
{
    try {
        for (auto &f: std::filesystem::directory_iterator(assets)) {
            if (f.path().extension() != ".jpg") continue;
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
        sf::Image img;
        img.create(size.x, size.y);

        for (unsigned x = 0; x < size.x; ++x) {
            Coords<float> fSample;
            float fDistanceToWall = this->computeColumn(x, fSample);

            float fCeiling = (size.y / 2.0f) - (size.y / fDistanceToWall);
            float fFloor = size.y - fCeiling;

            float fShade = 1.0f - std::min(fDistanceToWall / fDepth, 1.0f);
            for (unsigned y = 0; y < size.y; ++y) {
                if (y <= unsigned(fCeiling)) {
                    img.setPixel(x, y, sf::Color::Blue);
                } else if (y > unsigned(fCeiling) && y <= unsigned(fFloor)) {
                    if (fDistanceToWall <= fDepth) {
                        fSample.y = (y - fCeiling) / (fFloor - fCeiling);
                        sf::Color sampled = this->sampleTexture(fSample, "wall");
                        sampled.b = uint8_t(float(sampled.b) * fShade);
                        sampled.r = uint8_t(float(sampled.r) * fShade);
                        sampled.g = uint8_t(float(sampled.g) * fShade);
                        img.setPixel(x, y, sampled);
                    } else {
                        img.setPixel(x, y, sf::Color::Black);
                    }
                } else {
                    img.setPixel(x, y, cFloor);
                }
            }
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
    float fRayAngle = (player.angle - (fFOV / 2.0f)) + (float(x) / size.x) * fFOV;

    float fDistanceToWall = 0;
    Coords<float> fEye(std::sin(fRayAngle), std::cos(fRayAngle));

    while (fDistanceToWall < fDepth) {
        fDistanceToWall += fRayResolution;

        Coords<unsigned> nTest = player.getPlayerPos<float>() + fEye * fDistanceToWall;

        if (nTest.x >= map.width || nTest.y >= map.height) {
            fDistanceToWall = fDepth;
            break;
        } else {
            if (map.at(nTest) == '#') {
                Coords<float> fBlockMid(nTest);
                fBlockMid += 0.5f;
                Coords<float> fTestPoint(player.getPlayerPos<float>() + fEye * fDistanceToWall);
                float fTestAngle =
                    atan2f((fTestPoint.y - fBlockMid.y), (fTestPoint.x - fBlockMid.x));
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

const sf::Color Renderer::sampleTexture(const Coords<float> &fSample,
                                        const std::string &texture) const
{
    try {
        const sf::Image &img = sprite_list.at(texture);
        const sf::Vector2u imgSize = img.getSize();
        const Coords<unsigned> uSample(std::min(unsigned(fSample.x * imgSize.x), imgSize.x - 1),
                                       std::min(unsigned(fSample.y * imgSize.y), imgSize.y - 1));
        return img.getPixel(uSample.x, uSample.y);
    } catch (const std::out_of_range &oor) {
        Snitch::err("RENDERER") << "Missing texture : " << texture << "!" << Snitch::endl;
        return sf::Color::Black;
    }
}
