#pragma once

#include "Map.hpp"
#include "abstract/AObject.hpp"
#include <deque>
#include <memory>

class ObjectManager
{
public:
    ObjectManager();
    ~ObjectManager();
    ObjectManager(ObjectManager &) = delete;

    constexpr auto &getObjects() const { return qObjects; }
    constexpr auto &at(const unsigned i) const { return qObjects.at(i); }
    constexpr auto &at(const unsigned i) { return qObjects.at(i); }

    template <std::derived_from<AObject> T>
    constexpr void addObject(std::unique_ptr<T> obj)
    {
        qObjects.push_back(std::move(obj));
    }
    void update(const Map &, const float);
    void computeCollision(std::unique_ptr<AObject> &);

private:
    Vector<float> resolveWallCollision(const Map &, std::unique_ptr<AObject> &,
                                       const Vector<float> &) const;

private:
    std::deque<std::unique_ptr<AObject>> qObjects;
};

extern ObjectManager *object_manager;
