#include "objects/Poggers.hpp"

Poggers::Poggers(Coords<float> pos): AObject(std::move(pos), "pogger"), ALife(1)
{
    fSize = 0.75f;
}

Poggers::Poggers(float x, float y): Poggers(Coords(x, y)) {}

Poggers::~Poggers() {}

void Poggers::update(const float)
{
    if (timeout < PoggerTimeout) {
        timeout--;
        if (timeout <= 0) { this->setRemove(true); }
    }
}

void Poggers::onCollision(const std::unique_ptr<AObject> &)
{
    this->damage(1);
    if (!this->isAlive()) {
        sTexture = "explosion";
        timeout--;
    }
}
