#pragma once
#include "../imgui/imgui.h"
#include "../Utils/SettingsContext.hpp"
#include "../RenderEngine/Renderer.hpp"

struct RcsSettings
{
private:
    static const std::string enabledId;
    static const std::string verticalPowerId;
    static const std::string horizontalPowerId;

    bool _enabled;
    float _verticalPower;
    float _horizontalPower;

public:
    RcsSettings() : _enabled(false), _verticalPower(0), _horizontalPower(0) {}
    
    RcsSettings(bool enabled, float verticalPower, float horizontalPower)
    {
        _enabled = enabled;
        _verticalPower = verticalPower;
        _horizontalPower = horizontalPower;
    }

    bool isEnabled() const {
        return _enabled;
    }

    float getVerticalPower() const {
        return _verticalPower;
    }

    float getHorizontalPower() const {
        return _horizontalPower;
    }

    void render() {
        if(ImGui::BeginTabItem("RCS Settings")) {

            ImGui::Checkbox("Enabled##RCS", &_enabled);

            Renderer::renderImguiFloatValue("Vertical power", "RCS", &_verticalPower, 0.0f, 1.0f, 0.01f, 0.1f);
            Renderer::renderImguiFloatValue("Horizontal power", "RCS", &_horizontalPower, 0.0f, 1.0f, 0.01f, 0.1f);

            ImGui::EndTabItem();
        }
    }

    void load(const SettingsContext& settingsContext) {
        _enabled = settingsContext.loadBool(enabledId);
        _verticalPower = settingsContext.loadFloat(verticalPowerId);
        _horizontalPower = settingsContext.loadFloat(horizontalPowerId);
    }

    void save(SettingsContext& settingsContext) const {
        settingsContext.set(enabledId, _enabled);
        settingsContext.set(verticalPowerId, _verticalPower);
        settingsContext.set(horizontalPowerId, _horizontalPower);
    }
};

const std::string RcsSettings::enabledId = "rcs.enabled";
const std::string RcsSettings::verticalPowerId = "rcs.verticalPower";
const std::string RcsSettings::horizontalPowerId = "rcs.horizontalPower";