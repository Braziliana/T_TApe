#pragma once
#include <cmath>
#include "QAngle.hpp"

struct Vector2d {
public:
    float x;
    float y;

    Vector2d() : x(0), y(0) {}

    Vector2d(float x, float y) : x(x), y(y) {}

    Vector2d operator+(const Vector2d& other) const {
        return Vector2d(x + other.x, y + other.y);
    }

    Vector2d operator-(const Vector2d& other) const {
        return Vector2d(x - other.x, y - other.y);
    }

    Vector2d operator*(const float scalar) const {
        return Vector2d(x * scalar, y * scalar);
    }

    Vector2d operator/(const float scalar) const {
        return Vector2d(x / scalar, y / scalar);
    }

    Vector2d& operator+=(const Vector2d& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2d& operator-=(const Vector2d& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector2d& operator*=(const float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector2d& operator/=(const float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    float dot(const Vector2d& other) const {
        return x * other.x + y * other.y;
    }

    float length() const {
        return std::sqrt(x * x + y * y);
    }

    Vector2d& normalize() {
        float len = length();
        if (len > 0) {
            x /= len;
            y /= len;
        }
        return *this;
    }
};
