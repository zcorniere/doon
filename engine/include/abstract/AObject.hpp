#pragma once

#include "Vector.hpp"
#include <memory>
#include <optional>
#include <string>

class AObject
{
public:
    AObject(const Vector<float> pos, std::optional<std::string> text)
        : fPosition(std::move(pos)), sTexture(std::move(text))
    {
    }
    virtual ~AObject(){};
    virtual void onCollision(const std::unique_ptr<AObject> &){};
    virtual void onSceneryCollision(const Vector<float> &fSolved,
                                    Vector<float> &fPotential)
    {
        fPotential = fSolved;
    };

    virtual Vector<float> update(const float) = 0;

    std::optional<std::string> getTextureName() const noexcept { return sTexture; };
    constexpr void setRemove(bool vis) noexcept { bRemove = vis; };
    constexpr bool needRemove() const noexcept { return bRemove; };

    template <typename T = float>
    constexpr Vector<T> getPosition() const
    {
        return static_cast<Vector<T>>(fPosition);
    };
    template <typename T = float>
    constexpr T getAngle() const
    {
        return static_cast<T>(fAngle);
    };
    template <typename T = float>
    constexpr T getRadius() const
    {
        return static_cast<T>(fRadius);
    };

    template <typename T = float>
    constexpr void setAngle(T a) noexcept
    {
        fAngle = a;
    };
    template <typename T = float>
    constexpr void setPosition(Vector<T> a) noexcept
    {
        fPosition = a;
    };

protected:
    Vector<float> fPosition;
    float fAngle = 0.0f;
    float fRadius = 0.5f;

    std::optional<std::string> sTexture;
    bool bRemove = false;
};
