#include "Storage.hpp"
#include "Logger.hpp"
#include <SFML/Graphics/Image.hpp>

const std::unordered_set<std::string> Storage::valid_ext = {".jpg", ".png"};

Storage::Storage(const std::filesystem::path &path)
{
    try {
        for (auto &f: std::filesystem::directory_iterator(path)) {
            try {
                if (valid_ext.contains(f.path().extension())) {
                    sf::Image img;
                    img.loadFromFile(f.path());

                    Frame fr(img.getPixelsPtr(), img.getSize());
                    // swap texture X/Y since they'll be used as vertical stripes
                    // better for cpu caching
                    fr.rotate();
                    sprite_list.insert({f.path().stem(), std::move(fr)});
                } else if (f.path().extension() == ".map") {
                    map_list.insert({f.path().stem(), Map(f.path())});
                }
                logger.info("STORAGE") << "Loaded " << f.path();
                logger.endl();
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
