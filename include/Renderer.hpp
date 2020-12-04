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
        Renderer(const Player &, const Map &, Coords<unsigned> ,const std::string &);
        Renderer(const Renderer &) = delete;
        ~Renderer();
        virtual void run()final;
        virtual void stop()final;
        const sf::Image &getImage(const bool & = false);
        const float &getElapsedTime()const;

    private:
        const sf::Color sampleTexture(const Coords<float> &, const std::string &)const;

    private:
        std::chrono::system_clock::time_point t1;
        float fElapsedTime = 0.0f;
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
