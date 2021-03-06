#include "objects/Fireball.hpp"
#include "objects/Poggers.hpp"

Fireball::Fireball(Vector<float> pos, Vector<float> velocity)
    : AObject(pos, "fireball-0"), fVelocity(std::move(velocity))
{
    fRadius = 0.8f;
}

Fireball::~Fireball() {}

Vector<float> Fireball::update(const float fElapsed)
{
    fLifespan -= fElapsed;
    loop -= fElapsed;
    if (fLifespan <= 0.0f) {
        this->setRemove(true);
        return fPosition;
    }
    if (loop <= 0.0f) {
        loop = fAnimationLoop;
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

void Fireball::onCollision(const std::unique_ptr<AObject> &other)
{
    if (dynamic_cast<Poggers *>(other.get())) this->setRemove(true);
}
