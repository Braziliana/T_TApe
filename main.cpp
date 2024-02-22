#include <chrono>
#include <thread>
#include <atomic>
#include <string>
#include <GLFW/glfw3.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XInput2.h>

#include "Features/Settings.h"
#include "Drawings/OverlayWindow.h"
#include "Misc/InputManager.h"
#include "Misc/Logger.h"
#include "Misc/Memory.h"

#include <iostream>

#include "SDK/GameContext.h"
#include "FeaturesManager.h"

Drawings::OverlayWindow ow = Drawings::OverlayWindow();
std::atomic_bool stop_thread(false);

void handleKeyEvent(Display* display, XEvent* event) {
    if (event->type == KeyPress) {

        if(Features::Settings::getInstance().isOpen()) {
            Features::Settings::getInstance().close();
            ow.captureInput(false);
        }
        else {
            ow.focus();
            Features::Settings::getInstance().open();
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
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    XCloseDisplay(display);
}

SDK::GameContext* gameContext;
Timer timer;
bool update() {
    try {
        timer.Update();
        if(gameContext == nullptr || !gameContext->Update(&timer)){
            return true;
        }
        Features::FeaturesManager::getInstance().Update(gameContext, &timer);

        return true;
    }
    catch(const std::exception& ex) {
        std::cout << "Error: " << ex.what() << std::endl;
        return false;
    }

    return false;
}

void renderImgui() {
    Features::Settings::getInstance().Render();
}

bool init() {
    if(!ow.init()) {
        ow.destroy();
        return false;
    }

    int screenWidth;
    int screenHeight;
    ow.getScreenResolution(screenWidth, screenHeight);
    //Camera::getInstance().init(screenWidth, screenHeight);

    return true;
}

int main() {

    // for(auto i=0; i<50; i++){

    //     InputManager::MoveMouse(10, 10);
    // }

    InputManager::display = XOpenDisplay(NULL);
    while(!Memory::getInstance().isReady()) {
        Logger::clear();
        Logger::log("Waiting for game");
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    Logger::clear();
    Logger::log("T_T Ape 2.0");

    if(!init()) {
        return -1;
    }
    timer.Reset();
    gameContext = new SDK::GameContext();

    std::thread x11_thread(x11EventListener);
    x11_thread.detach();

    std::thread inputManagerThread(InputManager::run);
    inputManagerThread.detach();

    try {
        ow.run(&update, renderImgui);
    }
    catch(...) {

    }

    stop_thread = true;
    InputManager::stopThread = true;
    x11_thread.join();
    inputManagerThread.join();

    XCloseDisplay(InputManager::display);
    delete gameContext;
    return 0;
}
