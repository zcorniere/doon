#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "Map.hpp"

class Player {
    public:
        enum class Move {
            Forward, Backward, Left, Right
        };
        enum class Rotation {
            Clockwise, CounterClockwise
        };

    public:
        Player(const float x, const float y);
        ~Player();
        void rotate(const Player::Rotation dir, const float &fElapsedTime);
        void move(const Player::Move dir, const float &fElapsedTime);
        template<typename T>
        Coords<T> getPlayerPos()const {
            return { static_cast<T>(x), static_cast<T>(y) };
        };

    public:
        float x = 0.0f;
        float y = 0.0f;
        float angle = 0.0f;
};

#endif //_PLAYER_HPP_
