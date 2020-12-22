#ifndef _POGGERS_HPP_
#define _POGGERS_HPP_

#include "interface/IObject.hpp"

class Poggers : public IObject
{
public:
    Poggers(Coords<float> &);
    Poggers(float, float);
    ~Poggers();
    virtual const std::string &getTextureName() final { return sTexture; };
    virtual const Coords<float> &getPosition() final { return fPos; };
    virtual bool isDrawable() final;
    virtual void update() final;

private:
    const std::string sTexture = "pogger";
    Coords<float> fPos;
    bool bRemove = false;
};
#endif    //_POGGERS_HPP_
