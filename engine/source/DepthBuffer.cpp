#include "DepthBuffer.hpp"

DepthBuffer::DepthBuffer() {}

DepthBuffer::~DepthBuffer() {}

void DepthBuffer::resize(const Vector<unsigned> &p)
{
    buf.resize(p.x);
    std::for_each(buf.begin(), buf.end(), [p](auto &i) { i.resize(p.y); });
}

void DepthBuffer::resize(const unsigned x, const unsigned y)
{
    buf.resize(x);
    std::for_each(buf.begin(), buf.end(), [y](auto &i) { i.resize(y); });
}
