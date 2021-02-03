#ifndef _AOBJECT_HPP_
#define _AOBJECT_HPP_

#include "Coords.hpp"
#include <memory>
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
    virtual void onCollision(const std::unique_ptr<AObject> &){};
    virtual void onSceneryCollision() { this->setRemove(true); };

    virtual void update(const float) = 0;

    inline std::optional<std::string> getTextureName()const { return sTexture; };
    inline void setRemove(bool vis) { bRemove = vis; };
    inline bool needRemove()const { return bRemove; };
    template <typename T = float>
    Coords<T> inline getPosition() const
    {
        return static_cast<Coords<T>>(fPosition);
    };
    template <typename T = float>
    T inline getAngle() const
    {
        return static_cast<T>(fAngle);
    };

protected:
    Coords<float> fPosition;
    float fAngle = 0.0f;

    std::optional<std::string> sTexture;
    bool bRemove = false;
};

#endif    //_AOBJECT_HPP_
