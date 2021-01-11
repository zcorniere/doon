#include "ObjectManager.hpp"
#include "Logger.hpp"
#include <execution>

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
}
