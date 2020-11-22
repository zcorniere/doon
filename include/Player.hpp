#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include <map>
#include "Map.hpp"

class Player {
    public:
        enum class Move {
            Forward, Backward, Left, Right
        };

    public:
        Player(const float x, const float y);
        ~Player();
        void rotate(const float r);
        void move(const Player::Move dir);
        template<typename T>
        Coords<T> getPlayerPos()const;

    public:
        float x;
        float y;
        float angle = 0.0f;
};

#endif //_PLAYER_HPP_
