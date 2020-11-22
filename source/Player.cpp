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
            x += sinf(angle) * 1.0f;
            y += cosf(angle) * 1.0f;
        } break;
        case Move::Backward: {
            x -= sinf(angle) * 1.0f;
            y -= cosf(angle) * 1.0f;
        } break;
        case Move::Left: {

        } break;
        case Move::Right: {

        } break;
    }
}
