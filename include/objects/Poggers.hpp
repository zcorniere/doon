#ifndef _POGGERS_HPP_
#define _POGGERS_HPP_

#include "interface/IObject.hpp"

class Poggers : public virtual IObject
{
public:
    Poggers(Coords<float>);
    Poggers(float, float);
    ~Poggers();

    virtual const std::string &getTextureName() final { return sTexture; };
    virtual const Coords<float> &getPosition() final { return fPos; };
    virtual void update(float) final;
    virtual void setRemove(bool vis) final;

private:
    std::string sTexture = "pogger";
    Coords<float> fPos;
};

#endif    //_POGGERS_HPP_
