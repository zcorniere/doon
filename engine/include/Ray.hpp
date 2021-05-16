#pragma once

#include "Map.hpp"
#include "Vector.hpp"

class Ray
{
public:
    explicit constexpr Ray(const Vector<float> &fPos) noexcept: fOrigin(fPos), uMap(fPos)
    {
    }

    explicit constexpr Ray(const Vector<float> &fPos, const float fA) noexcept: Ray(fPos)
    {
        this->setRayDirection(fA);
    }

    constexpr void setRayDirection(const float fA) noexcept
    {
        fDirection.x = std::sin(fA);
        fDirection.y = std::cos(fA);
    }
    void shoot(const Map &);

public:
    Vector<float> fOrigin;
    Vector<float> fDirection;
    Vector<unsigned> uMap;
    float fDistance = 0.0f;
    char cHit;
    float fFish = 0.0f;
};
