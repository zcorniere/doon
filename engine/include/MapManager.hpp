#pragma once

#include "Map.hpp"
#include <deque>
#include <string>

class MapManager
{
public:
    MapManager();
    ~MapManager();

    MapManager &operator++();
    MapManager &operator--();
    const std::string &getName() const;
    const Map &get() const;
    Map &get();

private:
    std::deque<std::string> mapList;
    unsigned idx = 0;
};

extern MapManager *map_manager;
