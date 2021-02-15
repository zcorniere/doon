#ifndef _ASHOOT_HPP_
#define _ASHOOT_HPP_

#include "ObjectManager.hpp"

constexpr float fBulletSpeed = 16.0f;

template <class Bullet>
class AShoot : public virtual AObject
{
    static_assert(std::is_base_of_v<AObject, Bullet>, "Bullets must be an AObject");

public:
    virtual std::unique_ptr<AObject> shoot()
    {
        Coords<float> fObjV(std::sin(fAngle), std::cos(fAngle));
        return std::make_unique<Bullet>(fPosition + fObjV, fObjV * fBulletSpeed);
    };
};

#endif    //_ASHOOT_HPP_
