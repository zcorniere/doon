#include "ObjectManager.hpp"
#include "Logger.hpp"
#include "interface/ICollision.hpp"
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
        if (std::is_convertible<ICollision *, decltype(s.get())>::value ||
            s->needRemove())
            continue;
        for (auto &i: qObjects) {
            if (s == i || i->needRemove() ||
                std::is_convertible<ICollision *, decltype(i.get())>::value)
                continue;
            Coords<float> fVec(s->getPosition() - i->getPosition());
            if (fVec.mag() <= fCollisionSize) {
                dynamic_cast<ICollision *>(i.get())->onCollision(s);
                dynamic_cast<ICollision *>(s.get())->onCollision(i);
            }
        }
    }
}
