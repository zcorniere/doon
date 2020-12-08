#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "Coords.hpp"

class Player
{
public:
    enum class Move { Forward, Backward, Left, Right };
    enum class Rotation { Clockwise, CounterClockwise };

public:
    Player(Coords<float>);
    Player(const float, const float);
    ~Player();
    void rotate(const Player::Rotation, const float &);
    void move(const Player::Move, const float &);
    template <typename T>
    Coords<T> getPlayerPos() const
    {
        return static_cast<Coords<T>>(pos);
    };

public:
    Coords<float> pos;
    float angle = 0.0f;
};

#endif    //_PLAYER_HPP_
