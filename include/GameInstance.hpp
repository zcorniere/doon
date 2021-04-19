#pragma once

#include "Frame.hpp"
#include "Map.hpp"
#include "Vector.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <tuple>
#include <vector>

class GameInstance
{
public:
    GameInstance(const Vector<unsigned> &);
    ~GameInstance();
    void init();
    void run();
    const Vector<unsigned> &getSize() const;

private:
    void populateMap(const Map &);
    void handleInput(const float &);

private:
    Vector<unsigned> uSize;
    std::vector<std::tuple<sf::Texture, sf::Sprite>> extraSprites;
    sf::RenderWindow win;
    sf::Texture texture;
    sf::Sprite sprite;
};
