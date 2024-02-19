//
// Created by koelion on 05.02.24.
//

#ifndef APEX_QANGLE_H
#define APEX_QANGLE_H

#include <cmath>
#include <algorithm>
#include <limits>

namespace Math
{
    inline float deg2rad(float deg) {
        return deg * static_cast<float>(M_PI) / 180.0f;
    }

    struct QAngle
    {

        float x;
        float y;

        QAngle() : x(0), y(0) {}

        QAngle(float x, float y) : x(x), y(y) {}

        inline QAngle operator+(const QAngle& other) const {
            return QAngle(x + other.x, y + other.y).fixAngle();
        }

        inline QAngle operator-(const QAngle& other) const {
            return QAngle(x - other.x, y - other.y).fixAngle();
        }

        inline QAngle operator*(const float scalar) const {
            return QAngle(x * scalar, y * scalar).fixAngle();
        }

        inline QAngle operator/(const float scalar) const {
            return QAngle(x / scalar, y / scalar).fixAngle();
        }

        inline QAngle& operator+=(const QAngle& other) {
            x += other.x;
            y += other.y;
            fixAngle();
            return *this;
        }

        inline QAngle& operator-=(const QAngle& other) {
            x -= other.x;
            y -= other.y;
            fixAngle();
            return *this;
        }

        inline QAngle& operator*=(const float scalar) {
            x *= scalar;
            y *= scalar;
            fixAngle();
            return *this;
        }

        inline QAngle& operator/=(const float scalar) {
            x /= scalar;
            y /= scalar;
            fixAngle();
            return *this;
        }

        inline bool operator==(const QAngle& other) const
        {
            return x == other.x && y == other.y;
        }

        inline bool operator!=(const QAngle& other) const
        {
            return !(*this == other);
        }

        inline float dot(const QAngle& other) const {
            return x * other.x + y * other.y;
        }

        inline float length() const {
            return std::sqrt(x * x + y * y);
        }

        float AngleDistance(const float angle1, const float angle2, const float range) const {
            float difference = std::abs(angle1 - angle2);
            float distance = difference > range / 2 ? range - difference : difference;
            return distance;
        }

        float distanceTo(const QAngle& other) const {

            float pitchRange = 180.0f;
            float yawRange = 360.0f;

            float pitchDistance = AngleDistance(x, other.x, pitchRange);
            float yawDistance = AngleDistance(y, other.y, yawRange);

            float totalDistance = std::sqrt(pitchDistance* pitchDistance + yawDistance* yawDistance);

            return totalDistance;
        };

        inline QAngle& normalize() {
            float len = length();
            if (len > 0) {
                x /= len;
                y /= len;
            }
            return *this;
        }

        inline QAngle& clamp(float minVal, float maxVal) {
            x = std::clamp(x, minVal, maxVal);
            y = std::clamp(y, minVal, maxVal);

            return *this;
        }

        inline QAngle lerp(const QAngle& other, float t) const {
            return (*this) * (1.0f - t) + other * t;
        }

        inline QAngle& fixAngle() {
            if (!isValid()) {
                return *this;
            }

            while (x > 90.0f)
                x -= 180.f;

            while (x < -90.0f)
                x += 180.f;

            while (y > 180.f)
                y -= 360.f;

            while (y < -180.f)
                y += 360.f;

            return *this;
        }

        inline bool isValid() const {
            if (std::isnan(x) || std::isinf(x) || std::isnan(y) || std::isinf(y)) {
                return false;
            }

            return true;
        }

        inline static QAngle zero() {
            return QAngle(0, 0);
        }

        static float GetFov(const QAngle& viewAngle, const QAngle& aimAngle)
        {
            QAngle delta = aimAngle - viewAngle;
            delta.fixAngle();

            return std::sqrt(delta.x * delta.x + delta.y * delta.y);
        }

        inline void CopyFrom(const QAngle& angle) {
            x = angle.x;
            y = angle.y;
        }
    };

    typedef QAngle* PQAngle;
}

#endif //APEX_QANGLE_H
