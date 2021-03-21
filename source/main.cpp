#include "GameInstance.hpp"
#include "Logger.hpp"
#include "MapManager.hpp"
#include "RenderManager.hpp"
#include "StorageManager.hpp"
#include "ThreadManager.hpp"
#include "Vector.hpp"

constexpr const char sAssetsPath[] = "./assets/";

constexpr const unsigned WindowWidth = 1280;
constexpr const unsigned WindowHeight = 960;
constexpr const Vector<unsigned> uSize(WindowWidth, WindowHeight);

int main()
try {
    logger->start();
    thread_manager->start();
    storage_manager->load(sAssetsPath);
    map_manager->start();
    render_manager->resize(uSize);
    GameInstance game(uSize);

    game.init();
    game.run();
    return 0;
} catch (const std::exception &e) {
    logger->err() << e.what();
    logger->endl();
    return 84;
}
