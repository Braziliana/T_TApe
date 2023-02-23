#pragma once
#include "../imgui/imgui.h"
#include "RcsSettings.hpp"
#include "AimbotSettings.hpp"
#include "EspSettings.hpp"

class Settings
{
private:

    bool _opend;
    Settings() {
        _rcsSettings = RcsSettings(true, 0.75, 0.5);
        _aimbotSettings = AimbotSettings(false, false, 1.0f, 1.0f, 5.0f, 0.25f, 250.0f, 2);
        _espSettings = EspSettings(true, 1000, false, 100);
    }
    
    ~Settings() {}

    RcsSettings _rcsSettings;
    AimbotSettings _aimbotSettings;
    EspSettings _espSettings;

public:

    static Settings& getInstance() {
        static Settings instance;
        return instance;
    }

    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;

    EspSettings getEspSettings() const {
        return _espSettings;
    }

    RcsSettings getRcsSettings() const {
        return _rcsSettings;
    };

    AimbotSettings getAimbotSettings() const {
        return _aimbotSettings;
    };

    void open() {
        _opend = true;
    }

    void close() {
        _opend = false;
    }

    bool isOpend() const {
        return _opend;
    }

    void render() {
        if(!_opend){
            return;
        }

        ImGui::Begin("T_T Ape");
        
        if (ImGui::BeginTabBar("Settings")) {
            _espSettings.render();
            _rcsSettings.render();
            _aimbotSettings.render();
            ImGui::EndTabBar();
        }

        ImGui::End();
    }
};
