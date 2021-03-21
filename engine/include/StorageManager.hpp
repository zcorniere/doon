#pragma once

#include "Frame.hpp"
#include "Map.hpp"
#include <any>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>

class StorageManager
{
public:
    static const std::unordered_set<std::string> valid_ext;

public:
    StorageManager() = default;
    StorageManager(const StorageManager &) = delete;
    ~StorageManager();

    unsigned load(const std::filesystem::path &);
    template <typename T>
    inline const T &get(const std::string &name) const
    {
        return std::any_cast<const T &>(stor.at(name));
    }
    template <typename T>
    inline T &get(const std::string &name)
    {
        return std::any_cast<T &>(stor.at(name));
    }
    template <typename T>
    inline bool contains(const std::string &name) const
    {
        return stor.contains(name) && (stor.at(name).type() == typeid(T));
    }

    template <typename T>
    std::deque<std::string> getAll() const
    {
        std::deque<std::string> ret;

        for (auto &[i, e]: stor) {
            if (e.type() == typeid(T)) ret.push_back(i);
        }
        return ret;
    }

private:
    std::unordered_map<std::string, std::any> stor;
};

extern StorageManager *storage_manager;