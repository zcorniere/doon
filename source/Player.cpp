#include "Player.hpp"
#include <math.h>

constexpr float fMovementSpeed = 10.0f;
constexpr float fRotationSpeed = 1.0f;
constexpr float fElevationSpeed = 1.0f;

Player::Player(Coords<float> pos): pos(std::move(pos)) {}
Player::Player(const float x, const float y): pos(x, y) {}

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
}

void Player::move(const Player::Move dir, const float &fElapsedTime)
{
    switch (dir) {
        case Move::Forward: {
            pos.x += sinf(angle) * fMovementSpeed * fElapsedTime;
            pos.y += cosf(angle) * fMovementSpeed * fElapsedTime;
        } break;
        case Move::Backward: {
            pos.x -= sinf(angle) * fMovementSpeed * fElapsedTime;
            pos.y -= cosf(angle) * fMovementSpeed * fElapsedTime;
        } break;
        case Move::Left: {
            pos.x -= cosf(angle) * fMovementSpeed * fElapsedTime;
            pos.y += sinf(angle) * fMovementSpeed * fElapsedTime;
        } break;
        case Move::Right: {
            pos.x += cosf(angle) * fMovementSpeed * fElapsedTime;
            pos.y -= sinf(angle) * fMovementSpeed * fElapsedTime;
        } break;
    }
}

void Player::pan(const Player::Panning p, const float &m)
{
    switch (p) {
        case Panning::Up: elevation += (m * fElevationSpeed); break;
        case Panning::Down: elevation -= (m * fElevationSpeed); break;
    }
}

template <>
Coords<float> Player::getPlayerPos() const
{
    return pos;
};
