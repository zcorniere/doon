#ifndef _IOBJECT_HPP_
#define _IOBJECT_HPP_

#include "Coords.hpp"
#include <string>

class IObject
{
public:
    virtual ~IObject(){};
    virtual const std::string &getTextureName() = 0;
    virtual const Coords<float> &getPosition() = 0;
    virtual void update(float) = 0;

    void setRemove(bool vis) { bRemove = vis; };
    bool needRemove() { return bRemove; };

protected:
    bool bRemove = false;
};

#endif    //_IOBJECT_HPP_
