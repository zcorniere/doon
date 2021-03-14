#ifndef _MAP_HPP_
#define _MAP_HPP_

#include "Coords.hpp"
#include "interface/IMap.hpp"
#include <deque>
#include <filesystem>
#include <string>

class Map : public IMap
{
public:
    Map(const std::filesystem::path);
    ~Map();
    Coords<unsigned> getSize() const;
    inline char at(const Coords<unsigned> &idx) final
    {
        return map.at(idx.y * width + idx.x);
    }
    inline char at(const Coords<unsigned> &idx) const final
    {
        return map.at(idx.y * width + idx.x);
    }

    std::deque<Coords<unsigned>> getChars(const char c) const;
    bool isLocationSolid(const Coords<unsigned> &) const final;

    const std::string &getTextureName(const char) const final;

public:
    std::string map;
    unsigned width;
    unsigned height;
};

std::ostream &operator<<(std::ostream &, const Map &);

#endif    //_MAP_HPP_
