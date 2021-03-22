#include "Player.hpp"
#include "Logger.hpp"
#include "objects/Fireball.hpp"
#include <math.h>

constexpr const float fMovementSpeed = 10.0f;
constexpr const float fRotationSpeed = 1.0f;

constexpr const float fCooldownShoot = 0.5f;

Player::Player(Vector<float> fPosition)
    : ALife(5), AObject(fPosition, std::optional<std::string>())
{
}
Player::Player(const float x, const float y): Player(Vector(x, y)) {}

Player::~Player() {}

Vector<float> Player::update(const float fElapsedTime)
{
    Vector<float> nfPosition = fPosition + (fVelocity * fElapsedTime);
    fVelocity = {0, 0};
    if (fCooldown > 0.0f) fCooldown -= fElapsedTime;
    return nfPosition;
}

void Player::rotate(const Movement::Rotation dir, const float fElapsedTime)
{
    switch (dir) {
        case Movement::Clockwise: {
            fAngle += fRotationSpeed * fSpeedModifier * fElapsedTime;
        } break;
        case Movement::CounterClockwise: {
            fAngle -= fRotationSpeed * fSpeedModifier * fElapsedTime;
        } break;
    }
    if (fAngle == M_2_PI || fAngle == -M_2_PI) { fAngle = 0; }
}

void Player::move(const Movement::Move dir, const float)
{
    switch (dir) {
        case Movement::Forward: {
            fVelocity.x = sinf(fAngle) * fMovementSpeed * fSpeedModifier;
            fVelocity.y = cosf(fAngle) * fMovementSpeed * fSpeedModifier;
        } break;
        case Movement::Backward: {
            fVelocity.x = -sinf(fAngle) * fMovementSpeed * fSpeedModifier;
            fVelocity.y = -cosf(fAngle) * fMovementSpeed * fSpeedModifier;
        } break;
        case Movement::Left: {
            fVelocity.x = -cosf(fAngle) * fMovementSpeed * fSpeedModifier;
            fVelocity.y = sinf(fAngle) * fMovementSpeed * fSpeedModifier;
        } break;
        case Movement::Right: {
            fVelocity.x = cosf(fAngle) * fMovementSpeed * fSpeedModifier;
            fVelocity.y = -sinf(fAngle) * fMovementSpeed * fSpeedModifier;
        } break;
    }
}

void Player::shoot()
{
    if (fCooldown <= 0.0f) {
        this->AShoot::shoot();
        fCooldown = fCooldownShoot;
    } else {
        logger->info("PLAYER") << "Shoot in cooldown ! (" << fCooldown << ")";
        logger->endl();
    }
}
