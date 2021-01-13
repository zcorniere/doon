#ifndef _FIREBALL_HPP_
#define _FIREBALL_HPP_

#include "interface/IObject.hpp"

class Fireball : public virtual IObject
{
public:
    Fireball(Coords<float>, Coords<float>);
    ~Fireball();

    virtual const std::string &getTextureName() final { return sTexture; };
    virtual const Coords<float> &getPosition() final { return fPos; };
    virtual void update(float) final;

private:
    unsigned loop = 0;
    std::string sTexture = "fireball-0";
    Coords<float> fPos;
    Coords<float> fVelocity;
};

#endif    //_FIREBALL_HPP_
