#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include "Coords.hpp"
#include "DepthBuffer.hpp"
#include "Map.hpp"
#include "ObjectManager.hpp"
#include "ThreadPool.hpp"
#include "ThreadedQ.hpp"
#include "abstract/AObject.hpp"
#include "abstract/AThreaded.hpp"
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include <atomic>
#include <execution>
#include <memory>
#include <unordered_map>
#include <vector>

constexpr float fFOV = M_PI / 4.0f;
constexpr float fDepth = 32;

class Renderer
{
public:
    Renderer(ThreadPool &, const Map &, Coords<unsigned>, const std::string &);
    ~Renderer();
    const sf::Image &update(const float fAngle, const Coords<float> &, ObjectManager &);

    const DepthBuffer &getDepthBuffer() const;

private:
    const sf::Color sampleTexture(const Coords<float> &, const std::string &) const;
    const Coords<unsigned> sampleTextureCoords(const Coords<float> &,
                                               const Coords<float> &) const;
    const Coords<unsigned> sampleTextureCoords(const Coords<float> &,
                                               const sf::Vector2u &) const;
    float computeColumn(const unsigned &, const float, const Coords<float> &, Coords<float> &) const;
    void drawColumn(const float &, const unsigned x, Coords<float> &);

    void drawObject(const std::unique_ptr<AObject> &, const Coords<float> &,
                    const float &);

private:
    sf::Image img;
    Coords<unsigned> size;

    std::unordered_map<std::string, sf::Image> sprite_list;
    DepthBuffer qDepthBuffer;

    ThreadPool &pool;
    const Map &map;
};

#endif    //_RENDERER_HPP_
