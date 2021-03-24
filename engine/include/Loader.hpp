#pragma once

#include <filesystem>

namespace Loader
{
template <class T>
T load(const std::filesystem::path &);
};
