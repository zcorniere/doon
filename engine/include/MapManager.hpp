#pragma once

#include "Map.hpp"
#include <vector>
#include <string>

class MapManager
{
public:
    MapManager();
    ~MapManager();

    void start();
    void start(const std::string &);
    MapManager &operator++();
    MapManager &operator--();
    const std::string &getName() const;
    const Map &get() const;
    Map &get();

private:
    std::vector<std::string> mapList;
    unsigned idx = 0;
};

extern MapManager *map_manager;
