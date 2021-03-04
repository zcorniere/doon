#ifndef _MAP_HPP_
#define _MAP_HPP_

#include "Coords.hpp"
#include <deque>
#include <filesystem>
#include <string>

class Map
{
public:
    Map(const std::filesystem::path);
    Map(const Map &) = default;
    ~Map();
    Coords<unsigned> getSize() const;
    inline char operator[](const std::size_t idx) const { return map[idx]; }
    inline char at(const std::size_t idx) const { return map.at(idx); }
    std::deque<Coords<unsigned>> getChars(const char c) const;
    char at(const Coords<unsigned> &idx) const;

public:
    std::string map;
    unsigned width;
    unsigned height;
};

std::ostream &operator<<(std::ostream &, const Map &);

#endif    //_MAP_HPP_
