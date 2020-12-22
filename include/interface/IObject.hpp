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
    virtual void update() = 0;

    virtual bool isDrawable() = 0;

    void setVisible(bool vis) { bVisible = vis; };
    bool isVisible() { return bVisible; };

protected:
    bool bVisible = true;
};

#endif    //_IOBJECT_HPP_
