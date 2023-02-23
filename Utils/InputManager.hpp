#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

#include "Logger.hpp"



enum class KeyboardKey : int {
    Key_unknown = GLFW_KEY_UNKNOWN,
    Key_space = GLFW_KEY_SPACE,
    Key_apostrophe = GLFW_KEY_APOSTROPHE,
    Key_comma = GLFW_KEY_COMMA,
    Key_minus = GLFW_KEY_MINUS,
    Key_period = GLFW_KEY_PERIOD,
    Key_slash = GLFW_KEY_SLASH,
    Key_0 = GLFW_KEY_0,
    Key_1 = GLFW_KEY_1,
    Key_2 = GLFW_KEY_2,
    Key_3 = GLFW_KEY_3,
    Key_4 = GLFW_KEY_4,
    Key_5 = GLFW_KEY_5,
    Key_6 = GLFW_KEY_6,
    Key_7 = GLFW_KEY_7,
    Key_8 = GLFW_KEY_8,
    Key_9 = GLFW_KEY_9,
    Key_semicolon = GLFW_KEY_SEMICOLON,
    Key_equal = GLFW_KEY_EQUAL,
    Key_a = GLFW_KEY_A,
    Key_b = GLFW_KEY_B,
    Key_c = GLFW_KEY_C,
    Key_d = GLFW_KEY_D,
    Key_e = GLFW_KEY_E,
    Key_f = GLFW_KEY_F,
    Key_g = GLFW_KEY_G,
    Key_h = GLFW_KEY_H,
    Key_i = GLFW_KEY_I,
    Key_j = GLFW_KEY_J,
    Key_k = GLFW_KEY_K,
    Key_l = GLFW_KEY_L,
    Key_m = GLFW_KEY_M,
    Key_n = GLFW_KEY_N,
    Key_o = GLFW_KEY_O,
    Key_p = GLFW_KEY_P,
    Key_q = GLFW_KEY_Q,
    Key_r = GLFW_KEY_R,
    Key_s = GLFW_KEY_S,
    Key_t = GLFW_KEY_T,
    Key_u = GLFW_KEY_U,
    Key_v = GLFW_KEY_V,
    Key_w = GLFW_KEY_W,
    Key_x = GLFW_KEY_X,
    Key_y = GLFW_KEY_Y,
    Key_z = GLFW_KEY_Z,
    Key_left_bracket = GLFW_KEY_LEFT_BRACKET,
    Key_backslash = GLFW_KEY_BACKSLASH,
    Key_right_bracket = GLFW_KEY_RIGHT_BRACKET,
    Key_grave_accent = GLFW_KEY_GRAVE_ACCENT,
    Key_world_1 = GLFW_KEY_WORLD_1,
    Key_world_2 = GLFW_KEY_WORLD_2,
    Key_escape = GLFW_KEY_ESCAPE,
    Key_enter = GLFW_KEY_ENTER,
    Key_tab = GLFW_KEY_TAB,
    Key_backspace = GLFW_KEY_BACKSPACE,
    Key_insert = GLFW_KEY_INSERT,
    Key_delete = GLFW_KEY_DELETE,
    Key_right = GLFW_KEY_RIGHT,
    Key_left = GLFW_KEY_LEFT,
    Key_down = GLFW_KEY_DOWN,
    Key_up = GLFW_KEY_UP,
    Key_page_up = GLFW_KEY_PAGE_UP,
    Key_page_down = GLFW_KEY_PAGE_DOWN,
    Key_home = GLFW_KEY_HOME,
    Key_end = GLFW_KEY_END,
    Key_caps_lock = GLFW_KEY_CAPS_LOCK,
    Key_scroll_lock = GLFW_KEY_SCROLL_LOCK,
    Key_num_lock = GLFW_KEY_NUM_LOCK,
    Key_print_screen = GLFW_KEY_PRINT_SCREEN,
    Key_pause = GLFW_KEY_PAUSE,
    Key_f1 = GLFW_KEY_F1,
    Key_f2 = GLFW_KEY_F2,
    Key_f3 = GLFW_KEY_F3,
    Key_f4 = GLFW_KEY_F4,
    Key_f5 = GLFW_KEY_F5,
    Key_f6 = GLFW_KEY_F6,
    Key_f7 = GLFW_KEY_F7,
    Key_f8 = GLFW_KEY_F8,
    Key_f9 = GLFW_KEY_F9,
    Key_f10 = GLFW_KEY_F10,
    Key_f11 = GLFW_KEY_F11,
    Key_f12 = GLFW_KEY_F12,
};

enum class MouseButton : int {
    Button_1 = GLFW_MOUSE_BUTTON_1,
    Button_2 = GLFW_MOUSE_BUTTON_2,
    Button_3 = GLFW_MOUSE_BUTTON_3,
    Button_4 = GLFW_MOUSE_BUTTON_4,
    Button_5 = GLFW_MOUSE_BUTTON_5,
    Button_6 = GLFW_MOUSE_BUTTON_6,
    Button_7 = GLFW_MOUSE_BUTTON_7,
    Button_8 = GLFW_MOUSE_BUTTON_8,
    Button_Left = Button_1,
    Button_Right = Button_2,
    Button_Middle = Button_3
};

typedef void(*MouseActionFunc)(MouseButton button, double x, double y, bool press);
typedef void(*KeyboardActionFunc)(KeyboardKey key, bool press);

class InputManager {
public:
 
    static bool isKeyDown(KeyboardKey key) {
        return keys[static_cast<int>(key)] == GLFW_PRESS;
    }

    static bool isKeyUp(KeyboardKey key) {
        return keys[static_cast<int>(key)] == GLFW_RELEASE;
    }

    static bool isMouseButtonUp(MouseButton button) {
        return mouseButtons[static_cast<int>(button)] == GLFW_PRESS;
    }

    static bool isMouseButtonDown(MouseButton button) {
        return mouseButtons[static_cast<int>(button)] == GLFW_RELEASE;
    }

    static void keyCallback(int key, bool press) {
        if (key >= 0 && key < GLFW_KEY_LAST) {
            keys[key] = press;

            for(auto keyboardAction : keyboardActions) {
                keyboardAction(static_cast<KeyboardKey>(key), press);
            }

        }
    }

    static void mouseButtonCallback(int button, double x, double y, bool press) {
        if (button >= 0 && button < GLFW_MOUSE_BUTTON_LAST) {
            mouseButtons[button] = press;

            for(auto mouseAction : mouseActions) {
                mouseAction(static_cast<MouseButton>(button), x, y, press);
            }
        }
    }

    static void registerMouseAction(MouseActionFunc mouseAction) {
        if(mouseAction == nullptr) {
            return;
        }
        
        if (std::find(mouseActions.begin(), mouseActions.end(), mouseAction) == mouseActions.end()) {
            mouseActions.push_back(mouseAction);
        }
    }

    static void registerKeyboardAction(KeyboardActionFunc keyboardAction) {
        if(keyboardAction == nullptr) {
            return;
        }
        
        if (std::find(keyboardActions.begin(), keyboardActions.end(), keyboardAction) == keyboardActions.end()) {
            keyboardActions.push_back(keyboardAction);
        }
    }

private:

    static bool keys[GLFW_KEY_LAST];
    static bool mouseButtons[GLFW_MOUSE_BUTTON_LAST];

    static std::vector<MouseActionFunc> mouseActions;
    static std::vector<KeyboardActionFunc> keyboardActions;
};

bool InputManager::keys[GLFW_KEY_LAST] = {0};
bool InputManager::mouseButtons[GLFW_MOUSE_BUTTON_LAST] = {0};

std::vector<MouseActionFunc> InputManager::mouseActions = {};
std::vector<KeyboardActionFunc> InputManager::keyboardActions = {};