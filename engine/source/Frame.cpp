#include "Frame.hpp"
#include "Logger.hpp"
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

void Frame::create(const Coords<unsigned> &nSize, const Pixel pFill)
{
    size = nSize;
    data.clear();
    data.resize(size.x * size.y, pFill);
}

void Frame::rotate()
{
    if (size.x != size.y) {
        logger.warn("FRAME") << "Can't rotate a non-square Frame";
        logger.endl();
        return;
    }
    for (unsigned x = 0; x < size.x; ++x) {
        for (unsigned y = 0; y < size.y; ++y) {
            std::swap(data[size.y * y + x], data[size.x * x + y]);
        }
    }
}
