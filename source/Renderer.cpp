#include "Snitch.hpp"
#include "Renderer.hpp"
#include <math.h>
#include <iostream>

constexpr float fFOV = M_PI / 4.0f;
constexpr float fDepth = 32;

constexpr float fRayResolution = 0.01f;

Renderer::Renderer(const Player &player, const Map &map, Coords<unsigned> size) :
    IThreaded(), size(std::move(size)), player(player), map(map)
{
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
            float fRayAngle = (player.angle - fFOV / 2.0f) + (float(x) / size.x) * fFOV;
            float fDistanceToWall = 0;

            Coords<float> fEye = { .x = sinf(fRayAngle), .y = cosf(fRayAngle) };

            bool isEdge = false;
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
                        std::array<std::pair<float, float>, 4> p;
                        for (unsigned tx = 0; tx < 2; tx++)
                            for (unsigned ty = 0; ty < 2; ty++) {
                                Coords<float> v = {static_cast<float>(tx), static_cast<float>(ty)};
                                v += static_cast<Coords<float>>(nTest) - player.getPlayerPos<float>();
                                float d = v.mag();
                                v /= d;
                                float dot = v.dot(fEye);
                                p.at(tx + ty) = std::make_pair(d, dot);
                            }
                        std::sort(p.begin(), p.end(), [](const auto &left, const auto &right) { return left.first < right.first; });

                        float fBound = 0.005;
                        if (acos(p.at(0).second) < fBound) isEdge = true;
                        if (acos(p.at(1).second) < fBound) isEdge = true;
                        break;
                    }
                }
            }
            float fCeiling = (float(size.y) / 2.0) - (float(size.y) / fDistanceToWall);
            float fFloor = size.y - fCeiling;

            sf::Color floor = sf::Color::Green;
            sf::Color shade = sf::Color::White;

            if (isEdge)
                shade = sf::Color::Black;
            else {
                if (fDistanceToWall <= fDepth / 4.0f) shade = sf::Color::White;
                else if (fDistanceToWall < fDepth / 3.0f) shade = sf::Color(0xc4, 0xc4, 0xc4);
                else if (fDistanceToWall < fDepth / 2.0f) shade = sf::Color(0x8c, 0x8c, 0x8c);
                else if (fDistanceToWall < fDepth) shade = sf::Color(0x52, 0x52, 0x52);
                else shade = sf::Color::Black;
            }
            for (unsigned y = 0; y < size.y; y++) {
                if (y <= unsigned(fCeiling))
                    img->setPixel(x, y, sf::Color::Blue);
                else if (y > unsigned(fCeiling) && y <= unsigned(fFloor))
                    img->setPixel(x, y, shade);
                else {
                    const float b = 1.0f - (((float)y - size.y / 2.0f) / ((float)size.y / 2.0f));
                    if (b < 0.25) floor = sf::Color::Green;
                    else if (b < 0.5) floor = sf::Color(0x2a, 0xb5, 0x2c);
                    else if (b < 0.75) floor = sf::Color(0x2d, 0xa1, 0x2f);
                    else floor = sf::Color(0x32, 0x70, 0x34);
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

