#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include "interface/IThreaded.hpp"
#include "Player.hpp"
#include "Map.hpp"
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <SFML/Graphics.hpp>

class Renderer: public IThreaded {
    public:
        Renderer(const Player &player, const Map &map, Coords<unsigned> size);
        Renderer(const Renderer &) = delete;
        ~Renderer();
        virtual void run()final;
        virtual void stop()final;
        const sf::Image &getImage(const bool &bWait = false);

    private:
        std::atomic_bool bQuit = false;

        sf::Image img;
        Coords<unsigned> size;
        const Player &player;
        const Map &map;

        std::mutex mRendy;
        std::condition_variable vRendy;
};

#endif //_RENDERER_HPP_
