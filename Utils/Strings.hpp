#pragma once
#include <string>
#include <sstream>

template<typename ...Args>
std::string concatenateStrings(Args&&... args) {
    std::stringstream ss;
    (ss << ... << std::forward<Args>(args));
    return ss.str();
}