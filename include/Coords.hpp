#ifndef _COORDS_HPP_
#define _COORDS_HPP_

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <compare>
#include <iostream>

template <class T>
struct Coords {
    T x = 0;
    T y = 0;
    inline constexpr Coords() = default;
    inline constexpr Coords(sf::Vector2<T> o): x(o.x), y(o.y){};
    inline constexpr Coords(T x, T y): x(x), y(y){};
    inline constexpr Coords(T x): x(x), y(x){};
    inline Coords(const Coords<T> &other) = default;
    inline Coords(Coords<T> &&other)
    {
        x = std::move(other.x);
        y = std::move(other.y);
    }
    inline Coords &operator=(Coords other)
    {
        std::swap(x, other.x);
        std::swap(y, other.y);
        return *this;
    }
    inline T atan() const { return std::atan2(y, x); }
    inline T mag() const { return std::sqrt(x * x + y * y); };
    inline T mag2() const { return x * x + y * y; };
    inline Coords norm() const
    {
        T r = 1 / this->mag();
        return *this * r;
    };
    inline Coords floor() const { return Coords(std::floor(x), std::floor(y)); }
    inline Coords ceil() const { return Coords(std::ceil(x), std::ceil(y)); }
    inline Coords min(const Coords &other) const
    {
        return Coords(std::min(x, other.x), std::min(y, other.y));
    };
    inline Coords max(const Coords &other) const
    {
        return Coords(std::max(x, other.x), std::max(y, other.y));
    };
    inline T ratio() const { return x / y; };
    inline T cross(const Coords &other) const
    {
        return T(this->x * other.y - this->y * other.y);
    }
    inline T dot(const Coords &other) const
    {
        return T(this->x * other.x + this->y * other.y);
    };

    // inline std::strong_ordering operator<=>(const Coords &) const = default;
    inline bool operator>=(const Coords &other) const
    {
        return x >= other.x || y >= other.y;
    }
    inline bool operator==(const Coords &) const = default;
    inline bool operator!=(const Coords &) const = default;

    inline Coords operator+(const Coords &other) const
    {
        return {this->x + other.x, this->y + other.y};
    };
    inline Coords operator-(const Coords &other) const
    {
        return {this->x - other.x, this->y - other.y};
    };
    inline Coords operator*(const Coords &other) const
    {
        return {this->x * other.x, this->y * other.y};
    };
    inline Coords operator/(const Coords &other) const
    {
        return {this->x / other.x, this->y / other.y};
    };
    inline Coords operator+(const T &other) const
    {
        return {this->x + other, this->y + other};
    };
    inline Coords operator-(const T &other) const
    {
        return {this->x - other, this->y - other};
    };
    inline Coords operator*(const T &other) const
    {
        return {this->x * other, this->y * other};
    };
    inline Coords operator/(const T &other) const
    {
        return {this->x / other, this->y / other};
    };
    inline Coords &operator+=(const Coords &other)
    {
        this->x += other.x;
        this->y += other.y;
        return *this;
    };
    inline Coords &operator-=(const Coords &other)
    {
        this->x -= other.x;
        this->y -= other.y;
        return *this;
    };
    inline Coords &operator*=(const Coords &other)
    {
        this->x *= other.x;
        this->y *= other.y;
        return *this;
    };
    inline Coords &operator/=(const Coords &other)
    {
        this->x /= other.x;
        this->y /= other.y;
        return *this;
    };
    inline Coords &operator+=(const T &other)
    {
        this->x += other;
        this->y += other;
        return *this;
    };
    inline Coords &operator-=(const T &other)
    {
        this->x -= other;
        this->y -= other;
        return *this;
    };
    inline Coords &operator*=(const T &other)
    {
        this->x *= other;
        this->y *= other;
        return *this;
    };
    inline Coords &operator/=(const T &other)
    {
        this->x /= other;
        this->y /= other;
        return *this;
    };

    template <class E>
    inline operator Coords<E>() const
    {
        return {static_cast<E>(this->x), static_cast<E>(this->y)};
    }
};

namespace std
{

template <typename E>
constexpr bool isnan(Coords<E> &o)
{
    return isnan(o.x) && isnan(o.y);
};
}    // namespace std

template <typename T>
std::ostream &operator<<(std::ostream &os, const Coords<T> &other)
{
    os << "Coords<" << typeid(other.x).name() << "> { x = " << other.x
       << ", y = " << other.y << "}";
    return os;
}

#endif    //_COORDS_HPP_
