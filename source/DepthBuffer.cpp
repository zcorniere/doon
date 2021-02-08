#include "DepthBuffer.hpp"

float &DepthBuffer::at(Coords<unsigned> &p) { return buf.at(p.x).at(p.y); }
float &DepthBuffer::at(unsigned x, unsigned y) { return buf.at(x).at(y); }
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

