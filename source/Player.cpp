#include <math.h>
#include "Player.hpp"
#include "Snitch.hpp"

Player::Player(const float x, const float y) :
    x(x), y(y)
{}

Player::~Player() {}

void Player::rotate(const float r) {
    angle += r;
}

void Player::move(const Player::Move dir) {
    switch (dir) {
        case Move::Forward: {
            Snitch::debug() << "Forward" << Snitch::endl;
            x += sinf(angle) * 1.0f;
            y += cosf(angle) * 1.0f;
        } break;
        case Move::Backward: {
            Snitch::debug() << "Backward" << Snitch::endl;
            x -= sinf(angle) * 1.0f;
            y -= cosf(angle) * 1.0f;
        } break;
        case Move::Left: {

        } break;
        case Move::Right: {

        } break;
    }
}

template<>
Coords<unsigned> Player::getPlayerPos()const {
    return Coords<unsigned> { .x = static_cast<unsigned>(x),
                              .y = static_cast<unsigned>(y) };
}

template<>
Coords<float> Player::getPlayerPos()const {
    return Coords<float> { .x = x, .y = y };
}
