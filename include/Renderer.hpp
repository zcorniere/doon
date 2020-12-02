#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include "interface/AThreaded.hpp"
#include "Player.hpp"
#include "Map.hpp"
#include <memory>
#include <mutex>
#include <atomic>
#include <unordered_map>
#include <condition_variable>
#include <SFML/Graphics.hpp>

class Renderer: public AThreaded {
    public:
        Renderer(const Player &player, const Map &map, Coords<unsigned> size, const std::string &assets);
        Renderer(const Renderer &) = delete;
        ~Renderer();
        virtual void run()final;
        virtual void stop()final;
        const sf::Image &getImage(const bool &bWait = false);

    private:
        const sf::Color sampleTexture(const Coords<float> &fSample, const std::string &texture);

    private:
        std::atomic_bool bQuit = false;

        std::mutex mRendy;
        std::condition_variable vRendy;
        sf::Image lastImg;
        sf::Image img;
        std::unordered_map<std::string, sf::Image> sprite_list;

        Coords<unsigned> size;
        const Player &player;
        const Map &map;
};

#endif //_RENDERER_HPP_
