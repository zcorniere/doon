#pragma once

#include "Map.hpp"
#include "Vector.hpp"

class Ray
{
public:
    explicit constexpr Ray(const Vector<float> &fPos): fOrigin(fPos), uMap(fPos) {}

    explicit constexpr Ray(const Vector<float> &fPos, const float fA): Ray(fPos)
    {
        this->setRayDirection(fA);
    }

    constexpr void setRayDirection(const float fA)
    {
        fDirection.x = std::sin(fA);
        fDirection.y = std::cos(fA);
    }
    void shoot(const Map &);
    void sample();

public:
    Vector<float> fOrigin;
    Vector<float> fDirection;
    Vector<unsigned> uMap;
    float fDistance = 0.0f;
    Vector<float> fSample;
    char cHit;
    float fFish = 0.0f;
};
