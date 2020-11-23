#include <math.h>
#include "Player.hpp"
#include "Snitch.hpp"

constexpr float fMovementSpeed = 20.0f;

Player::Player(const float x, const float y) :
    x(x), y(y)
{}

Player::~Player() {}


void Player::rotate(const float r, const float &fElapsedTime) {
    angle += r * fElapsedTime;
}

void Player::move(const Player::Move dir, const float &fElapsedTime) {
    switch (dir) {
        case Move::Forward: {
            x += sinf(angle) * fMovementSpeed * fElapsedTime;
            y += cosf(angle) * fMovementSpeed * fElapsedTime;
        } break;
        case Move::Backward: {
            x -= sinf(angle) * fMovementSpeed * fElapsedTime;
            y -= cosf(angle) * fMovementSpeed * fElapsedTime;
        } break;
        case Move::Left: {
            x -= sinf(angle) * fMovementSpeed * fElapsedTime;
            y += cosf(angle) * fMovementSpeed * fElapsedTime;
        } break;
        case Move::Right: {
            x += sinf(angle) * fMovementSpeed * fElapsedTime;
            y -= cosf(angle) * fMovementSpeed * fElapsedTime;
        } break;
    }
}

template<>
Coords<unsigned> Player::getPlayerPos()const {
    return { static_cast<unsigned>(x), static_cast<unsigned>(y)};
}

template<>
Coords<float> Player::getPlayerPos()const {
    return { x, y };
}
