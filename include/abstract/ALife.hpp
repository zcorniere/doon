#ifndef _ALIFE_HPP_
#define _ALIFE_HPP_

class ALife
{
public:
    ALife(int iHP): iLifePoint(iHP){};
    constexpr int getHp() const { return iLifePoint; }
    constexpr void damage(int iDmg) { iLifePoint -= iDmg; }
    constexpr bool isAlive() const { return iLifePoint > 0; };

protected:
    int iLifePoint;
};

#endif    //_ALIFE_HPP_
