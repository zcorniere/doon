#ifndef _MAP_HPP_
#define _MAP_HPP_

#include <filesystem>
#include <string>
#include <vector>

template<typename T>
struct Coords { T x; T y; };

class Map {
    public:
        Map(const std::filesystem::path path);
        ~Map();
        char operator[](const std::size_t idx);
        char at(const std::size_t idx);
        char at(const Coords<unsigned> &idx);

    public:
        std::string map;
        unsigned width;
        unsigned height;
};

std::ostream &operator<<(std::ostream &os, const Map &other);

#endif //_MAP_HPP_
