//
// Created by koelion on 05.02.24.
//

#ifndef APEX_VECTOR3D_H
#define APEX_VECTOR3D_H

#include <cmath>
#include "QAngle.h"
#include "Vector2D.h"

namespace Math
{
    struct Vector3D
    {
        float x, y, z;

        Vector3D() : x(0), y(0), z(0) {}

        Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}

        Vector3D Subtract(const Vector3D& other) const {
            return Vector3D(x - other.x, y - other.y, z - other.z);
        }

        Vector3D Add(const Vector3D& other) const {
            return Vector3D(x + other.x, y + other.y, z + other.z);
        }

        float DotProduct(const Vector3D& other) const {
            return x * other.x + y * other.y + z * other.z;
        }

        float Magnitude() const {
            return std::sqrt(x * x + y * y + z * z);
        }

        float Magnitude2D() const {
            return std::sqrt(x * x + y * y);
        }

        float Distance(const Vector3D& other) const {
            Vector3D diff = Subtract(other);
            return diff.Magnitude();
        }

        bool IsZeroVector() {
            return x == 0 && y == 0 && z == 0;
        }

        bool IsValid() {
            if (std::isnan(x) || std::isinf(x) || std::isnan(y) || std::isinf(y) || std::isnan(z) || std::isinf(z)) {
                return false;
            }
            return true;
        }

        Vector3D& Normalize() {
            float len = Magnitude();
            if (len > 0) {
                x /= len;
                y /= len;
                z /= len;
            }
            return *this;
        }

        Vector3D Multiply(float scalar) const {
            return Vector3D(x * scalar, y * scalar, z * scalar);
        }

        Vector2D To2D() const {
            return Vector2D(x, y);
        }

        float Distance2D(const Vector3D& other) const {
            return (other.Subtract(*this)).Magnitude2D();
        }

        Vector3D operator+(const Vector3D& other) const {
            return Vector3D(x + other.x, y + other.y, z + other.z);
        }

        Vector3D operator-(const Vector3D& other) const {
            return Vector3D(x - other.x, y - other.y, z - other.z);
        }

        Vector3D operator*(const Vector3D& other) const {
            return Vector3D(x * other.x, y * other.y, z * other.z);
        }

        Vector3D operator*(const float scalar) const {
            return Vector3D(x * scalar, y * scalar, z * scalar);
        }

        Vector3D& operator+=(const Vector3D& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        Vector3D& operator*=(const float scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        bool operator==(const Vector3D& other) const {
            float epsilon = 1e-5f;
            return (std::abs(x - other.x) < epsilon)
                   && (std::abs(y - other.y) < epsilon)
                   && (std::abs(z - other.z) < epsilon);
        }

        bool operator!=(const Vector3D& other) const {
            return !(*this == other);
        }


        QAngle GetAngleTo(Vector3D to) {

            Vector3D extra = Vector3D(10000, 10000, 10000);
            Vector3D v1 = *this + extra;
            Vector3D v2 = to + extra;


            Vector3D newDirection = v2 - v1;
            //auto len = newDirection.Magnitude();
            //newDirection = newDirection.Normalize();

            float pitch = -std::atan2(newDirection.z, newDirection.Magnitude2D()) * (180.0f / static_cast<float>(M_PI));
            float yaw = std::atan2(newDirection.y, newDirection.x) * (180.0f / static_cast<float>(M_PI));

            return QAngle(pitch, yaw).fixAngle();
        }

        inline void CopyFrom(const Vector3D& from) {
            x = from.x;
            y = from.y;
            z = from.z;
        }
    };

    typedef Vector3D* PVector3D;
}

#endif //APEX_VECTOR3D_H
