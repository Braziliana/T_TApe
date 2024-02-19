//
// Created by koelion on 05.02.24.
//

#ifndef APEX_CAMERA_H
#define APEX_CAMERA_H
#include <stdint.h>
#include "../Misc/Memory.h"
#include "../Misc/Logger.h"

#include "../Math/Matrix.h"
#include "../Math/Vector2D.h"
#include "../Math/Vector3D.h"

#include "../Features/Settings.h"

#include "Offsets.h"

namespace SDK {
    struct Camera {
    private:
        Math::ViewMatrix viewMatrix;
        Math::Vector2D screenResolution = Math::Vector2D(3440, 1440);

    public:

        bool Update() {
            screenResolution.x = Features::Settings::getInstance().GetScreenWidth();
            screenResolution.y = Features::Settings::getInstance().GetScreenHeight();

            uintptr_t renderPtr = 0x0;
            if (!Memory::getInstance().read<uintptr_t>(OFF_REGION + OFF_VIEW_RENDER, &renderPtr)) {
                Logger::log("Failed to read renderPtr");
                return false;
            }

            if(!Memory::isValidPointer(renderPtr))
            {
                Logger::logError("Failed to read renderPtr", renderPtr);
                return false;
            }

            uintptr_t matrixPtr = 0x0;
            if (!Memory::getInstance().read<uintptr_t>(renderPtr + OFF_VIEW_MATRIX, &matrixPtr) || !Memory::isValidPointer(matrixPtr)) {
                Logger::log("Failed to read matrixPtr");
                return false;
            }

            return Memory::getInstance().read<Math::ViewMatrix>(matrixPtr, &viewMatrix);
        }

        bool WorldToScreen(Math::Vector3D world, Math::Vector2D &screen) {
            Math::Vector3D transformed = viewMatrix.transform(world);

            if (transformed.z < 0.001) {
                return false;
            }

            transformed.x *= 1.0 / transformed.z;
            transformed.y *= 1.0 / transformed.z;

            Math::Vector2D halfResolution = screenResolution / 2.0f;

            screen.x = halfResolution.x + transformed.x * halfResolution.x;
            screen.y = halfResolution.y - transformed.y * halfResolution.y;

            if (screen.x < 0 || screen.x > screenResolution.x || screen.y < 0 || screen.y > screenResolution.y) {
                return false;
            }

            return true;
        }

        Math::Vector2D GetScreenMiddle() {
            return screenResolution / 2;
        }

		float DistanceFromCrosshair(Math::Vector2D screenPosition) {
			return GetScreenMiddle().distanceTo(screenPosition);
		}
    };

    typedef Camera *PCamera;
}

#endif //APEX_CAMERA_H