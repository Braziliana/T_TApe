#pragma once
#include <chrono>

struct Timer 
{
private:
    std::chrono::steady_clock::time_point lastTime;
    float time;
    float deltaTime;

public:
    Timer(): deltaTime(0.0f), time(0.0f) {
        lastTime = std::chrono::steady_clock::now();
    }

    void Reset() {
        deltaTime = 0.0f;
        time = 0.0f;
        lastTime = std::chrono::steady_clock::now();
    }

    void Update()
    {
        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<float> duration = currentTime - lastTime;
        deltaTime = duration.count();
        lastTime = currentTime;
        time += deltaTime;
    }

    float DeltaTime()
    {
        return deltaTime;
    }

    float Time() {
        return time;
    }
};

typedef Timer* PTimer;