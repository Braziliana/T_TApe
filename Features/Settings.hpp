#pragma once
#include "../imgui/imgui.h"
#include "RcsSettings.hpp"
#include "AimbotSettings.hpp"
#include "EspSettings.hpp"
#include "../Utils/SettingsContext.hpp"
#include "../Game/Enums/HitboxType.hpp"
#include "../RenderEngine/Renderer.hpp"

class Settings
{
private:

    static const std::string sleepIntervalMsId;

    bool _opend;
    int _sleepIntervalMs;

    SettingsContext _settingsContext = SettingsContext("settings.cfg");
    RcsSettings _rcsSettings;
    AimbotSettings _aimbotSettings;
    EspSettings _espSettings;

    Settings() {
        if(_settingsContext.didLoad()) {
            internalLoad();
        }
        else {
            _rcsSettings = RcsSettings(true, 0.75, 0.5);
            _aimbotSettings = AimbotSettings(false, false, 1.0f, 1.0f, 5.0f, 0.25f, 250.0f, HitboxType::UpperChest, 90.0f);
            _espSettings = EspSettings(true, 1000, false, 100);
            _sleepIntervalMs = 10;
            save();
        }
    }
    
    ~Settings() {}

    void internalLoad() {
        _sleepIntervalMs = _settingsContext.loadInt(sleepIntervalMsId);

        _espSettings.load(_settingsContext);
        _rcsSettings.load(_settingsContext);
        _aimbotSettings.load(_settingsContext);
    }

    void renderLoadSaveTab() {
        if(ImGui::BeginTabItem("Misc")) {
            
            Renderer::renderImguiIntValue("Sleep interval ms (should recude cpu usage)", "Misc", &_sleepIntervalMs, 1, 100, 1, 5);          

            if(ImGui::Button("Load##Settings")) {
                load();
            }

            if(ImGui::Button("Save##Settings")) {
                save();
            }

            ImGui::EndTabItem();
        }
    }

public:

    static Settings& getInstance() {
        static Settings instance;
        return instance;
    }

    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;

    int getSleepIntervalMs() const {
        return _sleepIntervalMs;
    }

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
            renderLoadSaveTab();

            ImGui::EndTabBar();
        }

        ImGui::End();
    }

    void load() {
        _settingsContext.reload();
        internalLoad();
    }

    void save() {

        _settingsContext.set(sleepIntervalMsId, _sleepIntervalMs);

        _espSettings.save(_settingsContext);
        _rcsSettings.save(_settingsContext);
        _aimbotSettings.save(_settingsContext);

        _settingsContext.save();
    }
};

const std::string Settings::sleepIntervalMsId = "misc.sleepIntervalMs";