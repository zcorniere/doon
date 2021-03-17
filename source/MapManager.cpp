#include "MapManager.hpp"

MapManager::MapManager(Storage &s): stor(s), mapList(stor.getAll<Map>()), idx(0){};

MapManager::~MapManager() {}

MapManager &MapManager::operator++()
{
    idx++;
    if (idx >= mapList.size()) idx = 0;
    return *this;
}

MapManager &MapManager::operator--()
{
    if (idx == 0)
        idx = mapList.size();
    else
        idx--;
    return *this;
}

const std::string &MapManager::getName() const { return mapList.at(idx); }

const Map &MapManager::get() const { return stor.get<Map>(this->getName()); }

Map &MapManager::get() { return stor.get<Map>(this->getName()); }
