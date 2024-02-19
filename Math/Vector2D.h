//
// Created by koelion on 05.02.24.
//

#ifndef APEX_VECTOR2D_H
#define APEX_VECTOR2D_H

#include <cmath>

namespace Math
{
    struct Vector2D
    {
    public:
        float x;
        float y;

        Vector2D() : x(0), y(0) {}

        Vector2D(float x, float y) : x(x), y(y) {}

        Vector2D operator+(const Vector2D& other) const {
            return Vector2D(x + other.x, y + other.y);
        }

        Vector2D operator-(const Vector2D& other) const {
            return Vector2D(x - other.x, y - other.y);
        }

        Vector2D operator*(const float scalar) const {
            return Vector2D(x * scalar, y * scalar);
        }

        Vector2D operator/(const float scalar) const {
            return Vector2D(x / scalar, y / scalar);
        }

        Vector2D& operator+=(const Vector2D& other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vector2D& operator-=(const Vector2D& other) {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        Vector2D& operator*=(const float scalar) {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        Vector2D& operator/=(const float scalar) {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        float distanceTo(const Vector2D& other) const {
            return (other - *this).length();
        };

        float dot(const Vector2D& other) const {
            return x * other.x + y * other.y;
        }

        float length() const {
            return std::sqrt(x * x + y * y);
        }

        Vector2D& normalize() {
            float len = length();
            if (len > 0) {
                x /= len;
                y /= len;
            }
            return *this;
        }
    };

    typedef Vector2D* PVector2D;
}

#endif //APEX_VECTOR2D_H
