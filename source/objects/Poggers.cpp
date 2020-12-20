#include "objects/Poggers.hpp"

Poggers::Poggers(Coords<float> &pos): fPos(pos) {}

Poggers::Poggers(float x, float y): fPos(x, y) {}

Poggers::~Poggers() {}

const bool &Poggers::isDrawable() { return bRemove; }

void Poggers::update() {}
