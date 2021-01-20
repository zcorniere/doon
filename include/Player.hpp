#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "Coords.hpp"
#include "interface/IMovement.hpp"
#include "interface/IShoot.hpp"

class Player : public virtual IShoot,
               public virtual Movement::IMove,
               public virtual Movement::IRotate,
               public virtual Movement::IPan
{
public:
    Player(Coords<float>);
    Player(const float, const float);
    ~Player();

    virtual void rotate(const Movement::Rotation, const float &) final;
    virtual void move(const Movement::Move, const float &) final;
    virtual void pan(const Movement::Panning, const float &) final;

    virtual void shoot(ObjectManager &) final;

    template <typename T>
    inline Coords<T> getPlayerPos() const
    {
        return static_cast<Coords<T>>(pos);
    };

public:
    Coords<float> pos;
    float elevation = 1.0f;
    float angle = 0.0f;
    float fSpeedModifier = 1;
};

#endif    //_PLAYER_HPP_
