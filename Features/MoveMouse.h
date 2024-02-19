//
// Created by koelion on 06.02.24.
//

#ifndef APEX_MOVEMOUSE_H
#define APEX_MOVEMOUSE_H

#include "../Math/Vector2D.h"
#include "../Misc/InputManager.h"
#include "Settings.h"

namespace Features
{
    void SetMousePosition(int x, int y) {
        InputManager::MoveMouseAbosule(x, y);
    }

    void MoveMouse(int x, int y) {
        InputManager::MoveMouseRelative(x, y);
        //Math::Vector2D middle = Settings::getInstance().GetScreenMid();
        //SetMousePosition(middle.x + x, middle.y + y);
    }
}
#endif //APEX_MOVEMOUSE_H
