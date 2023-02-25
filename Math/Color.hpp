#pragma once
#include <algorithm>

struct Color
{
public:

    static const int size;

    float r;
    float g;
    float b;

    Color() : r(1.0f), g(1.0f), b(1.0f) {}

    Color(float r, float g, float b) : r(r), g(g), b(b) {
        clamp();
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
        r = std::clamp(r, 0.0f, 1.0f);
        g = std::clamp(g, 0.0f, 1.0f);
        b = std::clamp(b, 0.0f, 1.0f);
        return *this;
    }

    Color strenghten(float value){
        Color result = Color(r, g, b);
        result.r * value;
        result.g * value;
        result.b * value;
        return result;
    }
};

const int Color::size = 3;