#ifndef _GAMEINSTANCE_HPP_
#define _GAMEINSTANCE_HPP_

#include "Coords.hpp"
#include "Map.hpp"
#include "ObjectManager.hpp"
#include "Renderer.hpp"
#include "Storage.hpp"
#include "ThreadPool.hpp"
#include "interface/IGame.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

class GameInstance : public IGame
{
public:
    GameInstance(const unsigned, const unsigned);
    ~GameInstance();
    void init() final;
    void run() final;

private:
    void populateMap(const Map &);
    void handleInput(const float &);

private:
    std::string mapName;
    Coords<unsigned> uSize;
    Storage storage;

    ObjectManager objs;
    ThreadPool pool;
    Renderer rendy;

    sf::RenderWindow win;
};

#endif    //_GAMEINSTANCE_HPP_
