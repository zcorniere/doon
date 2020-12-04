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
        Player(const float, const float);
        ~Player();
        void rotate(const Player::Rotation, const float &);
        void move(const Player::Move, const float &);
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
