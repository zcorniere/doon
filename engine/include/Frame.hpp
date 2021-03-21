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

    inline void setPixel(const Vector<unsigned> &pos, Pixel px)
    {
        data.at(pos.y * size.x + pos.x) = std::move(px);
    }
    inline void setPixel(const unsigned &x, const unsigned &y, Pixel px)
    {
        data.at(y * size.x + x) = std::move(px);
    }

    inline const uint8_t *getFramePtr() const { return (uint8_t *)data.data(); }
    inline const Vector<unsigned> &getSize() const { return size; }
    inline const Pixel &getPixel(const Vector<unsigned> &pos) const
    {
        return data.at(pos.y * size.x + pos.x);
    }
    inline const Pixel &getPixel(const unsigned x, const unsigned y) const
    {
        return data.at(y * size.x + x);
    }

private:
    Vector<unsigned> size;
    std::vector<Pixel> data;
};
