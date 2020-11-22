#ifndef _MAP_HPP_
#define _MAP_HPP_

#include <filesystem>
#include <string>
#include <vector>

class Map {
    public:
        Map(const std::filesystem::path path);
        ~Map();
        char operator[](const std::size_t idx);
        char at(const std::size_t idx);
        char at(const std::pair<std::size_t, std::size_t> idx);

    public:
        std::string map;
        unsigned width;
        unsigned height;
};

std::ostream &operator<<(std::ostream &os, const Map &other);

#endif //_MAP_HPP_
