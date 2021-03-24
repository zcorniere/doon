#include "StorageManager.hpp"
#include "Loader.hpp"
#include "Logger.hpp"

constexpr const char map_extention[] = ".map";
constexpr const char image_ext[] = ".png";

unsigned StorageManager::load(const std::filesystem::path &path)
{
    try {
        for (auto &f: std::filesystem::directory_iterator(path)) {
            try {
                const auto &ext = f.path().extension();
                if (ext == image_ext) {
                    Frame fr = Loader::load<Frame>(f);
                    // swap texture X/Y since they'll be used as vertical stripes
                    // better for cpu caching
                    fr.rotate();
                    stor.insert({f.path().stem(), std::move(fr)});
                } else if (ext == map_extention) {
                    Map m = Loader::load<Map>(f);
                    stor.insert({f.path().stem(), std::move(m)});
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
