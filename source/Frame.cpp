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
