#pragma once

#include "Offsets.hpp"
#include "../Utils/Memory.hpp"
#include "../Math/Vector3d.hpp"
#include "../Math/Vector2d.hpp"
#include "../Math/Matrix.hpp"

class Camera
{
private:
    Camera() {}
    ~Camera() {}

    Vector2d screenResolution;
    ViewMatrix _viewMatrix;

public:
    static Camera& getInstance() {
        static Camera instance;
        return instance;
    }

    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    void init(int screenWidth, int screenHeight) {
        std::cout << "Size: " << screenWidth << ", " << screenHeight << std::endl;
        screenResolution = Vector2d(screenWidth, screenHeight);
    }
    
    const Vector2d& getScrrenResolution() const {
        return screenResolution;
    };

    void update() {
        ulong renderPtr = Memory::getInstance().read<ulong>(Offsets::getInstance().region + Offsets::getInstance().viewRender);
        ulong matrixPtr = Memory::getInstance().read<ulong>(renderPtr + Offsets::getInstance().viewMatrix);
        _viewMatrix = Memory::getInstance().read<ViewMatrix>(matrixPtr);
    }

    bool worldToScreen(Vector3d worldPosition, Vector2d& screenPosition) const {
        Vector3d transformed = _viewMatrix.transform(worldPosition);
 
        if (transformed.z < 0.001){
            return false;
        }

        transformed.x *= 1.0 / transformed.z;
        transformed.y *= 1.0 / transformed.z;

        Vector2d halfResoltion = screenResolution / 2.0f;

        screenPosition.x = halfResoltion.x + transformed.x * halfResoltion.x;
        screenPosition.y = halfResoltion.y - transformed.y * halfResoltion.y;

        return true;
    }
};