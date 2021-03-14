#include "Engine.hpp"
#include "GameInstance.hpp"
#include "Logger.hpp"
#include <csignal>

constexpr const unsigned WindowWidth = 1280;
constexpr const unsigned WindowHeight = 960;
constexpr const Coords<unsigned> uSize(WindowWidth, WindowHeight);

void sig_int_handler(const int signal)
{
    logger.err("SIGNAL") << "Received signal " << signal << ", exiting...";
    logger.endl();
    std::exit(signal);
}

int main()
try {
    std::signal(SIGINT, sig_int_handler);
    std::signal(SIGTERM, sig_int_handler);

    Engine<GameInstance> game(uSize);
    return game.run();
} catch (const std::exception &e) {
    logger.err() << e.what();
    logger.endl();
    return 1;
}
