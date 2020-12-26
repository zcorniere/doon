#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include "Coords.hpp"
#include "Map.hpp"
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
    Renderer(const Player &, const Map &, Coords<unsigned>, const std::string &);
    ~Renderer();
    const sf::Image &update();

    template <typename T>
    void addObject(std::unique_ptr<T> &obj)
    {
        static_assert(std::is_base_of<IObject, T>::value,
                      "Object must be derived from IObject interface");
        qObject.push_back(std::move(obj));
    }

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
    ThreadPool pool;

    Coords<unsigned> size;

    std::unordered_map<std::string, sf::Image> sprite_list;
    std::deque<std::unique_ptr<IObject>> qObject;
    std::vector<std::vector<float>> qDepthBuffer;

    const Player &player;
    const Map &map;
};

#endif    //_RENDERER_HPP_
