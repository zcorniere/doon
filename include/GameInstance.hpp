#ifndef _GAMEINSTANCE_HPP_
#define _GAMEINSTANCE_HPP_

#include "Coords.hpp"
#include "interface/IGame.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class GameInstance : public IGame
{
public:
    GameInstance(const Coords<unsigned> &, Storage &, ThreadPool &, ObjectManager &);
    ~GameInstance();
    void init() final;
    void update(const float) final;
    void drawToScreen(const uint8_t *const) final;
    const Coords<unsigned> &getSize() const final;
    bool isRunning() const final;
    const std::string &getMapName() const final;

private:
    void populateMap(const Map &);
    void handleInput(const float &);

private:
    std::deque<std::pair<sf::Texture, sf::Sprite>> extraSprites;
    std::string mapName;
    sf::RenderWindow win;
    sf::Texture texture;
    sf::Sprite sprite;
};

#endif    //_GAMEINSTANCE_HPP_
