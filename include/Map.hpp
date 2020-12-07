#ifndef _MAP_HPP_
#define _MAP_HPP_

#include "Coords.hpp"
#include <cmath>
#include <filesystem>
#include <string>
#include <vector>
#include <typeinfo>

class Map
{
public:
    Map(const std::filesystem::path);
    ~Map();
    char operator[](const std::size_t);
    char at(const std::size_t);
    char at(const Coords<unsigned> &);
    char at(const std::size_t) const;
    char at(const Coords<unsigned> &) const;

public:
    std::string map;
    unsigned width;
    unsigned height;
    Coords<unsigned> coord;
};

std::ostream &operator<<(std::ostream &, const Map &);

#endif    //_MAP_HPP_
