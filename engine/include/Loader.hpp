#pragma once

#include "Frame.hpp"
#include <filesystem>

namespace Loader
{
Frame loadPng(const std::filesystem::path &);

class LoaderError : public std::exception
{
public:
    LoaderError(const std::string &m): msg(std::move(m)) {}
    const std::string &getMessage() const { return msg; }

private:
    const std::string msg;
};
}    // namespace Loader
