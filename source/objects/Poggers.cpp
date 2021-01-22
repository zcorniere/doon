#include "objects/Poggers.hpp"

Poggers::Poggers(Coords<float> pos): fPos(std::move(pos)) {}

Poggers::Poggers(float x, float y): fPos(x, y) {}

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
    sTexture = "explosion";
    timeout--;
}
