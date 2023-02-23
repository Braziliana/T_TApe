#include <chrono>
#include <thread>
#include <atomic>
#include <string>
#include <GLFW/glfw3.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XInput2.h>

#include "Utils/Logger.hpp"
#include "Utils/Memory.hpp"
#include "Utils/InputManager.hpp"
#include "RenderEngine/Renderer.hpp"
#include "UI/OverlayWindow.hpp"
#include "Game/GameManager.hpp"
#include "Game/Camera.hpp"
#include "Features/FeaturesManager.hpp"
#include "Features/Settings.hpp"

OverlayWindow ow = OverlayWindow();
std::atomic_bool stop_thread(false);

void handleKeyEvent(Display* display, XEvent* event) {
    if (event->type == KeyPress) {

        if(Settings::getInstance().isOpend()) {
            Settings::getInstance().close();
            ow.captureInput(false);
        }
        else {
            ow.focus();
            Settings::getInstance().open();
            ow.captureInput(true);
        }
    }
}

void x11EventListener() {
    Display* display = XOpenDisplay(nullptr);
    Window root = DefaultRootWindow(display);
    XGrabKey(display, XKeysymToKeycode(display, XK_Insert), AnyModifier, root, False, GrabModeAsync, GrabModeAsync);
    XEvent event;
    while (!stop_thread) {
        XNextEvent(display, &event);
        handleKeyEvent(display, &event);
    }
    XCloseDisplay(display);
}


bool update() {
    try {
        GameManager::getInstance().update();

        if(!LevelManager::getInstance().isPlayable()) {
            return true;
        }

        FeaturesManager::getInstance().update();
        FeaturesManager::getInstance().render();

        return true;
    }
    catch(...) {
        return false;
    }

    return false;
}

void renderImgui() {
    Settings::getInstance().render();
}

bool init() {
    if(!ow.init()) {
        ow.destroy();
        return false;
    }

    int screenWidth;
    int screenHeight;
    ow.getScreenResolution(screenWidth, screenHeight);
    Camera::getInstance().init(screenWidth, screenHeight);

    return true;
}

int main(void)
{
    while(!Memory::getInstance().isReady()) {
        Logger::clear();
        Logger::log("Waiting for game");
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    Logger::clear();
    Logger::log("T_T Ape");

    if(!init()) {
        return -1;
    }

    std::thread x11_thread(x11EventListener);
    x11_thread.detach();

    try {
        ow.run(&update, renderImgui);
    }
    catch(...) {

    }
    stop_thread = true;
    x11_thread.join();

    return 0;
}