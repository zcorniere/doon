#ifndef _GAMEINSTANCE_HPP_
#define _GAMEINSTANCE_HPP_

#include "Coords.hpp"
#include "Map.hpp"
#include "ObjectManager.hpp"
#include "Player.hpp"
#include "Renderer.hpp"
#include "ThreadPool.hpp"
#include <SFML/Graphics.hpp>

constexpr char sMapPath[] = "./map";
constexpr char sAssetsPath[] = "./assets/";

class GameInstance
{
public:
    GameInstance(const unsigned, const unsigned);
    ~GameInstance();
    void init();
    void run();

private:
    void handleInput(const float &);

private:
    Coords<unsigned> uSize;

    const Map map;
    ObjectManager objs;
    ThreadPool pool;
    Renderer rendy;

    sf::RenderWindow win;
};

#endif    //_GAMEINSTANCE_HPP_
