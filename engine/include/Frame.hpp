#pragma once

#include "Pixel.hpp"
#include "Vector.hpp"
#include <cstdint>
#include <vector>

class Frame
{
public:
    explicit Frame();
    explicit Frame(const Vector<unsigned> &);
    explicit Frame(const uint8_t *, const Vector<unsigned> &);
    Frame(const Frame &) = default;
    ~Frame();

    void create(const Vector<unsigned> &, const Pixel = Color::Black);
    void rotate();

    constexpr void setPixel(const Vector<unsigned> &pos, Pixel px)
    {
        data.at(pos.y * size.x + pos.x) = std::move(px);
    }
    constexpr void setPixel(const unsigned &x, const unsigned &y, Pixel px)
    {
        setPixel({x, y}, px);
    }

    constexpr const uint8_t *getFramePtr() const noexcept
    {
        return (uint8_t *)data.data();
    }
    constexpr const Vector<unsigned> &getSize() const noexcept { return size; }
    constexpr const Pixel &getPixel(const Vector<unsigned> &pos) const
    {
        return data.at(pos.y * size.x + pos.x);
    }
    constexpr const Pixel &getPixel(const unsigned x, const unsigned y) const
    {
        return getPixel({x, y});
    }

private:
    Vector<unsigned> size;
    std::vector<Pixel> data;
};
