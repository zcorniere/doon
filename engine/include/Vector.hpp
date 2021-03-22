#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <compare>
#include <iostream>

template <class T>
struct Vector {
    T x = 0;
    T y = 0;
    constexpr Vector() = default;
    constexpr Vector(sf::Vector2<T> o): x(o.x), y(o.y){};
    constexpr Vector(T x, T y): x(x), y(y){};
    constexpr Vector(T x): x(x), y(x){};
    constexpr Vector(const Vector<T> &other) = default;
    constexpr Vector(Vector<T> &&other)
    {
        x = std::move(other.x);
        y = std::move(other.y);
    }
    constexpr Vector &operator=(Vector other)
    {
        std::swap(x, other.x);
        std::swap(y, other.y);
        return *this;
    }
    constexpr T atan() const { return std::atan2(y, x); }
    constexpr T mag() const { return std::sqrt(x * x + y * y); };
    constexpr T mag2() const { return x * x + y * y; };
    constexpr Vector norm() const
    {
        T r = 1 / this->mag();
        return *this * r;
    };
    constexpr Vector floor() const { return Vector(std::floor(x), std::floor(y)); }
    constexpr Vector ceil() const { return Vector(std::ceil(x), std::ceil(y)); }
    constexpr Vector min(const Vector &other) const
    {
        return Vector(std::min(x, other.x), std::min(y, other.y));
    };
    constexpr Vector max(const Vector &other) const
    {
        return Vector(std::max(x, other.x), std::max(y, other.y));
    };
    constexpr T ratio() const { return x / y; };
    constexpr T cross(const Vector &other) const
    {
        return T(this->x * other.y - this->y * other.y);
    }
    constexpr T dot(const Vector &other) const
    {
        return T(this->x * other.x + this->y * other.y);
    };

    // constexpr std::strong_ordering operator<=>(const Vector &) const = default;
    constexpr bool operator>=(const Vector &other) const
    {
        return x >= other.x || y >= other.y;
    }
    constexpr bool operator==(const Vector &) const = default;
    constexpr bool operator!=(const Vector &) const = default;

    constexpr Vector operator+(const Vector &other) const
    {
        return {this->x + other.x, this->y + other.y};
    };
    constexpr Vector operator-(const Vector &other) const
    {
        return {this->x - other.x, this->y - other.y};
    };
    constexpr Vector operator*(const Vector &other) const
    {
        return {this->x * other.x, this->y * other.y};
    };
    constexpr Vector operator/(const Vector &other) const
    {
        return {this->x / other.x, this->y / other.y};
    };
    constexpr Vector operator+(const T &other) const
    {
        return {this->x + other, this->y + other};
    };
    constexpr Vector operator-(const T &other) const
    {
        return {this->x - other, this->y - other};
    };
    constexpr Vector operator*(const T &other) const
    {
        return {this->x * other, this->y * other};
    };
    constexpr Vector operator/(const T &other) const
    {
        return {this->x / other, this->y / other};
    };
    constexpr Vector &operator+=(const Vector &other)
    {
        this->x += other.x;
        this->y += other.y;
        return *this;
    };
    constexpr Vector &operator-=(const Vector &other)
    {
        this->x -= other.x;
        this->y -= other.y;
        return *this;
    };
    constexpr Vector &operator*=(const Vector &other)
    {
        this->x *= other.x;
        this->y *= other.y;
        return *this;
    };
    constexpr Vector &operator/=(const Vector &other)
    {
        this->x /= other.x;
        this->y /= other.y;
        return *this;
    };
    constexpr Vector &operator+=(const T &other)
    {
        this->x += other;
        this->y += other;
        return *this;
    };
    constexpr Vector &operator-=(const T &other)
    {
        this->x -= other;
        this->y -= other;
        return *this;
    };
    constexpr Vector &operator*=(const T &other)
    {
        this->x *= other;
        this->y *= other;
        return *this;
    };
    constexpr Vector &operator/=(const T &other)
    {
        this->x /= other;
        this->y /= other;
        return *this;
    };

    template <class E>
    constexpr operator Vector<E>() const
    {
        return {static_cast<E>(this->x), static_cast<E>(this->y)};
    }
};

namespace std
{

template <typename E>
constexpr bool isnan(Vector<E> &o)
{
    return isnan(o.x) && isnan(o.y);
};
}    // namespace std

template <typename T>
std::ostream &operator<<(std::ostream &os, const Vector<T> &other)
{
    os << "Vector<" << typeid(other.x).name() << "> { x = " << other.x
       << ", y = " << other.y << "}";
    return os;
}
