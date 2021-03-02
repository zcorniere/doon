#include "GameInstance.hpp"
#include "Logger.hpp"
#include <csignal>

constexpr const unsigned WindowWidth = 1280;
constexpr const unsigned WindowHeight = 960;

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

    try {
        GameInstance game(WindowWidth, WindowHeight);
        game.init();
        game.run();
    } catch (const std::exception &e) {
        logger.err() << e.what();
        logger.endl();
    }
    return 0;
}
