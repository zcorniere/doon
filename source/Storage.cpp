#include "Storage.hpp"
#include "Logger.hpp"
#include <SFML/Graphics/Image.hpp>

Storage::Storage(const std::filesystem::path &path)
{
    try {
        for (auto &f: std::filesystem::directory_iterator(path)) {
            if (!valid_ext.contains(f.path().extension())) continue;
            try {
                sf::Image img;
                img.loadFromFile(f.path());
                sprite_list.insert(
                    {f.path().stem(), Frame(img.getPixelsPtr(), img.getSize())});
            } catch (const std::exception &e) {
                logger.warn("STORAGE") << e.what();
                logger.endl();
            }
        }
    } catch (const std::exception &e) {
        logger.err("STORAGE") << e.what();
        logger.endl();
    }
}

Storage::~Storage() {}
