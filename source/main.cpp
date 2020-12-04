#include <math.h>
#include <array>
#include <iostream>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "FrameLimiter.hpp"
#include "Player.hpp"
#include "Map.hpp"
#include "Snitch.hpp"
#include "Renderer.hpp"

constexpr int WindowWidth = 1280;
constexpr int WindowHeight = 960;

constexpr char sAssetsPath[] = "./assets/";
constexpr char sMapPath[] = "./map";

int main()
{
    const Map map(sMapPath);
    Player player(map.width / 2, map.height / 2);
    FrameLimiter<60> limiter;

    Snitch::msg() << "Map height :" << map.height << Snitch::endl;
    Snitch::msg() << "Map width :" << map.width << Snitch::endl;
    std::cerr << map << std::endl;

    Renderer rendy(player, map, {WindowWidth, WindowHeight}, sAssetsPath);
    rendy.run_threaded();

    sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "N/A");
    window.setVerticalSyncEnabled(true);

    auto tp1 = std::chrono::system_clock::now();

    bool bUpdate = true;
    while (window.isOpen())
    {
        auto tp2 = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = std::move(tp2);

        const float fElapsedTime = elapsedTime.count();
        window.setTitle(std::to_string(1.0f / fElapsedTime));

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::Closed: window.close(); bUpdate = false; break;
                case sf::Event::KeyPressed: {
                    switch (event.key.code) {
                        case sf::Keyboard::A: {
                            player.rotate(Player::Rotation::CounterClockwise, fElapsedTime);
                            bUpdate = true;
                        } break;
                        case sf::Keyboard::E: {
                            player.rotate(Player::Rotation::Clockwise, fElapsedTime);
                            bUpdate = true;
                        } break;
                        case sf::Keyboard::Z: {
                            player.move(Player::Move::Forward, fElapsedTime);
                            if (map.at(player.getPlayerPos<unsigned>()) == '#')
                                player.move(Player::Move::Backward, fElapsedTime);
                            bUpdate = true;
                        } break;
                        case sf::Keyboard::S: {
                            player.move(Player::Move::Backward, fElapsedTime);
                            if (map.at(player.getPlayerPos<unsigned>()) == '#')
                                player.move(Player::Move::Forward, fElapsedTime);
                            bUpdate = true;
                        } break;
                        case sf::Keyboard::Q: {
                            player.move(Player::Move::Left, fElapsedTime);
                            if (map.at(player.getPlayerPos<unsigned>()) == '#')
                                player.move(Player::Move::Right, fElapsedTime);
                            bUpdate = true;
                        } break;
                        case sf::Keyboard::D: {
                            player.move(Player::Move::Right, fElapsedTime);
                            if (map.at(player.getPlayerPos<unsigned>()) == '#')
                                player.move(Player::Move::Left, fElapsedTime);
                            bUpdate = true;
                        } break;
                        default: break;
                    }
                } break;
                default: break;
            }
        }

        if (bUpdate) {
            rendy.update();
        }
        sf::Texture texture;
        texture.loadFromImage(rendy.getImage());
        sf::Sprite sprite(texture);
        window.clear();
        window.draw(sprite);
        window.display();
        limiter.sleep();
        bUpdate = false;
    }

    return 0;
}
