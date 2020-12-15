#ifndef _MAP_HPP_
#define _MAP_HPP_

#include "Coords.hpp"
#include <filesystem>
#include <string>

class Map
{
public:
    Map(const std::filesystem::path);
    ~Map();
    Coords<unsigned> getSize() const;
    inline char operator[](const std::size_t idx) { return map[idx]; }
    inline char at(const std::size_t idx) { return map.at(idx); }
    inline char at(const Coords<unsigned> &idx) { return map.at(idx.y * width + idx.x); }

    inline char operator[](const std::size_t idx) const { return map[idx]; }
    inline char at(const std::size_t idx) const { return map.at(idx); }
    inline char at(const Coords<unsigned> &idx) const
    {
        return map.at(idx.y * width + idx.x);
    }

public:
    std::string map;
    unsigned width;
    unsigned height;
};

std::ostream &operator<<(std::ostream &, const Map &);

#endif    //_MAP_HPP_
