#include "objects/Fireball.hpp"

Fireball::Fireball(Coords<float> pos, Coords<float> velocity)
    : AObject(pos, "fireball-0"), fVelocity(std::move(velocity))
{
}

Fireball::~Fireball() {}

Coords<float> Fireball::update(const float fElapsed)
{
    fLifespan -= fElapsed;
    if (fLifespan <= 0.0f) {
        this->setRemove(true);
        return fPosition;
    }
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
    return fPosition + (fVelocity * fElapsed);
}

void Fireball::onCollision(const std::unique_ptr<AObject> &) { this->setRemove(true); }

void Fireball::onSceneryCollision(const Map &, const Coords<float> &)
{
    fVelocity *= -1.0f;
}
