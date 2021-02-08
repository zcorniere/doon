#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "Coords.hpp"
#include "abstract/ALife.hpp"
#include "abstract/AObject.hpp"
#include "interface/IMovement.hpp"
#include "interface/IShoot.hpp"

class Player : public virtual IShoot,
               public virtual ALife,
               public virtual Movement::IMove,
               public virtual Movement::IRotate,
               public virtual AObject
{
public:
    Player(Coords<float>);
    Player(const float, const float);
    ~Player();
    virtual void update(const float fElapsedTime) final;

    virtual void rotate(const Movement::Rotation, const float) final;
    virtual void move(const Movement::Move, const float) final;

    virtual void shoot(ObjectManager &) final;

public:
    Coords<float> fVelocity = 0;
    float fRotationVelocity = 0;
    float fSpeedModifier = 1;
    float fCooldown = 0.0f;
};

#endif    //_PLAYER_HPP_
