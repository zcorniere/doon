#pragma once

#include "Coords.hpp"
#include <string>

class IMap
{
public:
    virtual ~IMap() = default;
    virtual char at(const Coords<unsigned> &) = 0;
    virtual char at(const Coords<unsigned> &) const = 0;
    virtual bool isLocationSolid(const Coords<unsigned> &) const = 0;
    virtual const std::string &getTextureName(const char) const = 0;
};