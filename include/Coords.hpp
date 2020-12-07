#ifndef _COORDS_HPP_
#define _COORDS_HPP_

#include <algorithm>
#include <iostream>
#include <cmath>

template <class T>
struct Coords {
    Coords() = default;
    Coords(T x, T y): x(x), y(y){};
    Coords(Coords<T> &other) = default;
    Coords(Coords<T> &&other)
    {
        x = std::move(other.x);
        y = std::move(other.y);
    }
    Coords &operator=(Coords other)
    {
        std::swap(x, other.x);
        std::swap(x, other.y);
        return *this;
    }
    T x = 0;
    T y = 0;
    inline T mag() const { return T(std::sqrt(x * x + y * y)); };
    inline T dot(const Coords &other) const { return T(this->x * other.x + this->y * other.y); };
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
    inline Coords operator+(const T &other) const { return {this->x + other, this->y + other}; };
    inline Coords operator-(const T &other) const { return {this->x - other, this->y - other}; };
    inline Coords operator*(const T &other) const { return {this->x * other, this->y * other}; };
    inline Coords operator/(const T &other) const { return {this->x / other, this->y / other}; };
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
    inline bool operator==(const Coords &other) const
    {
        return {this->x == other.x, this->y == other.y};
    };
    inline bool operator!=(const Coords &other) const
    {
        return {this->x != other.x, this->y != other.y};
    };
    inline bool operator>(const Coords &other) const
    {
        return (this->x > other.x && this->y > other.y);
    };
    inline bool operator<(const Coords &other) const
    {
        return (this->x < other.x && this->y < other.y);
    };
    inline bool operator>=(const Coords &other) const
    {
        return (this->x >= other.x && this->y >= other.y);
    };
    inline bool operator<=(const Coords &other) const
    {
        return (this->x <= other.x && this->y <= other.y);
    };
    inline bool operator==(const T &other) const { return {this->x == other, this->y == other}; };
    inline bool operator!=(const T &other) const { return {this->x != other, this->y != other}; };
    inline bool operator>(const T &other) const { return (this->x > other && this->y > other); };
    inline bool operator<(const T &other) const { return (this->x < other && this->y < other); };
    inline bool operator>=(const T &other) const { return (this->x >= other && this->y >= other); };
    inline bool operator<=(const T &other) const { return (this->x <= other && this->y <= other); };
    static inline T add(const Coords &co) { return co.x + co.y; };

    template <class E>
    inline operator Coords<E>() const
    {
        return {static_cast<E>(this->x), static_cast<E>(this->y)};
    }
};
template<typename T>
std::ostream &operator<<(std::ostream &os, const Coords<T> &other) {
    os << "Coords<" << typeid(other.x).name() << "> { x = " << other.x << ", y = " << other.y << "}";
    return os;
}


#endif //_COORDS_HPP_
