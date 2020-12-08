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
    char operator[](const std::size_t);
    char at(const std::size_t);
    char at(const Coords<unsigned> &);

    char operator[](const std::size_t) const;
    char at(const std::size_t) const;
    char at(const Coords<unsigned> &) const;

public:
    std::string map;
    unsigned width;
    unsigned height;
};

std::ostream &operator<<(std::ostream &, const Map &);

#endif    //_MAP_HPP_
