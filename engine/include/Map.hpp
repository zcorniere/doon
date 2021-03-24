#pragma once

#include "Vector.hpp"
#include <deque>
#include <filesystem>
#include <string>

class Map
{
public:
    Map();
    ~Map();
    Vector<unsigned> getSize() const;
    constexpr char &at(const Vector<unsigned> &idx)
    {
        return map.at(idx.y * width + idx.x);
    }
    constexpr const char &at(const Vector<unsigned> &idx) const
    {
        return map.at(idx.y * width + idx.x);
    }

    std::deque<Vector<unsigned>> getChars(const char c) const;
    bool isLocationSolid(const Vector<unsigned> &) const;

    const std::string &getTextureName(const char) const;

public:
    std::string map;
    unsigned width;
    unsigned height;
};

std::ostream &operator<<(std::ostream &, const Map &);
