#ifndef _POGGERS_HPP_
#define _POGGERS_HPP_

#include "abstract/ALife.hpp"
#include "interface/ICollision.hpp"
#include "interface/IObject.hpp"

constexpr short PoggerTimeout = 10;

class Poggers : public virtual IObject, public virtual ICollision, public virtual ALife
{
public:
    Poggers(Coords<float>);
    Poggers(float, float);
    ~Poggers();

    virtual const std::string &getTextureName() final { return sTexture; };
    virtual const Coords<float> &getPosition() final { return fPos; };
    virtual void update(float) final;
    virtual void onCollision(const std::unique_ptr<IObject> &other) final;

private:
    short timeout = PoggerTimeout;
    std::string sTexture = "pogger";
    Coords<float> fPos;
};

#endif    //_POGGERS_HPP_
