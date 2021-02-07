#ifndef _FIREBALL_HPP_
#define _FIREBALL_HPP_

#include "abstract/AObject.hpp"

constexpr float uFireballLife = 10;

class Fireball : public virtual AObject
{
public:
    Fireball(Coords<float>, Coords<float>);
    ~Fireball();

    virtual void update(const float) final;
    virtual void onCollision(const std::unique_ptr<AObject> &) final;
    virtual void onSceneryCollision(const Map &) final;

private:
    float fLifespan = uFireballLife;
    unsigned loop = 0;
    Coords<float> fVelocity;
};

#endif    //_FIREBALL_HPP_
