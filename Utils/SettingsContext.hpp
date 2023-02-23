#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <filesystem>

#include "Strings.hpp"
#include "Logger.hpp"

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

    std::string loadString(const std::string& name) const {
        auto it = _settings.find(name);
        if (it != _settings.end()) {
            return it->second;
        } 

        return "";
    }

    int loadInt(const std::string& name) const {
        auto value = loadString(name);

        if(value.empty()) {
            return 0;
        }

        return std::stoi(value);
    }

    float loadFloat(const std::string& name) const {
        auto value = loadString(name);
        
        if(value.empty()) {
            return 0;
        }

        return std::stof(value);
    }

    bool loadBool(const std::string& name) const {
        auto value = loadString(name);
        
        if(value.empty()) {
            return false;
        }

        return std::stoi(value) != 0;
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

