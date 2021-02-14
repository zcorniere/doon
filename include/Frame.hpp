#ifndef _FRAME_HPP_
#define _FRAME_HPP_

#include "Coords.hpp"
#include "Logger.hpp"
#include "Pixel.hpp"
#include <cstdint>
#include <vector>

class Frame
{
public:
    Frame();
    Frame(const Coords<unsigned> &);
    Frame(const uint8_t *, const Coords<unsigned> &);
    ~Frame();

    void create(const Coords<unsigned> &);
    void setPixel(const Coords<unsigned> &, Pixel);
    void setPixel(const unsigned &, const unsigned &, Pixel);

    const uint8_t *getFramePtr() const;
    const Coords<unsigned> &getSize() const;
    const Pixel &getPixel(const Coords<unsigned> &) const;
    const Pixel &getPixel(const unsigned, const unsigned) const;

private:
    Coords<unsigned> size;
    std::vector<Pixel> data;
};

#endif    //_FRAME_HPP_
