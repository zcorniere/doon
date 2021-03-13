#pragma once

#include "Coords.hpp"
#include "Map.hpp"
#include "ObjectManager.hpp"
#include "Storage.hpp"
#include "ThreadPool.hpp"

class IGame
{
public:
    IGame() = delete;
    IGame(const Coords<unsigned> &u, Storage &s, ThreadPool &p, ObjectManager &o)
        : uSize(u), storage(s), pool(p), object(o){};
    ~IGame() = default;
    virtual void init() = 0;
    virtual void update(const float) = 0;
    virtual void drawToScreen(const uint8_t *const) = 0;
    virtual const Coords<unsigned> &getSize() const = 0;
    virtual bool isRunning() const = 0;
    virtual const std::string &getMapName() const = 0;

protected:
    Coords<unsigned> uSize;
    Storage &storage;
    ThreadPool &pool;
    ObjectManager &object;
};
