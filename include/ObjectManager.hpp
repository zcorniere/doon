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

    auto &getObjects() { return qObjects; }

    template <typename T>
    void addObject(std::unique_ptr<T> &obj)
    {
        static_assert(std::is_convertible<AObject, T>::value,
                      "Object must be derived from IObject interface");
        qObjects.push_back(std::move(obj));
    }
    void update(float fElapsedTime);

private:
    void computeCollision();

private:
    ThreadPool &pool;
    std::deque<std::unique_ptr<AObject>> qObjects;
};

#endif    //_OBJECTMANAGER_HPP_
