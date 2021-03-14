#include "GameInstance.hpp"
#include "FrameLimiter.hpp"
#include "Logger.hpp"
#include "Player.hpp"
#include "interface/IMovement.hpp"
#include "objects/Barrel.hpp"
#include "objects/Fireball.hpp"
#include "objects/Poggers.hpp"
#include <SFML/Graphics.hpp>
#include <random>

constexpr const char tIcon[] = "pogger";
constexpr const char sAssetsPath[] = "./assets/";

GameInstance::GameInstance(const Coords<unsigned> &u, ThreadPool &p, ObjectManager &o)
    : IGame(u, p, o), storage(sAssetsPath), mapName("main"), win()
{
    // Player is always the object placed at index 0
    object.addObject(std::make_unique<Player>(storage.get<Map>(mapName).getSize() / 2));
    logger.info("GAME_INSTANCE") << "Spawned Player at " << object.at(0)->getPosition();
    logger.endl();
    if (!std::getenv("DOON_NO_POGGERS")) this->populateMap(storage.get<Map>(mapName));
}

GameInstance::~GameInstance(){};

void GameInstance::init()
{
    const Frame &fr(storage.get<Frame>(tIcon));
    const Coords<unsigned> &uIconSize(fr.getSize());
    sf::ContextSettings setting;

    setting.antialiasingLevel = 2;
    win.create(sf::VideoMode(uSize.x, uSize.y), "N/A", sf::Style::Close, setting);
    win.setIcon(uIconSize.x, uIconSize.y, fr.getFramePtr());
    win.setVerticalSyncEnabled(true);
    win.setActive(false);
    win.clear();
    win.display();

    texture.create(uSize.x, uSize.y);

    std::pair<sf::Texture, sf::Sprite> crosshair;
    extraSprites.push_back(std::move(crosshair));
    const Frame &cros(storage.get<Frame>("crosshair"));
    Coords<unsigned> crossPosition(uSize / 2 - cros.getSize() / 2);
    crosshair.first.create(cros.getSize().x, cros.getSize().y);
    crosshair.first.update(cros.getFramePtr());
    crosshair.second.setPosition(crossPosition.x, crossPosition.y);
    extraSprites.push_back(std::move(crosshair));
    extraSprites.back().second.setTexture(extraSprites.back().first);
}

void GameInstance::update(const float fElapsedTime)
{
    static float secs = 0;

    secs += fElapsedTime;
    if (secs >= 0.10f) {
        win.setTitle(std::to_string(1.0f / fElapsedTime));
        secs = 0;
    }

    this->handleInput(fElapsedTime);
}

void GameInstance::drawToScreen(const uint8_t *const ptr)
{
    texture.update(ptr);
    sprite.setTexture(texture);

    win.draw(sprite);
    for (auto &[_, i]: extraSprites) { win.draw(i); }
    win.display();
}

const Coords<unsigned> &GameInstance::getSize() const { return uSize; }

bool GameInstance::isRunning() const { return win.isOpen(); }

const Map &GameInstance::getMap() const { return storage.get<Map>(mapName); }

const Frame &GameInstance::getTexture(const std::string &name) const
{
    return storage.get<Frame>(name);
}

void GameInstance::populateMap(const Map &map)
{
    for (const auto &i: map.getChars('P')) {
        Coords<float> fi = static_cast<Coords<float>>(i) + 0.5f;
        logger.info("GAME_INSTANCE") << "New Map Pogger: " << fi;
        logger.endl();
        object.addObject(std::make_unique<Poggers>(fi));
    }
    for (unsigned i = 0; i < 10; i++) {
        Coords<float> roll(std::rand() % map.getSize().x, std::rand() % map.getSize().y);
        roll += 0.5f;
        if (map.at(roll) == '#' || map.at(roll) == 'P') {
            i--;
        } else {
            logger.info("GAME_INSTANCE") << "New Pogger: " << roll;
            logger.endl();
            object.addObject(std::make_unique<Poggers>(roll));
        }
    }

    for (const auto &i: map.getChars('B')) {
        Coords<float> fCoords(i);
        fCoords += 0.5f;
        logger.info("GAME_INSTANCE") << "New Barrel: " << fCoords;
        logger.endl();
        object.addObject(std::make_unique<Barrel>(fCoords));
    }
}

void GameInstance::handleInput(const float &fElapsedTime)
{
    sf::Event event;
    Player *player = nullptr;

    if ((player = dynamic_cast<Player *>(object.at(0).get())) == nullptr) {
        logger.err("GAME_INSTANCE") << "Player not valid, aborting";
        logger.endl();
        std::abort();
    }

    if (!win.hasFocus()) return;
    while (win.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed: win.close(); break;
            // case sf::Event::Resized:
            //    rendy.resize(Coords(event.size.width, event.size.height));
            //    break;
            case sf::Event::KeyPressed: {
                switch (event.key.code) {
                    case sf::Keyboard::Escape: win.close(); break;
                    case sf::Keyboard::H: {
                        logger.debug() << "Screenshit !";
                        logger.endl();
                        texture.copyToImage().saveToFile("capture.png");
                    } break;
                    case sf::Keyboard::P: {
                        logger.msg("PLAYER")
                            << player->getPosition() << ": " << player->getAngle();
                        logger.endl();
                    } break;
                    case sf::Keyboard::R: {
                        object.getObjects().at(0)->setPosition(
                            storage.get<Map>(mapName).getSize() / 2);
                        object.getObjects().at(0)->setAngle(0.0f);
                    } break;
                    case sf::Keyboard::M: {
                        mapName = "s";
                        object.getObjects().at(0)->setPosition(
                            storage.get<Map>(mapName).getSize() / 2);
                        object.getObjects().at(0)->setAngle(0.0f);
                    } break;
                    case sf::Keyboard::Space: {
                        auto pl = player->shoot();
                        if (pl) object.addObject(std::move(pl));
                    } break;
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
