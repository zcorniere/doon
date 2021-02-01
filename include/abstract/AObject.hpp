#ifndef _AOBJECT_HPP_
#define _AOBJECT_HPP_

#include "Coords.hpp"
#include <optional>
#include <string>

class AObject
{
public:
    AObject(const Coords<float> pos, std::optional<std::string> text)
        : fPosition(std::move(pos)), sTexture(std::move(text))
    {
    }
    virtual ~AObject(){};
    virtual std::optional<std::string> getTextureName() { return sTexture; };
    virtual void setRemove(bool vis) { bRemove = vis; };
    virtual bool needRemove() { return bRemove; };

    virtual void update(float) = 0;

    const Coords<float> &getPosition() { return fPosition; };

protected:
    Coords<float> fPosition;
    float fAngle = 0.0f;

    std::optional<std::string> sTexture;
    bool bRemove = false;
};

#endif    //_AOBJECT_HPP_
