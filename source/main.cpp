#include "GameInstance.hpp"
#include "Logger.hpp"
#include <csignal>

constexpr unsigned WindowWidth = 1280;
constexpr unsigned WindowHeight = 960;

Logger logger;

__attribute__((constructor)) void ctor() { logger.start(); }
__attribute__((destructor)) void dtor() { logger.stop(); }

void sig_int_handler(int signal)
{
    logger.err("SIGNAL") << "Received signal " << signal << ", exiting...";
    logger.endl();
    std::exit(signal);
}

int main()
{
    std::signal(SIGINT, sig_int_handler);
    std::signal(SIGTERM, sig_int_handler);

    GameInstance game(WindowWidth, WindowHeight);
    game.init();
    game.run();
    return 0;
}
