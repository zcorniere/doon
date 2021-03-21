#include "Map.hpp"
#include <fstream>
#include <streambuf>

const std::string sWallTexture = "redbrick";
const std::string sWoodTexture = "wood";
const std::string sFloorTexture = "greystone";

Map::Map(const std::filesystem::path path)
{
    std::ifstream file(path);

    if (!file.is_open()) { throw std::runtime_error("Can't open file"); }
    map = std::string(std::istreambuf_iterator<char>(file),
                      std::istreambuf_iterator<char>());
    width = map.find_first_of('\n');
    if (map.size() < width) { throw std::runtime_error("Bad format"); };

    map.erase(std::remove(map.begin(), map.end(), '\n'), map.end());

    if (map.size() % width != 0) { throw std::runtime_error("Not a Cube"); }
    height = map.size() / width;
}

Map::~Map() {}

Vector<unsigned> Map::getSize() const { return Vector<unsigned>(width, height); };

std::deque<Vector<unsigned>> Map::getChars(const char c) const
{
    std::deque<Vector<unsigned>> ret;

    for (unsigned i = 0; i < map.size(); ++i) {
        if (map.at(i) == c) {
            unsigned uXpos = i % width;
            Vector<unsigned> pos(uXpos, (i - uXpos) / width);
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
        if (i > 0 && i % other.width == 0) { os << std::endl; }
        os << other.map.at(i);
    }
    return os;
}
