#include "Map.hpp"
#include <fstream>
#include <streambuf>

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

Coords<unsigned> Map::getSize() const { return Coords(width, height); };

char Map::at(const Coords<unsigned> &idx) const
{
    unsigned point = idx.y * width + idx.x;
    if (point >= map.size())
        return '#';
    else
        return map.at(point);
}

std::deque<Coords<unsigned>> Map::getChars(const char c) const
{
    std::deque<Coords<unsigned>> ret;

    for (unsigned i = 0; i < map.size(); ++i) {
        if (map.at(i) == c) {
            unsigned uXpos = i / width;
            Coords<unsigned> pos((i - uXpos) % height, uXpos);
            ret.push_back(std::move(pos));
        }
    }
    return ret;
}

std::ostream &operator<<(std::ostream &os, const Map &other)
{
    for (unsigned i = 0; i < other.map.size(); i++) {
        if (i % other.width == 0) { os << std::endl; }
        os << other.map.at(i);
    }
    return os;
}
