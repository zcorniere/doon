#ifndef _FIREBALL_HPP_
#define _FIREBALL_HPP_

#include "abstract/AObject.hpp"

constexpr const float fFireballLife = 10;
constexpr const float fAnimationLoop = 0.10;

class Fireball : public virtual AObject
{
public:
    Fireball(Coords<float>, Coords<float>);
    ~Fireball();

    Coords<float> update(const float) final;
    void onCollision(const std::unique_ptr<AObject> &) final;

private:
    float fLifespan = fFireballLife;
    float loop = fAnimationLoop;
    Coords<float> fVelocity;
};

#endif    //_FIREBALL_HPP_
