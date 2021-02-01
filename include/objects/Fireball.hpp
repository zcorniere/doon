#ifndef _FIREBALL_HPP_
#define _FIREBALL_HPP_

#include "abstract/AObject.hpp"
#include "interface/ICollision.hpp"

class Fireball : public virtual AObject, public virtual ICollision
{
public:
    Fireball(Coords<float>, Coords<float>);
    ~Fireball();

    virtual void update(float) final;
    virtual void onCollision(const std::unique_ptr<AObject> &) final;

private:
    unsigned loop = 0;
    Coords<float> fVelocity;
};

#endif    //_FIREBALL_HPP_
