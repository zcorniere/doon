#include "GameInstance.hpp"
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
    rendy.run_threaded(false);
    win.setVerticalSyncEnabled(true);
    win.clear(sf::Color::Black);
    win.display();
}

void GameInstance::run()
{
    FrameLimiter<60> limiter;
    sf::Image img;
    sf::Texture texture;
    sf::Sprite sprite;

    auto tp1 = std::chrono::system_clock::now();

    while (win.isOpen()) {
        rendy.update();
        auto tp2 = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = std::move(tp2);

        float fElapsedTime = elapsedTime.count();
        win.setTitle(std::to_string(1.0f / fElapsedTime));

        this->handleInput(fElapsedTime);
        rendy.rendered.wait();
        img = rendy.rendered.pop_back();
        texture.loadFromImage(img);
        sprite.setTexture(texture);

        win.clear();
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
