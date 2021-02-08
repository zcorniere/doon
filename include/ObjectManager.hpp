#ifndef _OBJECTMANAGER_HPP_
#define _OBJECTMANAGER_HPP_

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

    template <typename T>
    void addObject(std::unique_ptr<T> obj)
    {
        static_assert(std::is_base_of_v<AObject, T>,
                      "Object must be derived from AObject interface");
        qObjects.push_back(std::move(obj));
    }
    void update(const float fElapsedTime);

private:
    void computeCollision();

private:
    ThreadPool &pool;
    std::deque<std::unique_ptr<AObject>> qObjects;
};

#endif    //_OBJECTMANAGER_HPP_
