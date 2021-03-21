#ifndef _ASHOOT_HPP_
#define _ASHOOT_HPP_

#include "ObjectManager.hpp"
#include "Vector.hpp"
#include "abstract/AObject.hpp"
#include <memory>
constexpr const float fBulletSpeed = 16.0f;

template <std::derived_from<AObject> Bullet>
class AShoot : public virtual AObject
{
public:
    virtual void shoot()
    {
        Vector<float> fObjV(std::sin(fAngle), std::cos(fAngle));
        object_manager->addObject(
            std::make_unique<Bullet>(fPosition + fObjV, fObjV * fBulletSpeed));
    };
};

#endif    //_ASHOOT_HPP_
