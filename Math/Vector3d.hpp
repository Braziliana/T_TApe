#pragma once
#include <cmath>

struct Vector3d {
public:
    float x;
    float y;
    float z;

    Vector3d() : x(0), y(0), z(0) {}

    Vector3d(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3d operator+(const Vector3d& other) const {
        return Vector3d(x + other.x, y + other.y, z + other.z);
    }

    Vector3d operator-(const Vector3d& other) const {
        return Vector3d(x - other.x, y - other.y, z - other.z);
    }

    Vector3d operator*(const float scalar) const {
        return Vector3d(x * scalar, y * scalar, z * scalar);
    }

    Vector3d operator/(const float scalar) const {
        return Vector3d(x / scalar, y / scalar, z / scalar);
    }

    Vector3d& operator+=(const Vector3d& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector3d& operator-=(const Vector3d& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector3d& operator*=(const float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vector3d& operator/=(const float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    float dot(const Vector3d& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector3d cross(const Vector3d& other) const {
        return Vector3d(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
    }

    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    float distanceTo(const Vector3d& other) const {
        return (other - *this).length();
    };

    Vector3d& normalize() {
        float len = length();
        if (len > 0) {
            x /= len;
            y /= len;
            z /= len;
        }
        return *this;
    }

    Vector3d direction(const Vector3d& target) const {
        return (target - *this).normalize();
    }

    bool equals(const Vector3d& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool isValid() {
        if(std::isnan(x) || std::isinf(x) || std::isnan(y) || std::isinf(y) || std::isnan(z) || std::isinf(z)) {
            return false;
        }
        
        return true;
    }

    static Vector3d zero() {
        return Vector3d(0, 0, 0);
    }
};
