#ifndef _ISHOOT_HPP_
#define _ISHOOT_HPP_

#include "ObjectManager.hpp"

class IShoot
{
public:
    virtual void shoot(ObjectManager &objs) = 0;
};

#endif    //_ISHOOT_HPP_
