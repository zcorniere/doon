#include "objects/Barrel.hpp"

Barrel::Barrel(Vector<float> pos): AObject(std::move(pos), "barrel") { fRadius = 0.25f; }

Vector<float> Barrel::update(const float) { return fPosition; }
