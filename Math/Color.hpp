#pragma once
#include <algorithm>

struct Color
{
public:

    static const int size;

    union {
        struct { float r, g, b; };
        float values[3];
    };

    Color() : r(255.0f), g(255.0f), b(255.0f) {}

    Color(float r, float g, float b) : r(r), g(g), b(b) {
        clamp();
    }

    Color(float* colorValues) {
        values[0] = colorValues[0];
        values[1] = colorValues[1];
        values[2] = colorValues[2];
    }

    float& operator[](size_t index) {
        return values[index];
    }

    const float& operator[](size_t index) const {
        return values[index];
    }

    Color operator*(const float& scalar) const {
        return Color(r * scalar, g * scalar, b * scalar).clamp();
    }

    Color& operator*=(const float& scalar) {
        r *= scalar;
        g *= scalar;
        b *= scalar;
        return clamp();
    }

    Color operator+(const Color& other) const {
        return Color(r + other.r, g + other.g, b + other.b).clamp();
    }

    static Color lerp(Color startColor, Color endColor, float t) {
        t = std::clamp(t, 0.0f, 1.0f);

        startColor *= (1.0f-t);
        endColor *= t;
        auto result = startColor + endColor;
        
        return result.clamp();
    }

    Color& clamp() {
        r = std::clamp(r, 0.0f, 255.0f);
        g = std::clamp(g, 0.0f, 255.0f);
        b = std::clamp(b, 0.0f, 255.0f);
        return *this;
    }
};

const int Color::size = 3;