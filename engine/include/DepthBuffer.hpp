#pragma once

#include "Logger.hpp"
#include "Vector.hpp"
#include <vector>

class DepthBuffer
{
public:
    DepthBuffer();
    ~DepthBuffer();
    inline auto &at(const unsigned &x) { return buf.at(x); }
    inline float &at(const Vector<unsigned> &p) { return buf.at(p.x).at(p.y); }
    inline float &at(const unsigned x, const unsigned y) { return buf.at(x).at(y); }
    inline void fill(const unsigned x, const float value)
    {
        std::fill(buf.at(x).begin(), buf.at(x).end(), value);
    }
    void resize(const Vector<unsigned> &p);
    void resize(const unsigned x, const unsigned y);

private:
    std::vector<std::vector<float>> buf;
};
