#ifndef _IMOVEMENT_HPP_
#define _IMOVEMENT_HPP_

namespace Movement
{
enum Move { Forward, Backward, Left, Right };
enum Rotation { Clockwise, CounterClockwise };
enum Panning { Up, Down };

class IRotate
{
    virtual void rotate(const Movement::Rotation, const float) = 0;
};
class IMove
{
    virtual void move(const Movement::Move, const float) = 0;
};
class IPan
{
    virtual void pan(const Movement::Panning, const float) = 0;
};
}    // namespace Movement

#endif    //_IMOVEMENT_HPP_
