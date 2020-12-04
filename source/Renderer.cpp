#include "Renderer.hpp"
#include "Snitch.hpp"
#include <math.h>

constexpr float fFOV = M_PI / 4.0f;
constexpr float fDepth = 32;

constexpr float fRayResolution = 0.01f;
const std::string sWallTexture("wall");

const sf::Color cFloor(0x32, 0x70, 0x34);

Renderer::Renderer(const Player &player, const Map &map, Coords<unsigned> size,
                   const std::string &assets)
    : AThreaded(),
      t1(std::chrono::system_clock::now()),
      size(std::move(size)),
      player(player),
      map(map)
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
    img.create(size.x, size.y);
    lastImg.create(size.x, size.y);
}

Renderer::~Renderer() { this->stop(); }

void Renderer::run()
{
    while (!bQuit) {
        auto t2 = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTime = t2 - t1;
        t1 = std::move(t2);
        fElapsedTime = elapsedTime.count();

        for (unsigned x = 0; x < size.x; ++x) {
            float fRayAngle = (player.angle - (fFOV / 2.0f)) + (float(x) / size.x) * fFOV;

            float fDistanceToWall = 0;
            Coords<float> fSample;
            Coords<float> fEye(sinf(fRayAngle), cosf(fRayAngle));

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
                        Coords<float> fTestPoint(player.getPlayerPos<float>() +
                                                 fEye * fDistanceToWall);
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
            float fCeiling = float(size.y / 2.0) - size.y / fDistanceToWall;
            float fFloor = size.y - fCeiling;

            for (unsigned y = 0; y < size.y; ++y) {
                if (y <= unsigned(fCeiling)) {
                    img.setPixel(x, y, sf::Color::Blue);
                } else if (y > unsigned(fCeiling) && y <= unsigned(fFloor)) {
                    if (fDistanceToWall < fDepth) {
                        fSample.y = (y - fCeiling) / (fFloor - fCeiling);
                        img.setPixel(x, y, this->sampleTexture(fSample, "wall"));
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

void Renderer::stop()
{
    bQuit = true;
    this->AThreaded::stop();
}

const sf::Image &Renderer::getImage(const bool &bWait)
{
    std::unique_lock<std::mutex> ul(mRendy);
    if (bWait) { vRendy.wait(ul); }
    return lastImg;
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

const float &Renderer::getElapsedTime() const { return fElapsedTime; }
