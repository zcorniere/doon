#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include "Coords.hpp"
#include "DepthBuffer.hpp"
#include "Frame.hpp"
#include "Map.hpp"
#include "ObjectManager.hpp"
#include "Player.hpp"
#include "ThreadPool.hpp"
#include "abstract/AObject.hpp"
#include <SFML/Graphics.hpp>
#include <atomic>
#include <execution>
#include <memory>
#include <unordered_map>

constexpr float fFOV = M_PI / 4.0f;
constexpr float fDepth = 32;

class Renderer
{
public:
    Renderer(ThreadPool &, const Map &, Coords<unsigned>, const std::string &);
    ~Renderer();
    const uint8_t *update(const ObjectManager &, const unsigned);
    void resize(Coords<unsigned>);

private:
    const sf::Color sampleTexture(const Coords<float> &, const std::string &) const;
    const Coords<unsigned> sampleCoords(const Coords<float> &fSample,
                                        const Coords<float> &fSize) const
    {
        return {std::min(unsigned(fSample.x * fSize.x), unsigned(fSize.x) - 1),
                std::min(unsigned(fSample.y * fSize.y), unsigned(fSize.y) - 1)};
    }
    float computeColumn(const unsigned &, const float, const Coords<float> &,
                        Coords<float> &) const;
    void drawColumn(const float &, const unsigned x, Coords<float> &);

    void drawObject(const std::unique_ptr<AObject> &, const Coords<float> &,
                    const float &);

private:
    Frame img;
    Coords<unsigned> size;

    std::unordered_map<std::string, sf::Image> sprite_list;
    DepthBuffer qDepthBuffer;

    ThreadPool &pool;
    const Map &map;
};

#endif    //_RENDERER_HPP_
