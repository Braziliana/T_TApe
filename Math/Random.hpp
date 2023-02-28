#pragma once
#include <random>

inline float randomFloat() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    return dis(gen);
}

inline float randRange(float min, float max) {
    if (min > max)
    {
        float Temp = min;
        min = max;
        max = Temp;
    }
    
    return randomFloat() * (max - min) + min;
}
