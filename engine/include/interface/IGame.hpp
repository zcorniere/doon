#pragma once

#include "Coords.hpp"
#include "Frame.hpp"
#include "ObjectManager.hpp"
#include "ThreadPool.hpp"
#include "interface/IMap.hpp"

class IGame
{
public:
    IGame() = delete;
    IGame(const Coords<unsigned> &u, ThreadPool &p, ObjectManager &o)
        : uSize(u), pool(p), object(o){};
    ~IGame() = default;
    virtual void init() = 0;
    virtual void update(const float) = 0;
    virtual void drawToScreen(const uint8_t *const) = 0;
    virtual const Coords<unsigned> &getSize() const = 0;
    virtual bool isRunning() const = 0;
    virtual const IMap &getMap() const = 0;
    virtual const Frame &getTexture(const std::string &) const = 0;

protected:
    Coords<unsigned> uSize;
    ThreadPool &pool;
    ObjectManager &object;
};
