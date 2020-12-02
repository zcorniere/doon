#include "Snitch.hpp"
#include "Renderer.hpp"
#include <math.h>

constexpr float fFOV = M_PI / 4.0f;
constexpr float fDepth = 32;

constexpr float fRayResolution = 0.01f;
const std::string sWallTexture("wall");

const sf::Color cFloor(0x32, 0x70, 0x34);

Renderer::Renderer(const Player &player, const Map &map, Coords<unsigned> size, const std::string &assets) :
    AThreaded(), size(std::move(size)), player(player), map(map)
{
    for (auto &f: std::filesystem::directory_iterator(assets)) {
        if (f.path().extension() != ".jpg") continue;
        sf::Image img;
        img.loadFromFile(f.path());
        sprite_list.insert({f.path().stem(), std::move(img)});
    }
    img.create(size.x, size.y);
    lastImg.create(size.x, size.y);
}

Renderer::~Renderer() {
    this->stop();
}

void Renderer::run() {
    while(!bQuit) {
        for (unsigned x = 0; x < size.x; ++x) {
            float fRayAngle = (player.angle - (fFOV / 2.0f)) + (float(x) / size.x) * fFOV;
            float fDistanceToWall = 0;
            float fSampleX = 0.0f;

            Coords<float> fEye(sinf(fRayAngle), cosf(fRayAngle));

            while(fDistanceToWall < fDepth) {
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
                        float fTestAngle = atan2f((fTestPoint.y - fBlockMid.y), (fTestPoint.x - fBlockMid.x));
                        if (fTestAngle >= -M_PI * 0.25f && fTestAngle < M_PI * 0.25f)
                            fSampleX = fTestPoint.y - float(nTest.y);
                        else if (fTestAngle >= M_PI * 0.25f && fTestAngle < M_PI * 0.75f)
                            fSampleX = fTestPoint.x - float(nTest.x);
                        else if (fTestAngle < -M_PI * 0.25f && fTestAngle >= -M_PI * 0.75f)
                            fSampleX = fTestPoint.x - float(nTest.x);
                        else if (fTestAngle >= M_PI * 0.75f || fTestAngle < -M_PI * 0.75f)
                            fSampleX = fTestPoint.y - float(nTest.y);
                        break;
                    }
                }
            }
            float fCeiling = float(size.y / 2.0) - size.y / fDistanceToWall;
            float fFloor = size.y - fCeiling;

            for (unsigned y = 0; y < size.y; ++y) {
                if (y <= unsigned(fCeiling)) {
                    img.setPixel(x, y, sf::Color::Blue);
                } else if (y > unsigned(fCeiling) && y <= unsigned(fFloor)) {
                    if (fDistanceToWall < fDepth)
                    {
                        sf::Image &wall = sprite_list.at(sWallTexture);
                        sf::Vector2u wall_size = wall.getSize();

                        float fSampleY = (float(y) - fCeiling) / (fFloor - fCeiling);

                        Coords<unsigned> uSample(
                            std::min(unsigned(fSampleX * float(wall_size.x)), wall_size.x - 1),
                            std::min(unsigned(fSampleY * float(wall_size.y)), wall_size.y - 1)
                        );
                        img.setPixel(x, y, wall.getPixel(uSample.x, uSample.y));
                    } else {
                        img.setPixel(x, y, sf::Color::Black);
                    }
                } else {
                    img.setPixel(x, y, cFloor);
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
    this->AThreaded::stop();
}

const sf::Image &Renderer::getImage(const bool &bWait) {
    std::unique_lock<std::mutex> ul(mRendy);
    if (bWait) {
        vRendy.wait(ul);
    }
    return lastImg;
}
