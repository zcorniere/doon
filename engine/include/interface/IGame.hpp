#pragma once

class IGame
{
public:
    ~IGame() = default;
    virtual void init() = 0;
    virtual void run() = 0;
};