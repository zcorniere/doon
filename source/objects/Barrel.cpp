#include "objects/Barrel.hpp"

Barrel::Barrel(Coords<float> pos): AObject(std::move(pos), "barrel") { fSize = 0.25f; }

Barrel::Barrel(float x, float y): Barrel(Coords(x, y)) {}

Barrel::~Barrel() {}

Coords<float> Barrel::update(const float) { return fPosition; }
