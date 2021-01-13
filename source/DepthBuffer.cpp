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

const std::string array = " @";
const unsigned uSize = array.size();

std::ostream &operator<<(std::ostream &os, const DepthBuffer &d)
{
    for (unsigned i = 0; i < d.buf.size(); i += 25) {
        for (unsigned k = 0; k < d.buf.at(i).size(); k += 11) {
            os << array.at((uSize * unsigned(d.buf.at(i).at(k))) / 32);
        }
        os << std::endl;
    }
    return os;
}
