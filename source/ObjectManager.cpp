#include "ObjectManager.hpp"
#include "Logger.hpp"
#include <execution>
#include <functional>

ObjectManager::ObjectManager(ThreadPool &p, const Map &m): map(m), pool(p) {}

ObjectManager::~ObjectManager() {}

void ObjectManager::update(float fElapsedTime)
{
    std::deque<std::future<void>> fut;

    for (auto &i: qObjects) {
        fut.push_back(pool.push(
            [&i, this](int, float fElapsedTime) {
                Coords<float> next_coord(i->update(fElapsedTime));
                if (map.at(next_coord) == '#') {
                    i->onSceneryCollision(map, next_coord);
                } else {
                    i->setPosistion(next_coord);
                }
            },
            fElapsedTime));
    }

    std::for_each(std::execution::par, fut.begin(), fut.end(), [](auto &i) { i.wait(); });
    this->computeCollision();
    std::erase_if(qObjects, [](auto &i) { return i->needRemove(); });
}

void ObjectManager::computeCollision()
{
    for (auto &s: qObjects) {
        if (s->needRemove()) continue;
        for (auto &i: qObjects) {
            if (s == i || i->needRemove()) continue;
            Coords<float> fVec(s->getPosition<float>() - i->getPosition<float>());
            if (std::abs(fVec.mag()) <= i->getSize()) {
                i->onCollision(s);
                s->onCollision(i);
            }
        }
    }
}
