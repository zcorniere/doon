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

    for (std::size_t it = map.find('\n');
         it != std::string::npos; it = map.find('\n')) {
        map.erase(it, 1);
    }

    file.seekg(0, file.beg);
    std::string line;
    std::getline(file, line);
    width = line.size();

    if (line.size() % width != 0) {
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

char Map::at(const std::pair<std::size_t, std::size_t> idx) {
    return map.at(idx.first * width + idx.second);
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
