#pragma once

#include "Map.hpp"
#include "ThreadPool.hpp"
#include "abstract/AObject.hpp"
#include <deque>
#include <memory>

class ObjectManager
{
public:
    ObjectManager(ThreadPool &);
    ~ObjectManager();

    inline auto &getObjects() const { return qObjects; }
    inline auto &at(const unsigned i) const { return qObjects.at(i); }
    inline auto &at(const unsigned i) { return qObjects.at(i); }

    template <std::derived_from<AObject> T>
    void addObject(std::unique_ptr<T> obj)
    {
        qObjects.push_back(std::move(obj));
    }
    void update(const Map &, const float);
    void computeCollision(std::unique_ptr<AObject> &);

private:
    Coords<float> resolveWallCollision(const Map &, std::unique_ptr<AObject> &,
                                       const Coords<float> &) const;

private:
    ThreadPool &pool;

    std::deque<std::unique_ptr<AObject>> qObjects;
};
