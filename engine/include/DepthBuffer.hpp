#ifndef _DEPTHBUFFER_HPP_
#define _DEPTHBUFFER_HPP_

#include "Coords.hpp"
#include <vector>

class DepthBuffer
{
public:
    DepthBuffer();
    ~DepthBuffer();
    inline auto &at(const unsigned &p) { return buf.at(p); }
    inline float &at(const Coords<unsigned> &p) { return buf.at(p.x).at(p.y); }
    inline float &at(const unsigned x, const unsigned y) { return buf.at(x).at(y); }
    void resize(const Coords<unsigned> &p);
    void resize(const unsigned x, const unsigned y);

private:
    std::vector<std::vector<float>> buf;
};

#endif    //_DEPTHBUFFER_HPP_
