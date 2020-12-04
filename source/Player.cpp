#include "Player.hpp"
#include "Snitch.hpp"
#include <math.h>

constexpr float fMovementSpeed = 25.0f;
constexpr float fRotationSpeed = 4.0f;

Player::Player(const float x, const float y): x(x), y(y) {}

Player::~Player() {}

void Player::rotate(const Player::Rotation dir, const float &fElapsedTime)
{
    switch (dir) {
        case Rotation::Clockwise: {
            angle += fRotationSpeed * fElapsedTime;
        } break;
        case Rotation::CounterClockwise: {
            angle -= fRotationSpeed * fElapsedTime;
        } break;
    }
    if (angle < -M_PI) angle += M_PI_2;
    if (angle > M_PI) angle -= M_PI_2;
}

void Player::move(const Player::Move dir, const float &fElapsedTime)
{
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
            x -= cosf(angle) * fMovementSpeed * fElapsedTime;
            y += sinf(angle) * fMovementSpeed * fElapsedTime;
        } break;
        case Move::Right: {
            x += cosf(angle) * fMovementSpeed * fElapsedTime;
            y -= sinf(angle) * fMovementSpeed * fElapsedTime;
        } break;
    }
}
