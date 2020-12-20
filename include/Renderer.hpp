#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include "Coords.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "ThreadedQ.hpp"
#include "interface/AThreaded.hpp"
#include "interface/IObject.hpp"
#include <SFML/Graphics.hpp>
#include <atomic>
#include <memory>
#include <unordered_map>

class Renderer : public AThreaded
{
public:
    Renderer(const Player &, const Map &, Coords<unsigned>, const std::string &);
    Renderer(const Renderer &) = delete;
    ~Renderer();
    virtual void run() final;
    virtual void stop() final;

private:
    const sf::Color sampleTexture(const Coords<float> &, const std::string &) const;
    const Coords<unsigned> sampleTextureCoords(const Coords<float> &,
                                               const Coords<float> &) const;
    float computeColumn(const unsigned &, Coords<float> &);
    void drawColumn(const float &, const unsigned x, Coords<float> &, sf::Image &);
    void drawObject(std::unique_ptr<IObject> &, sf::Image &);

public:
    Coords<unsigned> size;
    ThreadedQ<sf::Image> rendered;

private:
    std::atomic_bool bQuit = false;

    std::unordered_map<std::string, sf::Image> sprite_list;
    std::deque<std::unique_ptr<IObject>> qObject;
    std::deque<float> qDepthBuffer;

    const Player &player;
    const Map &map;
};

#endif    //_RENDERER_HPP_
