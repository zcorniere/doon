#ifndef _ICOLLISION_HPP_
#define _ICOLLISION_HPP_

#include "interface/IObject.hpp"
#include <memory>

class ICollision
{
public:
    virtual void onCollision(const std::unique_ptr<IObject> &other) = 0;
};

#endif    //_ICOLLISION_HPP_
