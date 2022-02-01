#include "StorageManager.hpp"
#include "Logger.hpp"

constexpr const char map_extention[] = ".map";
constexpr const char image_ext[] = ".png";

constexpr const char *storageManager = "Storage Manager";
unsigned StorageManager::load_directory(const std::filesystem::path &path,
                                        const bool bEmpty)
{
    if (bEmpty) {
        logger->warn(storageManager) << "Clearing storage manager";
        stor.clear();
    }
    auto iterator = std::filesystem::directory_iterator(path);
    auto distance = std::distance(begin(iterator), end(iterator));
    logger->info(storageManager)
        << "Loading " << distance << " item from directory " << path;

    auto bar = logger->newProgressBar("Loading", distance);

    stor.reserve(stor.size() + distance);

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
                logger->warn(storageManager) << "Unknown extension : " << ext.c_str();

                continue;
            }
            logger->info(storageManager) << "Loaded " << f.path();

        } catch (const std::exception &e) {
            logger->err(storageManager) << e.what();
        }
        ++bar;
    }
    logger->deleteProgressBar(bar);
    return stor.size();
}

StorageManager::~StorageManager() {}
