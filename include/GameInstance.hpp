#ifndef _GAMEINSTANCE_HPP_
#define _GAMEINSTANCE_HPP_

#include "Coords.hpp"
#include "Map.hpp"
#include "ObjectManager.hpp"
#include "Renderer.hpp"
#include "Storage.hpp"
#include "ThreadPool.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

class GameInstance
{
public:
    GameInstance(const unsigned, const unsigned);
    ~GameInstance();
    void init();
    void run();

private:
    void populateMap();
    void handleInput(const float &);

private:
    Coords<unsigned> uSize;
    Storage storage;
    const Map &map;

    ObjectManager objs;
    ThreadPool pool;
    Renderer rendy;

    sf::RenderWindow win;
};

#endif    //_GAMEINSTANCE_HPP_
