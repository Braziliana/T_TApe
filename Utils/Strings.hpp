#pragma once
#include <string>
#include <sstream>

template<typename ...Args>
std::string concatenateStrings(Args&&... args) {
    std::stringstream ss;
    (ss << ... << std::forward<Args>(args));
    return ss.str();
}

std::string trim(std::string str) {

    str.erase(0, str.find_first_not_of(" \t\n\r"));
    str.erase(str.find_last_not_of(" \t\n\r") + 1);

    return str;
}
