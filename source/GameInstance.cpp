#include "GameInstance.hpp"
#include "FrameLimiter.hpp"
#include "Logger.hpp"
#include "objects/Fireball.hpp"

GameInstance::GameInstance(const unsigned windowWidth, const unsigned windowHeight)
    : uSize(windowWidth, windowHeight),
      win(sf::VideoMode(windowWidth, windowHeight), "N/A"),
      map(sMapPath),
      player(map.getSize() / 2),
      rendy(player, map, uSize, sAssetsPath)
{
    logger.msg() << "Map height :" << map.height;
    logger.endl();
    logger.msg() << "Map width :" << map.width;
    logger.endl();
    logger.raw() << map;
    logger.endl();
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

    if (!win.hasFocus()) return;
    while (win.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed: {
                win.close();
            } break;
            case sf::Event::KeyPressed: {
                switch (event.key.code) {
                    case sf::Keyboard::P:
                        logger.msg("PLAYER") << player.pos;
                        logger.endl();
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        player.pan(Player::Panning::Up, fElapsedTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        player.pan(Player::Panning::Down, fElapsedTime);
    }
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
