#include "Snitch.hpp"
#include "Renderer.hpp"
#include <math.h>

constexpr float fFOV = M_PI / 4.0f;
constexpr float fDepth = 24;

constexpr float fRayResolution = 0.01f;

Renderer::Renderer(const Player &player, const Map &map, Coords<unsigned> size) :
    IThreaded(), size(std::move(size)), player(player), map(map)
{
    img.create(size.x, size.y);
}

Renderer::~Renderer() {
    this->stop();
}

void Renderer::run() {
    while(!bQuit) {
        for (unsigned x = 0; x < size.x; x++) {
            float fRayAngle = (player.angle - fFOV / 2.0f) + ((float)x / (float)size.x) * fFOV;
            float fDistanceToWall = 0;

            float fEyeX = sinf(fRayAngle);
            float fEyeY = cosf(fRayAngle);

            bool isEdge = false;
            while(1) {
                fDistanceToWall += fRayResolution;

                unsigned nTestX = (int)(player.x + fEyeX * fDistanceToWall);
                unsigned nTestY = (int)(player.y + fEyeY * fDistanceToWall);
                if (fDistanceToWall >= fDepth ||
                    nTestX >= map.width ||
                    nTestY >= map.height) {
                    fDistanceToWall = fDepth;
                    break;
                } else {
                    if (map.at(nTestY * map.width + nTestX) == '#') {
                        std::array<std::pair<float, float>, 4> p;
                        for (unsigned tx = 0; tx < 2; tx++)
                            for (unsigned ty = 0; ty < 2; ty++) {
                                float vy = (float)nTestY + ty - player.y;
                                float vx = (float)nTestX + tx - player.x;
                                float d = sqrt(vx*vx + vy*vy);
                                float dot = (fEyeX * vx / d) + (fEyeY * vy / d);
                                p.at(tx + ty) = std::make_pair(d, dot);
                            }
                        std::sort(p.begin(), p.end(), [](const auto &left, const auto &right) { return left.first < right.first; });

                        float fBound = 0.01;
                        if (acos(p.at(0).second) < fBound) isEdge = true;
                        if (acos(p.at(1).second) < fBound) isEdge = true;
                        break;
                    }
                }
            }
            unsigned nCeiling = (float)(size.y / 2.0) - size.y / ((float)fDistanceToWall);
            unsigned nFloor = size.y - nCeiling;

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
                if (y < nCeiling)
                    img.setPixel(x, y, sf::Color::Blue);
                else if (y > nCeiling && y <= nFloor)
                    img.setPixel(x, y, shade);
                else {
                    const float b = 1.0f - (((float)y - size.y / 2.0f) / ((float)size.y / 2.0f));
                    if (b < 0.25) floor = sf::Color::Green;
                    else if (b < 0.5) floor = sf::Color(0x2a, 0xb5, 0x2c);
                    else if (b < 0.75) floor = sf::Color(0x2d, 0xa1, 0x2f);
                    else if (b < 0.9) floor = sf::Color(0x32, 0x70, 0x34);
                    else floor = sf::Color::Black;
                    img.setPixel(x, y, floor);
                }
            }
        }
        {
            std::unique_lock<std::mutex> ul(mRendy);
            vRendy.notify_one();
        }
        this->wait();
    }
}

void Renderer::stop() {
    bQuit = true;
    this->IThreaded::stop();
}

const sf::Image &Renderer::getImage(const bool &bWait) {
    if (bWait) {
        std::unique_lock<std::mutex> ul(mRendy);
        vRendy.wait(ul);
    }
    return img;
}

