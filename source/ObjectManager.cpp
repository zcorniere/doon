#include "ObjectManager.hpp"
#include "Logger.hpp"
#include <execution>
#include <functional>

constexpr float fCollisionSize = 1.0f;

ObjectManager::ObjectManager(ThreadPool &t): pool(t) {}

ObjectManager::~ObjectManager() {}

void ObjectManager::update(float fElapsedTime)
{
    std::deque<std::future<void>> fut;

    for (auto &i: qObjects) {
        fut.push_back(pool.push(
            [&i](int, float fElapsedTime) { i->update(fElapsedTime); }, fElapsedTime));
    }
    std::for_each(std::execution::par, fut.begin(), fut.end(), [](auto &i) { i.wait(); });
    std::erase_if(qObjects, [](auto &i) { return i->needRemove(); });
    this->computeCollision();
}

void ObjectManager::computeCollision()
{
    std::deque<std::future<void>> fut;

    for (auto &s: qObjects) {
        for (auto &i: qObjects) {
            if (s == i || s->needRemove() || i->needRemove()) continue;
            Coords<float> fVec(s->getPosition() - i->getPosition());
            if (fVec.mag() <= fCollisionSize) {
                s->setRemove(true);
                i->setRemove(true);
            }
        }
    }
}
