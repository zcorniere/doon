#ifndef __BARREL_HPP__
#define __BARREL_HPP__

#include "abstract/AObject.hpp"

class Barrel : public virtual AObject
{
public:
    Barrel(Coords<float>);
    Barrel(float, float);
    ~Barrel();

    Coords<float> update(const float) final;
    void onCollision(const std::unique_ptr<AObject> &) final;
};

#endif    //__BARREL_HPP__
