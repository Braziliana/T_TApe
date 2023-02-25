#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <filesystem>
#include <algorithm>

#include "Strings.hpp"
#include "Logger.hpp"
#include "Types.hpp"

class SettingsContext
{
private:
    bool _loaded;
    std::string _filename;
    std::map<std::string, std::string> _settings;

    bool loadSettings() {
        if (!std::filesystem::exists(_filename)) {
            return false;
        }

        std::ifstream file(_filename);
        
        if (file.is_open()) {
        
            std::string line;
            while (std::getline(file, line)) {

                size_t equalsPos = line.find('=');
                if (equalsPos != std::string::npos) {
                    std::string key = trim(line.substr(0, equalsPos));
                    std::string value = trim(line.substr(equalsPos + 1));

                    _settings[key] = value;
                }
            }

            file.close();
        }

        return true;
    }

public:
    SettingsContext(const std::string& filename) : _filename(filename), _loaded(false) {
        _loaded = loadSettings();
    }

    bool didLoad() {
        return _loaded;
    }

    bool reload() {
        _loaded = loadSettings();
        return _loaded;
    }

    void set(const std::string& name, const auto& value) {
        _settings[name] = std::to_string(value);
    }

    void set(const std::string& name, const std::string& value) {
        _settings[name] = value;
    }
    
    void set(const std::string& name, const float* value, const int& size) {
        _settings[name] = vectorToString(value, size);
    }

    std::string loadString(const std::string& name) const {
        auto it = _settings.find(name);
        if (it != _settings.end()) {
            return it->second;
        } 

        return "";
    }

    byte loadByte(const std::string& name, byte& value) const {
        auto valueStr = loadString(name);

        if(valueStr.empty()) {
            value = 0;
            return false;
        }

        int val = std::stoi(valueStr);
        value = std::clamp(val, 0, 255);
        return true;
    }

    bool loadInt(const std::string& name, int& value) const {
        auto valueStr = loadString(name);

        if(valueStr.empty()) {
            value = 0;
            return false;
        }

        value = std::stoi(valueStr);
        return true;
    }

    bool loadFloat(const std::string& name, float& value) const {
        auto valueStr = loadString(name);
        
        if(valueStr.empty()) {
            value = 0.0f;
            return false;
        }

        value = std::stof(valueStr);
        return true;
    }

    bool loadBool(const std::string& name, bool& value) const {
        auto valueStr = loadString(name);
        
        if(valueStr.empty()) {
            value = false;
            return false;
        }

        value = std::stoi(valueStr) != 0;
        return true;
    }

    bool loadVector(const std::string& name, float* vector, int size) const {
        auto value = loadString(name);
        
        if(value.empty()) {
            for(int i = 0; i < size; i++) {
                vector[i] = 0;
            }
            return false;
        }

        stringToVector(value, vector, size);
        return true;
    }

    void save() const {
        std::ofstream file(_filename);
        if (file.is_open()) {
            for (const auto& pair : _settings) {
                file << pair.first << " = " << pair.second << std::endl;
            }
            file.close();
        }
    }
};

