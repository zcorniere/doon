#pragma once

#include "Map.hpp"
#include "Vector.hpp"

struct Ray {
    float fFish = 0.0f;
    Vector<float> fOrigin;
    Vector<float> fDirection;
    float fDistance = 0.0f;
    Vector<float> fSample;
    char cHit;
    Vector<unsigned> uMap;

    void shoot(const Map &);
    void sample();
};
