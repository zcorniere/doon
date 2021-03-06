#include "Map.hpp"
#include <fstream>
#include <streambuf>

const std::string sWallTexture = "redbrick";
const std::string sWoodTexture = "wood";
const std::string sFloorTexture = "greystone";

Map::Map() {}

Map::~Map() {}

std::deque<Vector<unsigned>> Map::getChars(const char c) const
{
    std::deque<Vector<unsigned>> ret;

    for (unsigned i = 0; i < map.size(); ++i) {
        if (map.at(i) == c) {
            unsigned uXpos = i % size.x;
            Vector<unsigned> pos(uXpos, (i - uXpos) / size.x);
            ret.push_back(std::move(pos));
        }
    }
    return ret;
}

bool Map::isLocationSolid(const Vector<unsigned> &idx) const
{
    auto c = this->at(idx);
    return c == '#' || c == 'w';
}

const std::string &Map::getTextureName(const char c) const
{
    switch (c) {
        case '#': return sWallTexture;
        case '.': return sFloorTexture;
        case 'w': return sWoodTexture;
        default: return sWallTexture;
    }
}

std::ostream &operator<<(std::ostream &os, const Map &other)
{
    for (unsigned i = 0; i < other.map.size(); i++) {
        if (i > 0 && i % other.size.x == 0) { os << std::endl; }
        os << other.map.at(i);
    }
    return os;
}
