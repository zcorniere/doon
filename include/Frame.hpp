#ifndef _FRAME_HPP_
#define _FRAME_HPP_

#include "Coords.hpp"
#include "Pixel.hpp"
#include <cstdint>
#include <vector>

class Frame
{
public:
    explicit Frame();
    explicit Frame(const Coords<unsigned> &);
    explicit Frame(const uint8_t *, const Coords<unsigned> &);
    Frame(const Frame &) = default;
    ~Frame();

    void create(const Coords<unsigned> &);
    void rotate();
    inline void setPixel(const Coords<unsigned> &pos, Pixel px)
    {
        data.at(pos.y * size.x + pos.x) = std::move(px);
    }
    inline void setPixel(const unsigned &x, const unsigned &y, Pixel px)
    {
        data.at(y * size.x + x) = std::move(px);
    }

    inline const uint8_t *getFramePtr() const { return (uint8_t *)data.data(); }
    inline const Coords<unsigned> &getSize() const { return size; }
    inline const Pixel &getPixel(const Coords<unsigned> &pos) const
    {
        return data.at(pos.y * size.x + pos.x);
    }
    inline const Pixel &getPixel(const unsigned x, const unsigned y) const
    {
        return data.at(y * size.x + x);
    }

private:
    Coords<unsigned> size;
    std::vector<Pixel> data;
};

#endif    //_FRAME_HPP_
