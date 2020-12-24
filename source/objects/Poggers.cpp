#include "objects/Poggers.hpp"

Poggers::Poggers(Coords<float> pos): fPos(std::move(pos)) {}

Poggers::Poggers(float x, float y): fPos(x, y) {}

Poggers::~Poggers() {}

void Poggers::update() {}
