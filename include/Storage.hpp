#ifndef _STORAGE_HPP_
#define _STORAGE_HPP_

#include "Frame.hpp"
#include "Map.hpp"
#include <filesystem>
#include <unordered_map>
#include <unordered_set>

class Storage
{
public:
    static const std::unordered_set<std::string> valid_ext;

public:
    Storage(const std::filesystem::path &path);
    Storage(const Storage &) = default;
    ~Storage();

    template <typename T>
    inline const T &get(const std::string &name) const;
    template <typename T>
    inline bool contains(const std::string &name) const;

    template <>
    inline const Frame &get(const std::string &name) const
    {
        return sprite_list.at(name);
    }

    template <>
    inline bool contains<Frame>(const std::string &name) const
    {
        return sprite_list.contains(name);
    }

    template <>
    inline const Map &get(const std::string &name) const
    {
        return map_list.at(name);
    }

    template <>
    inline bool contains<Map>(const std::string &name) const
    {
        return map_list.contains(name);
    }

private:
    std::unordered_map<std::string, Map> map_list;
    std::unordered_map<std::string, Frame> sprite_list;
};

#endif    //_STORAGE_HPP_
