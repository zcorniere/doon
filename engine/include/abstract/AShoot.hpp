#ifndef _ASHOOT_HPP_
#define _ASHOOT_HPP_

#include <memory>
#include "Coords.hpp"
#include "abstract/AObject.hpp"
constexpr const float fBulletSpeed = 16.0f;

template <std::derived_from<AObject> Bullet>
class AShoot : public virtual AObject
{
public:
    virtual std::unique_ptr<AObject> shoot()
    {
        Coords<float> fObjV(std::sin(fAngle), std::cos(fAngle));
        return std::make_unique<Bullet>(fPosition + fObjV, fObjV * fBulletSpeed);
    };
};

#endif    //_ASHOOT_HPP_
