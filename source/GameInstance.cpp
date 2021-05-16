#include "GameInstance.hpp"
#include "FrameLimiter.hpp"
#include "Logger.hpp"
#include "MapManager.hpp"
#include "ObjectManager.hpp"
#include "Player.hpp"
#include "RenderManager.hpp"
#include "StorageManager.hpp"
#include "ThreadManager.hpp"
#include "objects/Barrel.hpp"
#include "objects/Fireball.hpp"
#include "objects/Poggers.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <random>

constexpr const char tIcon[] = "pogger";

GameInstance::GameInstance(const Vector<unsigned> &u): uSize(u), win()
{
    // Player is always the object placed at index 0
    object_manager->addObject(std::make_unique<Player>(map_manager->get().getSize() / 2));
    LOGGER_INFO << "Spawned Player at " << object_manager->at(0)->getPosition();
    logger->endl();
    if (!std::getenv("DOON_NO_POGGERS")) this->populateMap(map_manager->get());
}

GameInstance::~GameInstance(){};

void GameInstance::init()
{
    const Frame &fr(storage_manager->get<Frame>(tIcon));
    const Vector<unsigned> &uIconSize(fr.getSize());
    sf::ContextSettings setting;

    setting.antialiasingLevel = 2;
    win.create(sf::VideoMode(uSize.x, uSize.y), "N/A", sf::Style::Close, setting);
    win.setIcon(uIconSize.x, uIconSize.y, fr.getFramePtr());
    win.setVerticalSyncEnabled(true);
    win.setActive(false);
    win.clear();
    win.display();

    texture.create(uSize.x, uSize.y);

    const Frame &cros(storage_manager->get<Frame>("crosshair"));
    Vector<unsigned> crossPosition(uSize / 2 - cros.getSize() / 2);
    extraSprites.emplace_back();
    {
        sf::Texture &tex = std::get<0>(extraSprites.back());
        sf::Sprite &spr = std::get<1>(extraSprites.back());
        tex.create(cros.getSize().x, cros.getSize().y);
        tex.update(cros.getFramePtr());
        spr.setPosition(crossPosition.x, crossPosition.y);
        spr.setTexture(tex);
    }
}

void GameInstance::run()
{
    float secs = 0;
    float fElapsedTime = 0;
    while (win.isOpen()) {
        auto tp1 = std::chrono::high_resolution_clock::now();

        secs += fElapsedTime;
        if (secs >= 0.10f) {
            win.setTitle(std::to_string(1.0f / fElapsedTime));
            secs = 0;
        }

        this->handleInput(fElapsedTime);
        object_manager->update(fElapsedTime);
        texture.update(render_manager->update(0));
        sprite.setTexture(texture);

        win.draw(sprite);
        for (const auto &[_, i]: extraSprites) { win.draw(i); }
        win.display();
        auto tp2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsedTime(tp2 - tp1);
        fElapsedTime = elapsedTime.count();
    }
}

void GameInstance::populateMap(const Map &map)
{
    for (const auto &i: map.getChars('P')) {
        Vector<float> fi = static_cast<Vector<float>>(i) + 0.5f;
        LOGGER_INFO << "New Map Pogger: " << fi;
        logger->endl();
        object_manager->addObject(std::make_unique<Poggers>(fi));
    }
    for (unsigned i = 0; i < 10; i++) {
        Vector<float> roll(std::rand() % map.getSize().x, std::rand() % map.getSize().y);
        roll += 0.5f;
        if (map.at(roll) == '#' || map.at(roll) == 'P') {
            i--;
        } else {
            LOGGER_INFO << "New Pogger: " << roll;
            logger->endl();
            object_manager->addObject(std::make_unique<Poggers>(roll));
        }
    }

    for (const auto &i: map.getChars('B')) {
        Vector<float> fVector(i);
        fVector += 0.5f;
        LOGGER_INFO << "New Barrel: " << fVector;
        logger->endl();
        object_manager->addObject(std::make_unique<Barrel>(fVector));
    }
}

void GameInstance::handleInput(const float &fElapsedTime)
{
    sf::Event event;
    Player *player = nullptr;

    if ((player = dynamic_cast<Player *>(object_manager->at(0).get())) == nullptr) {
        LOGGER_ERR << "Player not valid, aborting";
        logger->endl();
        std::abort();
    }

    if (!win.hasFocus()) return;
    while (win.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed: win.close(); break;
            case sf::Event::Resized:
                render_manager->resize(Vector(event.size.width, event.size.height));
                break;
            case sf::Event::KeyPressed: {
                switch (event.key.code) {
                    case sf::Keyboard::Escape: win.close(); break;
                    case sf::Keyboard::H: {
                        LOGGER_DEBUG << "Screenshot !";
                        logger->endl();
                        texture.copyToImage().saveToFile("capture.png");
                    } break;
                    case sf::Keyboard::P: {
                        LOGGER_INFO << *player;
                        logger->endl();
                    } break;
                    case sf::Keyboard::R: {
                        object_manager->getObjects().at(0)->setPosition(
                            map_manager->get().getSize() / 2);
                        object_manager->getObjects().at(0)->setAngle(0.0f);
                    } break;
                    case sf::Keyboard::M: {
                        ++(*map_manager);
                        object_manager->getObjects().at(0)->setPosition(
                            map_manager->get().getSize() / 2);
                        object_manager->getObjects().at(0)->setAngle(0.0f);
                    } break;
                    case sf::Keyboard::Space: player->shoot(); break;
                    case sf::Keyboard::LShift: player->fSpeedModifier = 0.5; break;
                    default: break;
                }
            } break;
            case sf::Event::KeyReleased: {
                switch (event.key.code) {
                    case sf::Keyboard::LShift: player->fSpeedModifier = 1; break;
                    default: break;
                }
            } break;
            default: break;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        player->rotate(Movement::Rotation::CounterClockwise, fElapsedTime);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        player->rotate(Movement::Rotation::Clockwise, fElapsedTime);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        player->move(Movement::Move::Forward, fElapsedTime);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        player->move(Movement::Move::Backward, fElapsedTime);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        player->move(Movement::Move::Left, fElapsedTime);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        player->move(Movement::Move::Right, fElapsedTime);
}
