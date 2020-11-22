#include <math.h>
#include <array>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "FrameLimiter.hpp"
#include "Player.hpp"
#include "Map.hpp"
#include "Snitch.hpp"

constexpr int WindowWidth = 500;
constexpr int WindowHeight = 500;

constexpr float fFOV = M_PI / 4.0f;
constexpr float fDepth = 24;

int main()
{
    Map map("./map");
    Player player(map.width / 2, map.height / 2);
    FrameLimiter<60> limiter;

    Snitch::msg() << "Map height :" << map.height << Snitch::endl;
    Snitch::msg() << "Map width :" << map.width << Snitch::endl;
    std::cerr << map << std::endl;

    sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "Doon");
    sf::Image img;

    img.create(WindowWidth, WindowHeight);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::Closed: window.close(); break;
                case sf::Event::KeyPressed: {
                    switch (event.key.code) {
                        case sf::Keyboard::Q: player.rotate(-0.1f); break;
                        case sf::Keyboard::D: player.rotate(0.1f); break;
                        case sf::Keyboard::Z: {
                            player.move(Player::Move::Forward);
                            if (map.at(player.y * map.height + player.x) == '#')
                                player.move(Player::Move::Backward);
                        } break;
                        case sf::Keyboard::S: {
                            player.move(Player::Move::Backward);
                            if (map.at(player.y * map.height + player.x) == '#')
                                player.move(Player::Move::Forward);
                        } break;
                        default: break;
                    }
                } break;
                default: break;
            }
        }
        for (int x = 0; x < WindowWidth; x++) {
            float fRayAngle = (player.angle - fFOV / 2.0f) + ((float)x / (float)WindowWidth) * fFOV;
            float fDistanceToWall = 0;

            float fEyeX = sinf(fRayAngle);
            float fEyeY = cosf(fRayAngle);

            bool isEdge = false;
            while(1) {
                fDistanceToWall += 0.1f;

                unsigned nTestX = (int)(player.x + fEyeX * fDistanceToWall);
                unsigned nTestY = (int)(player.y + fEyeY * fDistanceToWall);
                if (fDistanceToWall >= fDepth ||
                    nTestX >= map.width ||
                    nTestY >= map.height) {
                    fDistanceToWall = fDepth;
                    break;
                } else {
                    if (map.at(nTestY * map.width + nTestX) == '#') {
                        std::array<std::pair<float, float>, 4> p;
                        for (unsigned tx = 0; tx < 2; tx++)
                            for (unsigned ty = 0; ty < 2; ty++) {
                                float vy = (float)nTestY + ty - player.y;
                                float vx = (float)nTestX + tx - player.x;
                                float d = sqrt(vx*vx + vy*vy);
                                float dot = (fEyeX * vx / d) + (fEyeY * vy / d);
                                p.at(tx + ty) = std::make_pair(d, dot);
                            }
                        std::sort(p.begin(), p.end(), [](const auto &left, const auto &right) { return left.first < right.first; });

                        float fBound = 0.01;
                        if (acos(p.at(0).second) < fBound) isEdge = true;
                        if (acos(p.at(1).second) < fBound) isEdge = true;

                        break;
                    }
                }
            }
            int nCeiling = (float)(WindowHeight / 2.0) - WindowHeight / ((float)fDistanceToWall);
            int nFloor = WindowHeight - nCeiling;

            sf::Color floor = sf::Color::Green;
            sf::Color shade = sf::Color::White;

            if (isEdge)
                shade = sf::Color::Black;
            else {
                if (fDistanceToWall <= fDepth / 4.0f) shade = sf::Color::White;
                else if (fDistanceToWall < fDepth / 3.0f) shade = sf::Color(0xc4, 0xc4, 0xc4);
                else if (fDistanceToWall < fDepth / 2.0f) shade = sf::Color(0x8c, 0x8c, 0x8c);
                else if (fDistanceToWall < fDepth) shade = sf::Color(0x52, 0x52, 0x52);
                else shade = sf::Color::Black;
            }
            for (int y = 0; y < WindowHeight; y++) {
                if (y < nCeiling)
                    img.setPixel(x, y, sf::Color::Blue);
                else if (y > nCeiling && y <= nFloor)
                    img.setPixel(x, y, shade);
                else {
                    const float b = 1.0f - (((float)y - WindowHeight / 2.0f) / ((float)WindowHeight / 2.0f));
                    if (b < 0.25) floor = sf::Color::Green;
                    else if (b < 0.5) floor = sf::Color(0x2a, 0xb5, 0x2c);
                    else if (b < 0.75) floor = sf::Color(0x2d, 0xa1, 0x2f);
                    else if (b < 0.9) floor = sf::Color(0x32, 0x70, 0x34);
                    else floor = sf::Color::Black;
                    img.setPixel(x, y, floor);
                }
            }
        }

        sf::Texture texture;
        texture.loadFromImage(img);
        sf::Sprite sprite(texture);
        window.clear();
        window.draw(sprite);
        window.display();
        limiter.sleep();
    }

    return 0;
}
