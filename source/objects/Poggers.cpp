#include "objects/Poggers.hpp"

Poggers::Poggers(Coords<float> pos): ALife(1), fPos(std::move(pos)) {}

Poggers::Poggers(float x, float y): Poggers(Coords(x, y)) {}

Poggers::~Poggers() {}

void Poggers::update(float)
{
    if (timeout < PoggerTimeout) {
        timeout--;
        if (timeout <= 0) { this->IObject::setRemove(true); }
    }
}

void Poggers::onCollision(const std::unique_ptr<IObject> &)
{
    this->damage(1);
    if (!this->isAlive()) {
        sTexture = "explosion";
        timeout--;
    }
}
