#include "objects/Fireball.hpp"

Fireball::Fireball(Coords<float> pos, Coords<float> velocity)
    : fPos(std::move(pos)), fVelocity(std::move(velocity))
{
}

Fireball::~Fireball() {}

void Fireball::update(float fElapsed)
{
    fPos += (fVelocity * fElapsed);
    if (loop++ == 5) {
        loop = 0;
        if (sTexture == "fireball-0") {
            sTexture = "fireball-1";
        } else if (sTexture == "fireball-1") {
            sTexture = "fireball-2";
        } else if (sTexture == "fireball-2") {
            sTexture = "fireball-3";
        } else if (sTexture == "fireball-3") {
            sTexture = "fireball-0";
        }
    }
}
