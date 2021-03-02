#ifndef _OBJECTMANAGER_HPP_
#define _OBJECTMANAGER_HPP_

#include "Map.hpp"
#include "ThreadPool.hpp"
#include "abstract/AObject.hpp"
#include <deque>
#include <memory>

class ObjectManager
{
public:
    ObjectManager(ThreadPool &, const Map &);
    ~ObjectManager();

    inline auto &getObjects() const { return qObjects; }
    inline auto &at(const unsigned i) const { return qObjects.at(i); }
    inline auto &at(const unsigned i) { return qObjects.at(i); }

    template <std::derived_from<AObject> T>
    void addObject(std::unique_ptr<T> obj)
    {
        qObjects.push_back(std::move(obj));
    }
    void update(const float fElapsedTime);
    void computeCollision();

private:
    Coords<float> resolveWallCollision(std::unique_ptr<AObject> &,
                                       const Coords<float> &) const;

private:
    const Map &map;
    ThreadPool &pool;

    std::deque<std::unique_ptr<AObject>> qObjects;
};

#endif    //_OBJECTMANAGER_HPP_
