#pragma once
#include <chrono>

class TimeManager
{
private:

    std::chrono::high_resolution_clock::time_point startFrameTime;
    std::chrono::high_resolution_clock::time_point lastFrameTime;
    float time;
    float deltaTime;

    TimeManager() : time(0), deltaTime(0) {
        startFrameTime = lastFrameTime = std::chrono::high_resolution_clock::now();
    }
    ~TimeManager() {}

public:
    static TimeManager& getInstance() {
        static TimeManager instance;
        return instance;
    }

    TimeManager(const TimeManager&) = delete;
    TimeManager& operator=(const TimeManager&) = delete;

    void update() {
        std::chrono::high_resolution_clock::time_point currentFrameTime = std::chrono::high_resolution_clock::now();

        std::chrono::duration<float> applicationElapsedSeconds = std::chrono::duration_cast<std::chrono::duration<float>>(currentFrameTime - startFrameTime);
        time = applicationElapsedSeconds.count();

        std::chrono::duration<float> frameElapsedSeconds = std::chrono::duration_cast<std::chrono::duration<float>>(currentFrameTime - lastFrameTime);
        deltaTime = frameElapsedSeconds.count();

        lastFrameTime = currentFrameTime;
    }

    float getTime() const {
        return time;
    }

    float getDeltaTime() const {
        return deltaTime;
    }
};
