#include "objects/Fireball.hpp"

Fireball::Fireball(Coords<float> pos, Coords<float> velocity)
    : fPos(std::move(pos)), fVelocity(std::move(velocity))
{
}

Fireball::~Fireball() {}

void Fireball::update() { fPos += fVelocity; }
