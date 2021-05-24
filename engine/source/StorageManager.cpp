#include "StorageManager.hpp"
#include "Logger.hpp"

constexpr const char map_extention[] = ".map";
constexpr const char image_ext[] = ".png";

unsigned StorageManager::load_directory(const std::filesystem::path &path,
                                        const bool bEmpty)
{
    if (bEmpty) {
        LOGGER_WARN << "Clearing storage manager";
        logger->endl();
        stor.clear();
    }
    auto iterator = std::filesystem::directory_iterator(path);
    auto distance = std::distance(begin(iterator), end(iterator));
    LOGGER_INFO << "Loading " << distance << " item from directory " << path;
    stor.reserve(stor.size() + distance);
    logger->endl();

    // must recreate the iterator, because it is consumed by std::distance
    for (auto &f: std::filesystem::directory_iterator(path)) {
        try {
            const auto &ext = f.path().extension();
            if (ext == image_ext) {
                Frame fr = this->load<Frame>(f);
                // swap texture X/Y since they'll be used as vertical stripes
                // better for cpu caching
                fr.rotate();
                stor.insert({f.path().stem(), std::move(fr)});
            } else if (ext == map_extention) {
                Map m = this->load<Map>(f);
                stor.insert({f.path().stem(), std::move(m)});
            } else {
                LOGGER_WARN << "Unknown extension : " << ext.c_str();
                logger->endl();
                continue;
            }
            LOGGER_INFO << "Loaded " << f.path();
            logger->endl();
        } catch (const std::exception &e) {
            LOGGER_ERR << e.what();
            logger->endl();
        }
    }
    return stor.size();
}

StorageManager::~StorageManager() {}
