#pragma once
#include "Vector3d.hpp"
#include "QAngle.hpp"

class Predictor
{
private:

public:

    static Vector3d predictPosition(const Vector3d& startPosition, const Vector3d& velocityVector, const float& time, const float& precision) {
        return startPosition + (velocityVector * time) * precision;
    }

    static float predictBulletDrop(const Vector3d& startPosition, const Vector3d& targetPosition, const float& time, const float& bulletDropRate) {
        return  (0.5f * bulletDropRate * time * time);
    }
};
