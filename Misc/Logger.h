//
// Created by koelion on 05.02.24.
//

#ifndef APEX_LOGGER_H
#define APEX_LOGGER_H

#include <iostream>
#include <string>
#include <sstream>

class Logger
{
private:

public:
    static void clear() {
        std::cout << "\033[2J\033[1;1H";
    }

    static void log(std::string msg) {
        log(msg.c_str());
    }

    static void log(const char* msg) {
        std::cout << msg << std::endl;
    }

    static void logError(std::string msg) {
        logError(msg.c_str());
    }

    static void logError(const char* msg) {
        std::cerr << msg << std::endl;
    }

    static void logError(const char* msg, uintptr_t ptr) {
        std::cerr << msg << " " << std::hex << ptr << std::dec << std::endl;
    }

    static void log(const char* msg, float x, float y, float z) {
        std::cout << msg << "x: " << x << ", y: " << y << ", z: " << z << std::endl;
    }
};
#endif //APEX_LOGGER_H
