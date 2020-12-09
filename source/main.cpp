#include <SFML/Graphics.hpp>
#include <array>
#include <chrono>
#include <iostream>
#include <math.h>

#include "FrameLimiter.hpp"
#include "GameInstance.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Renderer.hpp"
#include "Snitch.hpp"

constexpr unsigned WindowWidth = 1280;
constexpr unsigned WindowHeight = 960;

int main()
{
    GameInstance game(WindowWidth, WindowHeight);
    game.init();
    game.run();
    return 0;
}
