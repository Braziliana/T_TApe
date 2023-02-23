#pragma once
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

    static void log(const char* msg, float x, float y, float z) {
        std::cout << msg << "x: " << x << ",y: " << y << ", z: " << z << std::endl;
    }
};
