#ifndef _ADESTRUCTIBLE_HPP_
#define _ADESTRUCTIBLE_HPP_

#include "interface/IObject.hpp"
#include <memory>

class ADestructible
{
public:
    virtual void kill() { isAlive = false; };
    virtual void revive() { isAlive = true; };
    virtual void onCollision(std::unique_ptr<IObject> &obj)
    {
        (void)obj;
        this->kill();
    }

private:
    bool isAlive = true;
};

#endif    //_ADESTRUCTIBLE_HPP_
