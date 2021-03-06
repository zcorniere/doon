#ifndef _POGGERS_HPP_
#define _POGGERS_HPP_

#include "abstract/ALife.hpp"
#include "abstract/AObject.hpp"

constexpr const short PoggerTimeout = 10;

class Poggers : public virtual AObject, public virtual ALife
{
public:
    Poggers(Vector<float>);

    virtual Vector<float> update(const float) final;
    virtual void onCollision(const std::unique_ptr<AObject> &other) final;

private:
    short timeout = PoggerTimeout;
};

#endif    //_POGGERS_HPP_
