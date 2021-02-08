#include "Player.hpp"
#include "Logger.hpp"
#include "objects/Fireball.hpp"
#include <math.h>

constexpr float fMovementSpeed = 10.0f;
constexpr float fRotationSpeed = 1.0f;

constexpr float fCooldownShoot = 0.5f;

Player::Player(Coords<float> fPosition)
    : ALife(5), AObject(fPosition, std::optional<std::string>("pogger"))
{
}
Player::Player(const float x, const float y): Player(Coords(x, y)) {}

Player::~Player() {}

void Player::update(const float fElapsedTime)
{
    fPosition += (fVelocity * fElapsedTime);
    fVelocity.x = 0;
    fVelocity.y = 0;
    if (fCooldown > 0.0f) fCooldown -= fElapsedTime;
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

void Player::shoot(ObjectManager &obj)
{
    if (fCooldown <= 0.0f) {
        Coords<float> fObjV(std::sin(fAngle), std::cos(fAngle));
        obj.addObject(std::make_unique<Fireball>(fPosition + fObjV, fObjV * 16.0f));
        fCooldown = fCooldownShoot;
    } else {
        logger.info("PLAYER") << "Shoot in cooldown ! (" << fCooldown << ")";
        logger.endl();
    }
}
