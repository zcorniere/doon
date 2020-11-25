#ifndef _MAP_HPP_
#define _MAP_HPP_

#include <filesystem>
#include <string>
#include <vector>
#include <cmath>

template<class T>
struct Coords {
    T x = 0;
    T y = 0;
    T mag()const { return T(std::sqrt(x*x + y*y)); };
    T dot(const Coords &other)const { return T(this->x * other.x + this->y * other.y); };
    Coords operator+(const Coords &other)const { return {this->x + other.x, this->y + other.y }; };
    Coords operator-(const Coords &other)const { return {this->x - other.x, this->y - other.y }; };
    Coords operator*(const Coords &other)const { return {this->x * other.x, this->y * other.y }; };
    Coords operator/(const Coords &other)const { return {this->x / other.x, this->y / other.y }; };
    Coords operator+(const T &other)const { return {this->x + other, this->y + other };};
    Coords operator-(const T &other)const { return {this->x - other, this->y - other };};
    Coords operator*(const T &other)const { return {this->x * other, this->y * other };};
    Coords operator/(const T &other)const { return {this->x / other, this->y / other };};
    Coords &operator+=(const Coords &other) { this->x += other.x; this->y += other.y; return *this; };
    Coords &operator-=(const Coords &other) { this->x -= other.x; this->y -= other.y; return *this; };
    Coords &operator*=(const Coords &other) { this->x *= other.x; this->y *= other.y; return *this; };
    Coords &operator/=(const Coords &other) { this->x /= other.x; this->y /= other.y; return *this; };
    Coords &operator+=(const T &other) { this->x += other; this->y += other; return *this; };
    Coords &operator-=(const T &other) { this->x -= other; this->y -= other; return *this; };
    Coords &operator*=(const T &other) { this->x *= other; this->y *= other; return *this; };
    Coords &operator/=(const T &other) { this->x /= other; this->y /= other; return *this; };
    bool operator== (const Coords &other)const { return {this->x == other.x, this->y == other.y }; };
    bool operator!= (const Coords &other)const { return {this->x != other.x, this->y != other.y }; };
    bool operator> (const Coords &other)const { return (this->x > other.x && this->y > other.y); };
    bool operator< (const Coords &other)const { return (this->x < other.x && this->y < other.y); };
    bool operator>= (const Coords &other)const { return (this->x >= other.x && this->y >= other.y); };
    bool operator<= (const Coords &other)const { return (this->x <= other.x && this->y <= other.y); };
    bool operator== (const T &other)const { return {this->x == other, this->y == other }; };
    bool operator!= (const T &other)const { return {this->x != other, this->y != other }; };
    bool operator> (const T &other)const { return (this->x > other && this->y > other); };
    bool operator< (const T &other)const { return (this->x < other && this->y < other); };
    bool operator>= (const T &other)const { return (this->x >= other && this->y >= other); };
    bool operator<= (const T &other)const { return (this->x <= other && this->y <= other); };
    static T add(const Coords &co) { return co.x + co.y; };

    template<class E>
    operator Coords<E>()const { return { static_cast<E>(this->x), static_cast<E>(this->y)}; }
};

class Map {
    public:
        Map(const std::filesystem::path path);
        ~Map();
        char operator[](const std::size_t idx);
        char at(const std::size_t idx);
        char at(const Coords<unsigned> &idx);
        char at(const std::size_t idx)const;
        char at(const Coords<unsigned> &idx)const;

    public:
        std::string map;
        unsigned width;
        unsigned height;
        Coords<unsigned> coord = { .x = width, .y = height};
};

std::ostream &operator<<(std::ostream &os, const Map &other);

#endif //_MAP_HPP_
