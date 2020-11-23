#include "Snitch.hpp"
#include "Map.hpp"
#include <fstream>
#include <streambuf>

Map::Map(const std::filesystem::path path) {
    std::ifstream file(path);

    if (!file.is_open()) {
        throw std::runtime_error("Can't open file");
    }
    map = std::string(std::istreambuf_iterator<char>(file),
                      std::istreambuf_iterator<char>());
    if ((width = map.find_first_of('\n')) == std::string::npos) {
        throw std::runtime_error("Bad format");
    };

    map.erase(std::remove(map.begin(), map.end(), '\n'), map.end());

    if (map.size() % width != 0) {
        throw std::runtime_error("Not a Cube");
    }
    height = map.size() / width;
}

Map::~Map() {}

char Map::operator[](const std::size_t idx) {
    return map[idx];
}

char Map::at(const std::size_t idx) {
    return map.at(idx);
}

char Map::at(const Coords<unsigned> &idx) {
    return map.at(idx.y * width + idx.x);
}

std::ostream &operator<<(std::ostream &os, const Map &other) {
    for (unsigned i = 0; i < other.map.size(); i++) {
        if (i % other.width == 0) {
            os << std::endl;
        }
        os << other.map.at(i);
    }
    return os;
}
