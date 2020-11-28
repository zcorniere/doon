#include "Snitch.hpp"
#include "Renderer.hpp"
#include <math.h>
#include <iostream>

constexpr float fFOV = M_PI / 4.0f;
constexpr float fDepth = 32;

constexpr float fRayResolution = 0.01f;

Renderer::Renderer(const Player &player, const Map &map, Coords<unsigned> size, const std::string &assets) :
    IThreaded(), size(std::move(size)), player(player), map(map)
{
    for (auto &f: std::filesystem::directory_iterator(assets)) {
        if (f.path().extension() != ".jpg") continue;
        sf::Image img;
        img.loadFromFile(f.path());
        sprite_list.insert({f.path().stem(), std::move(img)});
    }
    img = std::make_shared<sf::Image>();
    img->create(size.x, size.y);
    lastImg = std::make_shared<sf::Image>();
    lastImg->create(size.x, size.y);
}

Renderer::~Renderer() {
    this->stop();
}

void Renderer::run() {
    while(!bQuit) {
        for (unsigned x = 0; x < size.x; x++) {
            float fRayAngle = (player.angle - (fFOV / 2.0f)) + (float(x) / size.x) * fFOV;
            float fDistanceToWall = 0;
            float fSampleX = 0.0f;

            Coords<float> fEye = { .x = sinf(fRayAngle), .y = cosf(fRayAngle) };

            while(1) {
                fDistanceToWall += fRayResolution;

                Coords<unsigned> nTest = player.getPlayerPos<float>() + fEye * fDistanceToWall;

                if (fDistanceToWall >= fDepth ||
                    nTest.x >= map.width ||
                    nTest.y >= map.height) {
                    fDistanceToWall = fDepth;
                    break;
                } else {
                    if (map.at(nTest) == '#') {
                        Coords<float> fBlockMid = static_cast<Coords<float>>(nTest) + 0.5f;
                        Coords<float> fTestPoint = player.getPlayerPos<float>() + fEye * fDistanceToWall;
                        float fTestAngle = atan2f((fTestPoint.y - fBlockMid.y), (fTestPoint.x - fBlockMid.x));
                        if (fTestAngle >= -M_PI * 0.25f && fTestAngle < M_PI * 0.25f)
                            fSampleX = fTestPoint.y - float(nTest.y);
                        if (fTestAngle >= M_PI * 0.25f && fTestAngle < M_PI * 0.75f)
                            fSampleX = fTestPoint.x - float(nTest.x);
                        if (fTestAngle < -M_PI * 0.25f && fTestAngle >= -M_PI * 0.75f)
                            fSampleX = fTestPoint.x - float(nTest.x);
                        if (fTestAngle >= M_PI * 0.75f || fTestAngle < -M_PI * 0.75f)
                            fSampleX = fTestPoint.y - float(nTest.y);
                        break;
                    }
                }
            }
            float fCeiling = float(size.y / 2.0) - size.y / fDistanceToWall;
            float fFloor = size.y - fCeiling;

            sf::Color floor = sf::Color(0x32, 0x70, 0x34);

            for (unsigned y = 0; y < size.y; y++) {
                if (y <= unsigned(fCeiling)) {
                    img->setPixel(x, y, sf::Color::Blue);
                } else if (y > unsigned(fCeiling) && y <= unsigned(fFloor)) {
                    if (fDistanceToWall < fDepth)
                    {
                        sf::Image &wall = sprite_list.at("wall");
                        sf::Vector2u wall_size = wall.getSize();

                        float fSampleY = (float(y) - fCeiling) / (fFloor - fCeiling);

                        Coords<unsigned> uSample = {
                            std::min(unsigned(fSampleX * float(wall_size.x)), wall_size.x - 1),
                            std::min(unsigned(fSampleY * float(wall_size.y)), wall_size.y - 1)
                        };
                        img->setPixel(x, y, wall.getPixel(uSample.x, uSample.y));
                    } else {
                        img->setPixel(x, y, sf::Color::Black);
                    }
                } else {
                    img->setPixel(x, y, floor);
                }
            }
        }
        {
            std::unique_lock<std::mutex> ul(mRendy);
            std::swap(img, lastImg);
            vRendy.notify_one();
        }
        this->wait();
    }
}

void Renderer::stop() {
    bQuit = true;
    this->IThreaded::stop();
}

const std::shared_ptr<sf::Image> Renderer::getImage(const bool &bWait) {
    std::unique_lock<std::mutex> ul(mRendy);
    if (bWait) {
        vRendy.wait(ul);
    }
    return lastImg;
}
