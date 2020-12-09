#ifndef _GAMEINSTANCE_HPP_
#define _GAMEINSTANCE_HPP_

#include "Coords.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Renderer.hpp"
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
    sf::RenderWindow win;
    const Map map;
    Player player;
    Renderer rendy;
};

#endif    //_GAMEINSTANCE_HPP_
