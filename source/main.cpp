#include "GameInstance.hpp"
#include "Logger.hpp"
#include "MapManager.hpp"
#include "RenderManager.hpp"
#include "StorageManager.hpp"
#include "ThreadManager.hpp"
#include "Vector.hpp"
#include <stdlib.h>
#include <unistd.h>

constexpr const char sAssetsPath[] = "./assets/";

constexpr const char sBaseMap[] = "main";
constexpr const int iDefaultThreadNb = 4;

constexpr const unsigned WindowWidth = 1280;
constexpr const unsigned WindowHeight = 960;
constexpr const Vector<unsigned> uSize(WindowWidth, WindowHeight);

struct cmdOption {
    const char *base_map = sBaseMap;
    int nb_thread = iDefaultThreadNb;
};

cmdOption getCmdLineOption(int ac, char **av)
{
    cmdOption opt{};
    int c;

    while ((c = getopt(ac, av, "m:t:h")) != -1) {
        switch (c) {
            case 'm': opt.base_map = optarg; break;
            case 't': opt.nb_thread = atoi(optarg); break;
            case '?': {
                if (optopt == 'c')
                    std::cerr << "Option -" << optopt << " requires an argument."
                              << std::endl;
                std::exit(1);
            } break;
            default:
                std::cout << "./doon [-t NB_THREAD] [-m STARTING_MAP]" << std::endl;
                std::exit(0);
                break;
        }
    }
    return opt;
}

int main(int ac, char **av)
try {
    cmdOption opt = getCmdLineOption(ac, av);

    logger->start();
    storage_manager->load_directory(sAssetsPath);
    thread_manager->start(opt.nb_thread);
    map_manager->start(opt.base_map);
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
