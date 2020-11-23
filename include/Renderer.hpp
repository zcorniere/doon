#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include "Player.hpp"
#include "Map.hpp"
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <SFML/Graphics.hpp>

class Renderer {
    public:
        Renderer(const Player &player, const Map &map, Coords<unsigned> size);
        Renderer(const Renderer &) = delete;
        ~Renderer();
        void run();
        void run_threaded();
        void stop();
        void update();
        const sf::Image &getImage(const bool &bWait = false);

    private:
        void wait();

    private:
        std::atomic_bool bQuit = false;

        sf::Image img;
        Coords<unsigned> size;
        const Player &player;
        const Map &map;

        std::thread rendy_thread;

        std::mutex mVariable;
        std::condition_variable vBlocking;

        std::mutex mRendy;
        std::condition_variable vRendy;
};

#endif //_RENDERER_HPP_
