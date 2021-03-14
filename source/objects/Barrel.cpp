#include "objects/Barrel.hpp"

Barrel::Barrel(Coords<float> pos): AObject(std::move(pos), "barrel") { fSize = 0.25f; }

Coords<float> Barrel::update(const float) { return fPosition; }
