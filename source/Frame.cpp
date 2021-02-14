#include "Frame.hpp"
#include <cstring>

Frame::Frame() {}

Frame::Frame(const Coords<unsigned> &size) { this->create(size); }

Frame::Frame(const uint8_t *ptr, const Coords<unsigned> &uSize)
{
    this->create(uSize);
    if (ptr)
        std::memcpy(data.data(), ptr, uSize.x * uSize.y * 4);
    else {
        logger.err("FRAME") << "Invalid pixel ptr";
        logger.endl();
    }
}

Frame::~Frame() {}

void Frame::create(const Coords<unsigned> &nSize)
{
    size = nSize;
    data.clear();
    data.resize(size.x * size.y);
}

void Frame::setPixel(const Coords<unsigned> &pos, Pixel px)
{
    data.at(pos.y * size.x + pos.x) = std::move(px);
}

void Frame::setPixel(const unsigned &x, const unsigned &y, Pixel px)
{
    data.at(y * size.x + x) = std::move(px);
}

const uint8_t *Frame::getFramePtr() const { return (uint8_t *)data.data(); }

const Coords<unsigned> &Frame::getSize() const { return size; }

const Pixel &Frame::getPixel(const Coords<unsigned> &pos) const
{
    return data.at(pos.y * size.x + pos.x);
}

const Pixel &Frame::getPixel(const unsigned y, const unsigned x) const
{
    return data.at(y * size.x + x);
}
