#include "DepthBuffer.hpp"

void DepthBuffer::resize(Coords<unsigned> &p)
{
    buf.resize(p.x);
    std::for_each(buf.begin(), buf.end(), [p](auto &i) { i.resize(p.y); });
}
void DepthBuffer::resize(unsigned x, unsigned y)
{
    buf.resize(x);
    std::for_each(buf.begin(), buf.end(), [y](auto &i) { i.resize(y); });
}
