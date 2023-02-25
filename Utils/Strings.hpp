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

std::string vectorToString(const float* vector, const int size) {

    std::stringstream ss;

    ss << '(' << vector[0];

    for(int i = 1; i < size; i++) {
        ss << ", " << vector[i];
    }

    ss << ')';

    return ss.str();
}

void stringToVector(const std::string& s, float* vector, const int size) {

    std::string valueString = s.substr(1, s.length() - 2);
    std::stringstream ss(valueString);
    std::string crruentValue;

    int i=0;
    for(i = 0; i < size; i++) {
        if(getline(ss, crruentValue, ',')) {
            vector[i] = std::stof(crruentValue);
        }
        else {
            break;
        }
    }

    for(; i < size; i++) {
        vector[i] = 0.0f;
    }
}

