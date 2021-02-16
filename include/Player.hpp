#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "Coords.hpp"
#include "abstract/ALife.hpp"
#include "abstract/AObject.hpp"
#include "abstract/AShoot.hpp"
#include "interface/IMovement.hpp"
#include "objects/Fireball.hpp"

class Player : public virtual ALife,
               public virtual Movement::IMove,
               public virtual Movement::IRotate,
               public virtual AShoot<Fireball>,
               public virtual AObject
{
public:
    Player(Coords<float>);
    Player(const float, const float);
    ~Player();
    virtual void onSceneryCollision(const Map &, const Coords<float> &) final;
    virtual Coords<float> update(const float fElapsedTime) final;

    virtual void rotate(const Movement::Rotation, const float) final;
    virtual void move(const Movement::Move, const float) final;

    virtual std::unique_ptr<AObject> shoot() final;

public:
    Coords<float> fVelocity = 0;
    float fRotationVelocity = 0;
    float fSpeedModifier = 1;
    float fCooldown = 0.0f;
};

#endif    //_PLAYER_HPP_
