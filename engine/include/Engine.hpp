#pragma once

#include "Coords.hpp"
#include "FrameLimiter.hpp"
#include "Logger.hpp"
#include "ObjectManager.hpp"
#include "Renderer.hpp"
#include "ThreadPool.hpp"
#include "interface/IGame.hpp"
#include "interface/IMap.hpp"
#include <chrono>

template <std::derived_from<IGame> T, unsigned FPS = 60>
class Engine
{
public:
    Engine() = delete;
    Engine(const Coords<unsigned> size)
        :
          pool(),
          rendy(pool, size),
          objects(pool),
          game(size, pool, objects)
    {
    }
    ~Engine() = default;
    int run()
    {
        FrameLimiter<FPS> limiter;
        const uint8_t *frame_ptr = nullptr;
        auto tp1 = std::chrono::system_clock::now();

        game.init();
        while (game.isRunning()) {
            const IMap &map(game.getMap());
            auto tp2 = std::chrono::system_clock::now();
            std::chrono::duration<float> elapsedTime(tp2 - tp1);
            float fElapsedTime = elapsedTime.count();
            tp1 = std::move(tp2);

            game.update(fElapsedTime);
            objects.update(map, fElapsedTime);
            frame_ptr = rendy.update(game, map, objects, 0);

            if (frame_ptr == nullptr) {
                logger.err("ENGINE") << "Frame pointer is null !";
                logger.endl();
            } else {
                game.drawToScreen(frame_ptr);
            }
            limiter.sleep();
        }
        return 0;
    }

private:
    ThreadPool pool;
    Renderer rendy;
    ObjectManager objects;
    T game;
};
