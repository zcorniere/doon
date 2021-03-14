#ifndef _STORAGE_HPP_
#define _STORAGE_HPP_

#include "Frame.hpp"
#include "Map.hpp"
#include <any>
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
    inline const T &get(const std::string &name) const
    {
        return std::any_cast<const T &>(stor.at(name));
    }
    template <typename T>
    inline bool contains(const std::string &name) const
    {
        return stor.contains(name) && (stor.at(name).type() == typeid(T));
    }

private:
    std::unordered_map<std::string, std::any> stor;
};

#endif    //_STORAGE_HPP_
