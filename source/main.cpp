#include "GameInstance.hpp"
#include "Logger.hpp"

constexpr unsigned WindowWidth = 1280;
constexpr unsigned WindowHeight = 960;

Logger logger;

__attribute__((constructor)) void ctor() { logger.start(); }
__attribute__((destructor)) void dtor() { logger.stop(); }

int main()
{
    GameInstance game(WindowWidth, WindowHeight);
    game.init();
    game.run();
    return 0;
}
