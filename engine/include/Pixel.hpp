#ifndef _COLOR_HPP_
#define _COLOR_HPP_

#include <SFML/Graphics/Color.hpp>
#include <cstdint>
#include <utility>

class Pixel
{
public:
    constexpr Pixel() = default;
    constexpr Pixel(const Pixel &) = default;
    constexpr Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
        : r(std::move(r)), g(std::move(g)), b(std::move(b)), a(std::move(a)){};
    constexpr Pixel(const sf::Color &col)
        : r(std::move(col.r)),
          g(std::move(col.g)),
          b(std::move(col.b)),
          a(std::move(col.a))
    {
    }
    constexpr void shade(const float fShade)
    {
        r *= fShade;
        g *= fShade;
        b *= fShade;
    }

public:
    uint8_t r = 255;
    uint8_t g = 255;
    uint8_t b = 255;
    uint8_t a = 255;
};
static_assert(sizeof(Pixel) == sizeof(uint32_t), "Pixel must be 32 bits");

namespace Color
{
static constexpr const Pixel Black(0, 0, 0);
static constexpr const Pixel White(255, 255, 255);
static constexpr const Pixel Blue(0, 0, 255);
static constexpr const Pixel LightBlue(0, 128, 255);
static constexpr const Pixel Green(0, 255, 0);
static constexpr const Pixel Red(255, 0, 0);
}    // namespace Color

#endif    //_COLOR_HPP_
