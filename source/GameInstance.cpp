#include "GameInstance.hpp"
#include "objects/Fireball.hpp"
#include <FrameLimiter.hpp>

GameInstance::GameInstance(const unsigned windowWidth, const unsigned windowHeight)
    : uSize(windowWidth, windowHeight),
      win(sf::VideoMode(windowWidth, windowHeight), "N/A"),
      map(sMapPath),
      player(map.getSize() / 2),
      rendy(player, map, uSize, sAssetsPath)
{
    Snitch::msg() << "Map height :" << map.height << Snitch::endl;
    Snitch::msg() << "Map width :" << map.width << Snitch::endl;
    std::cerr << map << std::endl;
}

GameInstance::~GameInstance(){};

void GameInstance::init()
{
    win.setVerticalSyncEnabled(true);
    win.clear(sf::Color::Black);
    win.display();
}

void GameInstance::run()
{
    FrameLimiter<60> limiter;
    sf::Texture texture;
    sf::Sprite sprite;

    float secs = 0;
    auto tp1 = std::chrono::system_clock::now();

    while (win.isOpen()) {
        auto tp2 = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = std::move(tp2);

        float fElapsedTime = elapsedTime.count();
        secs += fElapsedTime;
        if (secs >= 0.25f) {
            win.setTitle(std::to_string(1.0f / fElapsedTime));
            secs = 0;
        }

        this->handleInput(fElapsedTime);
        texture.loadFromImage(rendy.update());
        sprite.setTexture(texture);

        win.draw(sprite);
        win.display();
        limiter.sleep();
    }
}

void GameInstance::handleInput(const float &fElapsedTime)
{
    sf::Event event;

    while (win.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed: {
                win.close();
            } break;
            case sf::Event::KeyPressed: {
                switch (event.key.code) {
                    case sf::Keyboard::P:
                        Snitch::msg("PLAYER") << player.pos << Snitch::endl;
                        break;
                    case sf::Keyboard::Space: {
                        float fNoise = (((float)rand() / (float)RAND_MAX) - 0.5f) * 0.1f;
                        Coords<float> fObjV;
                        fObjV.x = std::sin(player.angle + fNoise) * 14.0f * fElapsedTime;
                        fObjV.y = std::cos(player.angle + fNoise) * 14.0f * fElapsedTime;
                        auto obj =
                            std::make_unique<Fireball>(player.pos, std::move(fObjV));
                        rendy.addObject(obj);
                    } break;
                    default: break;
                }
            } break;
            default: break;
        }
    }
    if (!win.hasFocus()) return;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        player.rotate(Player::Rotation::CounterClockwise, fElapsedTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        player.rotate(Player::Rotation::Clockwise, fElapsedTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
        player.move(Player::Move::Forward, fElapsedTime);
        if (map.at(player.getPlayerPos<unsigned>()) == '#')
            player.move(Player::Move::Backward, fElapsedTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        player.move(Player::Move::Backward, fElapsedTime);
        if (map.at(player.getPlayerPos<unsigned>()) == '#')
            player.move(Player::Move::Forward, fElapsedTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        player.move(Player::Move::Left, fElapsedTime);
        if (map.at(player.getPlayerPos<unsigned>()) == '#')
            player.move(Player::Move::Right, fElapsedTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        player.move(Player::Move::Right, fElapsedTime);
        if (map.at(player.getPlayerPos<unsigned>()) == '#')
            player.move(Player::Move::Left, fElapsedTime);
    }
}
