#pragma once
#include <cmath>
#include <algorithm>
#include <limits>

#include "Vector2d.hpp"
#include "Vector3d.hpp"

struct QAngle {

    float x;
    float y;

    QAngle() : x(0), y(0) {}

    QAngle(float x, float y) : x(x), y(y) {}

    QAngle operator+(const QAngle& other) const {
        return QAngle(x + other.x, y + other.y);
    }

    QAngle operator-(const QAngle& other) const {
        return QAngle(x - other.x, y - other.y);
    }

    QAngle operator*(const float scalar) const {
        return QAngle(x * scalar, y * scalar);
    }

    QAngle operator/(const float scalar) const {
        return QAngle(x / scalar, y / scalar);
    }

    QAngle& operator+=(const QAngle& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    QAngle& operator-=(const QAngle& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    QAngle& operator*=(const float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    QAngle& operator/=(const float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    float dot(const QAngle& other) const {
        return x * other.x + y * other.y;
    }

    float length() const {
        return std::sqrt(x * x + y * y);
    }

    float distanceTo(const QAngle& other) const {
        return (other - *this).length();
    };

    QAngle& normalize() {
        float len = length();
        if (len > 0) {
            x /= len;
            y /= len;
        }
        return *this;
    }

    QAngle& clamp(float minVal, float maxVal) {
        x = std::clamp(x, minVal, maxVal);
        y = std::clamp(y, minVal, maxVal);

        return *this;
    }

    static QAngle lookAt(const Vector3d& from, const Vector3d& target) {
        Vector3d direction = target-from;//from.direction(target);
        float length = direction.length();
        
        QAngle result;
        
        if(length == 0)
        {
            result.x = std::numeric_limits<float>::quiet_NaN();
        }
        else
        {
            result.x = -asinf(direction.z / length) * (180 / M_PI);
        }

        result.y = atan2f(direction.y, direction.x) * (180 / M_PI);

        return result;
    }

    QAngle lookAt(const Vector3d& from, const Vector3d& target, float t) const {
        QAngle targetAngle = lookAt(from, target);

        t = std::clamp(t, 0.0f, 1.0f);
        return lerp(targetAngle, t);
    }

    QAngle lookAt(const Vector3d& from, const Vector3d& target, float t, float maxAngleChange, float verticalChange = 1.0f, float horizontalChange = 1.0f) const {
        QAngle desiredAngle = lookAt(from, target, t);
        QAngle angleChange = desiredAngle - *this;
        angleChange = angleChange.clamp(-maxAngleChange, maxAngleChange);
        
        angleChange.x *= verticalChange; 
        angleChange.y *= horizontalChange;

        return *this + angleChange;
    }

    QAngle lerp(const QAngle& other, float t) const {
        return (*this) * (1 - t) + other * t;
    }

    QAngle& fixAngle() {

        while (x > 89.0f)
            x -= 180.f;

        while (x < -89.0f)
            x += 180.f;

        while (y > 180.f)
            y -= 360.f;

        while (y < -180.f)
            y += 360.f;

        return *this;
    }

    bool isValid() {
        if(std::isnan(x) || std::isinf(x) || std::isnan(y) || std::isinf(y)) {
            return false;
        }
        
        return true;
    }

    static QAngle zero() {
        return QAngle(0, 0);
    }
};