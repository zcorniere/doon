#ifndef _STORAGE_HPP_
#define _STORAGE_HPP_

#include "Frame.hpp"
#include <filesystem>
#include <unordered_map>
#include <unordered_set>

const std::unordered_set<std::string> valid_ext = {".jpg", ".png"};

class Storage
{
public:
    Storage(const std::filesystem::path &path);
    Storage(const Storage &) = default;
    ~Storage();
    inline const Frame &get(const std::string &name) const
    {
        return sprite_list.at(name);
    }
    inline bool contains(const std::string &name) const
    {
        return sprite_list.contains(name);
    }

private:
    std::unordered_map<std::string, Frame> sprite_list;
};

#endif    //_STORAGE_HPP_
