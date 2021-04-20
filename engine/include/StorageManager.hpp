#pragma once

#include "Frame.hpp"
#include "Map.hpp"
#include <any>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <unordered_set>

class StorageManager
{
public:
    StorageManager() = default;
    StorageManager(const StorageManager &) = delete;
    ~StorageManager();

    unsigned load_directory(const std::filesystem::path &, const bool = false);

    template <typename T>
    static T load(const std::filesystem::path &);

    template <typename T>
    constexpr const T &get(const std::string &name) const
    {
        return std::any_cast<const T &>(stor.at(name));
    }
    template <typename T>
    constexpr T &get(const std::string &name)
    {
        return std::any_cast<T &>(stor.at(name));
    }
    template <typename T>
    constexpr bool contains(const std::string &name) const
    {
        return stor.contains(name) && (stor.at(name).type() == typeid(T));
    }

    template <typename T>
    std::vector<std::string> getAll() const
    {
        std::vector<std::string> ret;

        for (const auto &[i, e]: stor) {
            if (e.type() == typeid(T)) ret.push_back(i);
        }
        return ret;
    }

private:
    std::unordered_map<std::string, std::any> stor;
};

extern StorageManager *storage_manager;
