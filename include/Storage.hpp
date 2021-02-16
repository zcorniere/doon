#ifndef _STORAGE_HPP_
#define _STORAGE_HPP_

#include <SFML/Graphics/Image.hpp>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>

const std::unordered_set<std::string> valid_ext = {".jpg", ".png"};

class Storage
{
public:
    Storage(const std::filesystem::path &path);
    ~Storage();
    inline const sf::Image &get(const std::string &name) const
    {
        return sprite_list.at(name);
    }
    inline bool contains(const std::string &name) const
    {
        return sprite_list.contains(name);
    }

private:
    std::unordered_map<std::string, sf::Image> sprite_list;
};

#endif    //_STORAGE_HPP_
