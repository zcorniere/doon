#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include "Coords.hpp"
#include "Map.hpp"
#include "ObjectManager.hpp"
#include "Player.hpp"
#include "ThreadPool.hpp"
#include "ThreadedQ.hpp"
#include "abstract/AThreaded.hpp"
#include "interface/IObject.hpp"
#include <SFML/Graphics.hpp>
#include <atomic>
#include <memory>
#include <unordered_map>
#include <vector>

class Renderer
{
public:
    Renderer(ThreadPool &, const Player &, const Map &, Coords<unsigned>,
             const std::string &);
    ~Renderer();
    const sf::Image &update(ObjectManager &);

private:
    const sf::Color sampleTexture(const Coords<float> &, const std::string &) const;
    const Coords<unsigned> sampleTextureCoords(const Coords<float> &,
                                               const Coords<float> &) const;
    const Coords<unsigned> sampleTextureCoords(const Coords<float> &,
                                               const sf::Vector2u &) const;
    float computeColumn(const unsigned &, Coords<float> &) const;
    void drawColumn(const float &, const unsigned x, Coords<float> &);
    void drawObject(std::unique_ptr<IObject> &);

private:
    sf::Image img;

    Coords<unsigned> size;

    std::unordered_map<std::string, sf::Image> sprite_list;
    std::vector<std::vector<float>> qDepthBuffer;

    ThreadPool &pool;
    const Player &player;
    const Map &map;
};

#endif    //_RENDERER_HPP_
