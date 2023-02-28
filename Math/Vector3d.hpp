#pragma once
#include <cmath>

struct Vector3d {
    private:

    float calcInvSqRoot(float n) const {
    
        const float threehalfs = 1.5F;
        float y = n;
        
        long i = * ( long * ) &y;

        i = 0x5f3759df - ( i >> 1 );
        y = * ( float * ) &i;
        
        y = y * ( threehalfs - ( (n * 0.5F) * y * y ) );
        
        return y;
    }

public:
    float x;
    float y;
    float z;

    Vector3d() : x(0), y(0), z(0) {}

    Vector3d(float x, float y, float z) : x(x), y(y), z(z) {}

    bool operator==(const Vector3d& other) const {
        return x == other.x &&
            y == other.y &&
            z == other.z;
    }   
     
    bool operator!=(const Vector3d& other) const {
        return !(*this == other);
    }

    Vector3d operator+(const Vector3d& other) const {
        return Vector3d(x + other.x, y + other.y, z + other.z);
    }

    Vector3d operator-(const Vector3d& other) const {
        return Vector3d(x - other.x, y - other.y, z - other.z);
    }

    Vector3d operator*(const float scalar) const {
        return Vector3d(x * scalar, y * scalar, z * scalar);
    }

    Vector3d operator*(const Vector3d& v) const {
        return Vector3d(x * v.x, y * v.y, z * v.z);
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
    
    float length2D() const {
        return std::sqrt(x * x + y * y);
    }

    float distanceTo(const Vector3d& other) const {
        return (other - *this).length();
    };

    float distance2DTo(const Vector3d& other) const {
        return (other - *this).length2D();
    };


    Vector3d getSafeNormal(float tolerance = 0.00000001) const {
        const float squareSum = x*x + y*y + z*z;

        // Check if the vector is already a unit vector or if the magnitude is zero
        if (squareSum == 1.0f || squareSum < tolerance)
        {
            return *this;
        }

        // Calculate the square root of the magnitude
        const float scale = calcInvSqRoot(squareSum);

        return Vector3d(x*scale, y*scale, z*scale);
    }


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
