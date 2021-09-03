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
    constexpr const Vector<unsigned> &getSize() const { return size; }

    inline char &at(const Vector<unsigned> &idx)
    {
        return map.at(idx.y * size.x + idx.x);
    }
    inline const char &at(const Vector<unsigned> &idx) const
    {
        return map.at(idx.y * size.x + idx.x);
    }

    std::deque<Vector<unsigned>> getChars(const char c) const;
    bool isLocationSolid(const Vector<unsigned> &) const;

    const std::string &getTextureName(const char) const;

public:
    std::string map;
    Vector<unsigned> size;
};

std::ostream &operator<<(std::ostream &, const Map &);
