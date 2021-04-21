#include "GameInstance.hpp"
#include "Logger.hpp"
#include "MapManager.hpp"
#include "RenderManager.hpp"
#include "StorageManager.hpp"
#include "ThreadManager.hpp"
#include "Vector.hpp"

constexpr const char sAssetsPath[] = "./assets/";
constexpr const char sBaseMap[] = "main";

constexpr const unsigned WindowWidth = 1280;
constexpr const unsigned WindowHeight = 960;
constexpr const Vector<unsigned> uSize(WindowWidth, WindowHeight);

int main()
try {
    logger->start();
    storage_manager->load_directory(sAssetsPath);
    thread_manager->start();
    map_manager->start(sBaseMap);
    render_manager->resize(uSize);
    GameInstance game(uSize);

    game.init();
    game.run();
    return 0;
} catch (const std::exception &e) {
    logger->err() << e.what();
    logger->endl();
    return 1;
}
