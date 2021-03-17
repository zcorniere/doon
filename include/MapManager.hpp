#pragma once

#include "Map.hpp"
#include "Storage.hpp"
#include <deque>
#include <string>

class MapManager
{
public:
    MapManager(Storage &);
    ~MapManager();

    MapManager &operator++();
    MapManager &operator--();
    const std::string &getName() const;
    const Map &get() const;
    Map &get();

private:
    Storage &stor;
    std::deque<std::string> mapList;
    unsigned idx = 0;
};
