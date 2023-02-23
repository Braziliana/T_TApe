#pragma once
#include "../imgui/imgui.h"
#include "../Utils/SettingsContext.hpp"
#include "../RenderEngine/Renderer.hpp"

struct EspSettings
{
private:
    static const std::string enabledId;
    static const std::string glowRangeInMetersId;
    static const std::string drawHitboxPositionId;
    static const std::string hitboxRangeInMetersId;

    bool _glowEnabled;
    float _glowRangeInMeters;
    bool _drawHitboxPosition;
    float _hitboxRangeInMeters;

public:
    EspSettings() : _glowEnabled(true), _glowRangeInMeters(1000), _drawHitboxPosition(false), _hitboxRangeInMeters(100) {}
    
    EspSettings(bool glow, float glowRangeInMeters, bool drawHitboxPosition, float hitboxRangeInMeters) :
        _glowEnabled(glow), _glowRangeInMeters(glowRangeInMeters),
        _drawHitboxPosition(drawHitboxPosition), _hitboxRangeInMeters(hitboxRangeInMeters) {}

    bool isGlowEnabled() const {
        return _glowEnabled;
    }

    float glowRangeInMeters() const {
        return _glowRangeInMeters;
    }

    bool isDrawHitboxPositionEnabled() const {
        return _drawHitboxPosition;
    }

    float hitboxRangeInMeters() const {
        return _hitboxRangeInMeters;
    }

    void render() {
        if(ImGui::BeginTabItem("ESP Settings")) {

            ImGui::Checkbox("Glow", &_glowEnabled);
            Renderer::renderImguiFloatValue("Glow range in meters", "ESP", &_glowRangeInMeters, 25.0f, 10000.0f, 1.0f, 50.0f);

            ImGui::Checkbox("Hitbox Position", &_drawHitboxPosition);
            Renderer::renderImguiFloatValue("Hitbox range in meters", "ESP", &_hitboxRangeInMeters, 25.0f, 500.0f, 1.0f, 25.0f);

            ImGui::EndTabItem();
        }
    }

    void load(const SettingsContext& settingsContext) {
        _glowEnabled = settingsContext.loadBool(enabledId);
        _glowRangeInMeters = settingsContext.loadFloat(glowRangeInMetersId);
        _drawHitboxPosition = settingsContext.loadBool(drawHitboxPositionId);
        _hitboxRangeInMeters = settingsContext.loadFloat(hitboxRangeInMetersId);
    }

    void save(SettingsContext& settingsContext) const {
        settingsContext.set(enabledId, _glowEnabled);
        settingsContext.set(glowRangeInMetersId, _glowRangeInMeters);
        settingsContext.set(drawHitboxPositionId, _drawHitboxPosition);
        settingsContext.set(hitboxRangeInMetersId, _hitboxRangeInMeters);
    }
};

const std::string EspSettings::enabledId = "esp.glowEnabled";
const std::string EspSettings::glowRangeInMetersId = "esp.glowRangeInMeters";
const std::string EspSettings::drawHitboxPositionId = "esp.drawHitboxPosition";
const std::string EspSettings::hitboxRangeInMetersId = "esp.hitboxRangeInMeters";