#include "GameInstance.hpp"
#include "FrameLimiter.hpp"
#include "Logger.hpp"
#include "interface/IMovement.hpp"
#include "objects/Fireball.hpp"
#include "objects/Poggers.hpp"
#include <random>

GameInstance::GameInstance(const unsigned windowWidth, const unsigned windowHeight)
    : uSize(windowWidth, windowHeight),
      win(sf::VideoMode(windowWidth, windowHeight), "N/A"),
      map(sMapPath),
      objs(pool),
      pool(),
      player(map.getSize() / 2),
      rendy(pool, player, map, uSize, sAssetsPath)
{
    logger.msg() << "Map height :" << map.height;
    logger.endl();
    logger.msg() << "Map width :" << map.width;
    logger.endl();
    logger.raw() << map;
    logger.endl();

    if (!std::getenv("DOON_NO_POGGERS")) {
        for (auto &i: map.getChars('P')) {
            Coords<float> fi = static_cast<Coords<float>>(i) + 0.5f;
            logger.info("GAME_INSTANCE") << "New Map Pogger: " << fi;
            logger.endl();
            objs.getObjects().push_back(std::make_unique<Poggers>(fi));
        }
        for (unsigned i = 0; i < 10; i++) {
            Coords<float> roll(std::rand() % map.getSize().x,
                               std::rand() % map.getSize().y);
            roll += 0.5f;
            if (map.at(roll) == '#') {
                i--;
            } else {
                logger.info("GAME_INSTANCE") << "New Pogger: " << roll;
                logger.endl();
                objs.getObjects().push_back(std::make_unique<Poggers>(roll));
            }
        }
    }
}

GameInstance::~GameInstance(){};

void GameInstance::init()
{
    win.setVerticalSyncEnabled(true);
    // win.clear(sf::Color::Black);
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
        if (secs >= 0.10f) {
            win.setTitle(std::to_string(1.0f / fElapsedTime));
            secs = 0;
        }

        this->handleInput(fElapsedTime);
        player.update(fElapsedTime);
        objs.update(fElapsedTime);
        texture.loadFromImage(rendy.update(objs));
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
                    case sf::Keyboard::P: {
                        logger.msg("PLAYER") << player.pos << ": " << player.angle;
                        logger.endl();
                    } break;
                    case sf::Keyboard::K: {
                        logger.raw() << rendy.getDepthBuffer();
                        logger.endl();
                    } break;
                    case sf::Keyboard::Space: {
                        player.shoot(objs);
                    } break;
                    case sf::Keyboard::LShift: {
                        player.fSpeedModifier = 0.5;
                    } break;
                    default: break;
                }
            } break;
            case sf::Event::KeyReleased: {
                switch (event.key.code) {
                    case sf::Keyboard::LShift: {
                        player.fSpeedModifier = 1;
                    } break;
                    default: break;
                }
            } break;
            default: break;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        player.pan(Movement::Panning::Up, fElapsedTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        player.pan(Movement::Panning::Down, fElapsedTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        player.rotate(Movement::Rotation::CounterClockwise, fElapsedTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        player.rotate(Movement::Rotation::Clockwise, fElapsedTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
        player.move(Movement::Move::Forward, fElapsedTime);
        if (map.at(player.getPlayerPos<unsigned>()) == '#')
            player.move(Movement::Move::Backward, fElapsedTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        player.move(Movement::Move::Backward, fElapsedTime);
        if (map.at(player.getPlayerPos<unsigned>()) == '#')
            player.move(Movement::Move::Forward, fElapsedTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        player.move(Movement::Move::Left, fElapsedTime);
        if (map.at(player.getPlayerPos<unsigned>()) == '#')
            player.move(Movement::Move::Right, fElapsedTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        player.move(Movement::Move::Right, fElapsedTime);
        if (map.at(player.getPlayerPos<unsigned>()) == '#')
            player.move(Movement::Move::Left, fElapsedTime);
    }
}
