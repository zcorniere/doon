#include "StorageManager.hpp"
#include "Logger.hpp"
#include "Loader.hpp"

constexpr const char map_extention[] = ".map";
const std::unordered_set<std::string> StorageManager::valid_ext = {".jpg", ".png"};

unsigned StorageManager::load(const std::filesystem::path &path)
{
    try {
        for (auto &f: std::filesystem::directory_iterator(path)) {
            try {
                if (valid_ext.contains(f.path().extension())) {
                    Frame fr(Loader::loadPng(f));
                    // swap texture X/Y since they'll be used as vertical stripes
                    // better for cpu caching
                    fr.rotate();
                    stor.insert({f.path().stem(), std::move(fr)});
                } else if (f.path().extension() == map_extention) {
                    stor.insert({f.path().stem(), Map(f.path())});
                }
                logger->info("STORAGE") << "Loaded " << f.path();
                logger->endl();
            } catch (const std::exception &e) {
                logger->warn("STORAGE") << e.what();
                logger->endl();
            }
        }
    } catch (const std::exception &e) {
        logger->err("STORAGE") << e.what();
        logger->endl();
    }
    return stor.size();
}

StorageManager::~StorageManager() {}
