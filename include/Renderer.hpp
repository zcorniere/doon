#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include "Coords.hpp"
#include "Map.hpp"
#include "Object.hpp"
#include "Player.hpp"
#include "ThreadPool.hpp"
#include "ThreadedQ.hpp"
#include "interface/AThreaded.hpp"
#include <SFML/Graphics.hpp>
#include <atomic>
#include <unordered_map>

class Renderer
{
public:
    Renderer(const Player &, const Map &, Coords<unsigned>, const std::string &);
    ~Renderer();
    const sf::Image &update();

private:
    const sf::Color sampleTexture(const Coords<float> &, const std::string &) const;
    const Coords<unsigned> sampleTextureCoords(const Coords<float> &,
                                               const Coords<float> &) const;
    const Coords<unsigned> sampleTextureCoords(const Coords<float> &fSample,
                                               const sf::Vector2u &fSize) const;
    float computeColumn(const unsigned &, Coords<float> &);
    void drawColumn(const float &, const unsigned x, Coords<float> &);
    void drawObject(Object &);

private:
    sf::Image img;
    ThreadPool pool;

    Coords<unsigned> size;

    std::unordered_map<std::string, sf::Image> sprite_list;
    std::deque<Object> qObject;
    std::deque<float> qDepthBuffer;

    const Player &player;
    const Map &map;
};

#endif    //_RENDERER_HPP_
