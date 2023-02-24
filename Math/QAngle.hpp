#pragma once
#include <cmath>
#include <algorithm>
#include <limits>

#include "Vector2d.hpp"
#include "Vector3d.hpp"

enum class AngleSmoothType {
    LerpSmoothing,
    LinearSmoothing,
    ExponentialSmoothing,
    SCurveSmoothing,
    BezierSmoothing,
    AccelerationSmoothing,
    JerkLimitedSmoothing
};

struct QAngle {

    float x;
    float y;

    QAngle() : x(0), y(0) {}

    QAngle(float x, float y) : x(x), y(y) {}

    inline QAngle operator+(const QAngle& other) const {
        return QAngle(x + other.x, y + other.y);
    }

    inline QAngle operator-(const QAngle& other) const {
        return QAngle(x - other.x, y - other.y);
    }

    inline QAngle operator*(const float scalar) const {
        return QAngle(x * scalar, y * scalar);
    }

    inline QAngle operator/(const float scalar) const {
        return QAngle(x / scalar, y / scalar);
    }

    inline QAngle& operator+=(const QAngle& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    inline QAngle& operator-=(const QAngle& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    inline QAngle& operator*=(const float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    inline QAngle& operator/=(const float scalar) {
        x /= scalar;
        y /= scalar;
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

    float distanceTo(const QAngle& other) const {
        return (other - *this).length();
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

        if(!targetAngle.isValid()) {
            return targetAngle;
        }

        t = std::clamp(t, 0.0f, 1.0f);
        return lerp(targetAngle, t);
    }

    QAngle lookAt(const Vector3d& from, const Vector3d& target, float t, float maxAngleChange, float verticalChange = 1.0f, float horizontalChange = 1.0f) const {
        QAngle targetAngle = lookAt(from, target, t);
        
        if(!targetAngle.isValid()) {
            return targetAngle;
        }

        QAngle angleChange = targetAngle - *this;
        angleChange = angleChange.clamp(-maxAngleChange, maxAngleChange);
        
        angleChange.x *= verticalChange; 
        angleChange.y *= horizontalChange;

        return *this + angleChange;
    }

    static QAngle lerpSmoothing(const QAngle& currentAngle, const QAngle& targetAngle, const float smoothingFactor) {
        return currentAngle.lerp(targetAngle, std::clamp(smoothingFactor, 0.0f, 1.0f));
    }

    static QAngle linearSmoothing(const QAngle& currentAngle, const QAngle& targetAngle, const float smoothingFactor) {
        return currentAngle + (targetAngle - currentAngle) * smoothingFactor;
    }

    static QAngle exponentialSmoothing(const QAngle& currentAngle, const QAngle& targetAngle, const float smoothingFactor) {
        return targetAngle * smoothingFactor + currentAngle * (1 - smoothingFactor);
    }

    static QAngle sCurveSmoothing(const QAngle& currentAngle, const QAngle& targetAngle, const float smoothingFactor) {
        float factor = smoothingFactor * smoothingFactor * (3 - 2 * smoothingFactor);

        return targetAngle * factor + currentAngle * (1 - factor);
    }

    static QAngle bezierSmoothing(const QAngle& currentAngle, const QAngle& targetAngle, const float smoothingFactor) {
        const float distanceToTarget = currentAngle.distanceTo(targetAngle);
        const float t = 1.0f - pow(smoothingFactor, distanceToTarget);
        const float smoothness = 0.5f;

        const QAngle midAngle = (currentAngle + targetAngle) * 0.5f;
        const QAngle controlPoint = (midAngle - currentAngle) * smoothness + currentAngle;
        
        return currentAngle * ((1 - t) * (1 - t)) + controlPoint * 2 * (1 - t) * t + targetAngle * (t * t);
    }

    static QAngle accelerationSmoothing(const QAngle& currentAngle, const QAngle& targetAngle, const float deltaTime, const float maxAcceleration) {
        const float maxDeltaAngle = maxAcceleration * deltaTime;
        const QAngle deltaAngle = targetAngle - currentAngle;
        const float deltaAngleMagnitude = deltaAngle.length();

        if (deltaAngleMagnitude <= maxDeltaAngle) {
            return targetAngle;
        }

        const QAngle deltaAngleNormalized = deltaAngle / deltaAngleMagnitude;

        return currentAngle + deltaAngleNormalized * maxDeltaAngle;
    }

    static QAngle jerkLimitedSmoothing(const QAngle& currentAngle, const QAngle& targetAngle, const float deltaTime, const float maxJerk) {
        const float maxDeltaAngle = 0.5f * maxJerk * deltaTime * deltaTime;
        const QAngle deltaAngle = targetAngle - currentAngle;
        const float deltaAngleMagnitude = deltaAngle.length();
        
        if (deltaAngleMagnitude <= maxDeltaAngle) {
            return targetAngle;
        }

        const QAngle deltaAngleNormalized = deltaAngle / deltaAngleMagnitude;

        return currentAngle + deltaAngleNormalized * maxDeltaAngle;
    }

    inline QAngle lerp(const QAngle& other, float t) const {
        return (*this) * (1.0f - t) + other * t;
    }

    inline QAngle& fixAngle() {

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

    inline bool isValid() {
        if(std::isnan(x) || std::isinf(x) || std::isnan(y) || std::isinf(y)) {
            return false;
        }
        
        return true;
    }

    inline static QAngle zero() {
        return QAngle(0, 0);
    }
};