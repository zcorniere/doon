#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "Coords.hpp"

class Player
{
public:
    enum class Move { Forward, Backward, Left, Right };
    enum class Rotation { Clockwise, CounterClockwise };
    enum class Panning { Up, Down };

public:
    Player(Coords<float>);
    Player(const float, const float);
    ~Player();
    void rotate(const Player::Rotation, const float &);
    void move(const Player::Move, const float &);
    void pan(const Player::Panning, const float &);

    template <typename T>
    inline Coords<T> getPlayerPos() const
    {
        return static_cast<Coords<T>>(pos);
    };

public:
    Coords<float> pos;
    float elevation = 1.0f;
    float angle = 0.0f;
    float fSpeedModifier = 1;
};

#endif    //_PLAYER_HPP_
