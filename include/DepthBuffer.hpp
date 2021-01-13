#ifndef _DEPTHBUFFER_HPP_
#define _DEPTHBUFFER_HPP_

#include "Coords.hpp"
#include <ostream>
#include <vector>

struct DepthBuffer {
    std::vector<std::vector<float>> buf;

    float &at(Coords<unsigned> &p);
    float &at(unsigned x, unsigned y);
    void resize(Coords<unsigned> &p);
    void resize(unsigned x, unsigned y);
};

std::ostream &operator<<(std::ostream &os, const DepthBuffer &d);
#endif    //_DEPTHBUFFER_HPP_
