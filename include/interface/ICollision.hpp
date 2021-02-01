#ifndef _ICOLLISION_HPP_
#define _ICOLLISION_HPP_

#include "abstract/AObject.hpp"
#include <memory>

class ICollision
{
public:
    virtual void onCollision(const std::unique_ptr<AObject> &other) = 0;
};

#endif    //_ICOLLISION_HPP_
