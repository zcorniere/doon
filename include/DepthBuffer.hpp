#ifndef _DEPTHBUFFER_HPP_
#define _DEPTHBUFFER_HPP_

#include "Coords.hpp"
#include <vector>

struct DepthBuffer {
    std::vector<std::vector<float>> buf;

    inline float &at(Coords<unsigned> &p) { return buf.at(p.x).at(p.y); }
    inline float &at(unsigned x, unsigned y) { return buf.at(x).at(y); }
    void resize(const Coords<unsigned> &p);
    void resize(const unsigned x, const unsigned y);
};

#endif    //_DEPTHBUFFER_HPP_
