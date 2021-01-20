#include "Player.hpp"
#include "objects/Fireball.hpp"
#include <math.h>

constexpr float fMovementSpeed = 10.0f;
constexpr float fRotationSpeed = 1.0f;
constexpr float fElevationSpeed = 1.0f;

Player::Player(Coords<float> pos): pos(std::move(pos)) {}
Player::Player(const float x, const float y): pos(x, y) {}

Player::~Player() {}

void Player::rotate(const Movement::Rotation dir, const float &fElapsedTime)
{
    switch (dir) {
        case Movement::Clockwise: {
            angle += fRotationSpeed * fElapsedTime;
        } break;
        case Movement::CounterClockwise: {
            angle -= fRotationSpeed * fElapsedTime;
        } break;
    }
    if (angle == M_2_PI || angle == -M_2_PI) { angle = 0; }
}

void Player::move(const Movement::Move dir, const float &fElapsedTime)
{
    switch (dir) {
        case Movement::Forward: {
            pos.x += sinf(angle) * fMovementSpeed * fSpeedModifier * fElapsedTime;
            pos.y += cosf(angle) * fMovementSpeed * fSpeedModifier * fElapsedTime;
        } break;
        case Movement::Backward: {
            pos.x -= sinf(angle) * fMovementSpeed * fSpeedModifier * fElapsedTime;
            pos.y -= cosf(angle) * fMovementSpeed * fSpeedModifier * fElapsedTime;
        } break;
        case Movement::Left: {
            pos.x -= cosf(angle) * fMovementSpeed * fSpeedModifier * fElapsedTime;
            pos.y += sinf(angle) * fMovementSpeed * fSpeedModifier * fElapsedTime;
        } break;
        case Movement::Right: {
            pos.x += cosf(angle) * fMovementSpeed * fSpeedModifier * fElapsedTime;
            pos.y -= sinf(angle) * fMovementSpeed * fSpeedModifier * fElapsedTime;
        } break;
    }
}

void Player::pan(const Movement::Panning p, const float &m)
{
    switch (p) {
        case Movement::Up: elevation += (m * fElevationSpeed); break;
        case Movement::Down: elevation -= (m * fElevationSpeed); break;
    }
}

void Player::shoot(ObjectManager &obj)
{
    float fNoise = (((float)rand() / (float)RAND_MAX) - 0.5f) * 0.1f;
    float fVelocity = angle + fNoise;
    Coords<float> fObjV(std::sin(fVelocity) * 16.0f, std::cos(fVelocity) * 16.0f);
    obj.getObjects().push_back(std::make_unique<Fireball>(pos, std::move(fObjV)));
}

template <>
Coords<float> Player::getPlayerPos() const
{
    return pos;
};
