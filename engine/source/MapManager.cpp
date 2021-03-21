#include "MapManager.hpp"
#include "StorageManager.hpp"

MapManager::MapManager(): mapList(storage_manager->getAll<Map>()), idx(0){};

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

const Map &MapManager::get() const { return storage_manager->get<Map>(this->getName()); }

Map &MapManager::get() { return storage_manager->get<Map>(this->getName()); }
