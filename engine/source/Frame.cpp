#include "Frame.hpp"
#include "Logger.hpp"
#include <cstring>

Frame::Frame() {}

Frame::Frame(const Vector<unsigned> &size) { this->create(size); }

Frame::Frame(const uint8_t *ptr, const Vector<unsigned> &uSize)
{
    this->create(uSize);
    if (ptr)
        std::memcpy(data, ptr, uSize.x * uSize.y * 4);
    else {
        logger->err("FRAME") << "Invalid pixel ptr";
    }
}

Frame::Frame(const Frame &f)
{
    this->create(f.getSize());
    std::memcpy(data, f.data, size.x * size.y * 4);
}

Frame::~Frame()
{
    if (data != nullptr) delete data;
}

void Frame::create(const Vector<unsigned> &nSize, const Pixel pFill)
{
    size = nSize;
    if (data != nullptr) delete data;
    data = new Pixel[(size.x * size.y)];
    std::fill(data, data + (size.x * size.y), pFill);
}

void Frame::rotate()
{
    if (size.x != size.y) {
        logger->warn("FRAME") << "Can't rotate a non-square Frame";

        return;
    }
    for (unsigned x = 0; x < size.x; ++x) {
        for (unsigned y = 0; y < size.y; ++y) {
            std::swap(data[size.y * y + x], data[size.x * x + y]);
        }
    }
}
