#pragma once
#include "../imgui/imgui.h"
#include "../Utils/SettingsContext.hpp"
#include "../RenderEngine/Renderer.hpp"
#include "../Math/Color.hpp"

enum class GlowType : int {
    DynamicColorGlow = 0,
    StaticColorGlow = 1
};

struct EspSettings
{
private:
    static const std::string glowEnabledId;
    static const std::string glowTypeId;
    static const std::string glowRangeInMetersId;
    static const std::string glowStaticColorId;
    static const std::string glowMaxShieldColorId;
    static const std::string glowMinShieldColorId;
    static const std::string glowMaxHealthColorId;
    static const std::string glowMinHealthColorId;

    static const std::string drawHitboxPositionId;
    static const std::string hitboxRangeInMetersId;

    bool _glowEnabled;
    GlowType _glowType;
    float _glowRangeInMeters;
    Color _glowStaticColor;
    Color _glowMaxShieldColor;
    Color _glowMinShieldColor;
    Color _glowMaxHealthColor;
    Color _glowMinHealthColor;

    bool _drawHitboxPosition;
    float _hitboxRangeInMeters;

public:
    EspSettings() :
        _glowEnabled(true),
        _glowType(GlowType::DynamicColorGlow),
        _glowRangeInMeters(1000),
        _glowStaticColor(255.0f, 255.0f, 255.0f),
        _glowMaxShieldColor(0.0f, 0.0f, 255.0f),
        _glowMinShieldColor(255.0f, 255.0f, 255.0f),
        _glowMaxHealthColor(0.0f, 255.0f, 0.0f),
        _glowMinHealthColor(255.0f, 0.0f, 255.0f),
        _drawHitboxPosition(false),
        _hitboxRangeInMeters(100) {}
    
    EspSettings(bool glow, float glowRangeInMeters, bool drawHitboxPosition, float hitboxRangeInMeters) :
        _glowEnabled(glow),
        _glowType(GlowType::DynamicColorGlow),
        _glowRangeInMeters(glowRangeInMeters),
        _glowStaticColor(255.0f, 255.0f, 255.0f),
        _glowMaxShieldColor(0.0f, 0.0f, 255.0f),
        _glowMinShieldColor(255.0f, 255.0f, 255.0f),
        _glowMaxHealthColor(0.0f, 255.0f, 0.0f),
        _glowMinHealthColor(255.0f, 0.0f, 255.0f),
        _drawHitboxPosition(drawHitboxPosition),
        _hitboxRangeInMeters(hitboxRangeInMeters) {}

    bool isGlowEnabled() const {
        return _glowEnabled;
    }

    float glowRangeInMeters() const {
        return _glowRangeInMeters;
    }

    GlowType getGlowType() const {
        return _glowType;
    }

    Color getGlowStaticColor() const {
        return _glowStaticColor;
    }

    Color getGlowMaxShieldColor() const {
        return _glowMaxShieldColor;
    }

    Color getGlowMinShieldColor() const {
        return _glowMinShieldColor;
    }

    Color getGlowMaxHealthColor() const {
        return _glowMaxHealthColor;
    }

    Color getGlowMinHealthColor() const {
        return _glowMinHealthColor;
    }
     
    bool isDrawHitboxPositionEnabled() const {
        return _drawHitboxPosition;
    }

    float hitboxRangeInMeters() const {
        return _hitboxRangeInMeters;
    }

    void render() {
        if(ImGui::BeginTabItem("ESP Settings")) {

            ImGui::Checkbox("Glow##ESP", &_glowEnabled);
            Renderer::renderImguiFloatValue("Glow range in meters", "ESP", &_glowRangeInMeters, 25.0f, 10000.0f, 1.0f, 50.0f);

            if(_glowType == GlowType::DynamicColorGlow) {
                Renderer::renderImguiColorValue("Glow max shield color", "ESP", _glowMaxShieldColor);
                Renderer::renderImguiColorValue("Glow min shield color", "ESP", _glowMinShieldColor);
                Renderer::renderImguiColorValue("Glow max health color", "ESP", _glowMaxHealthColor);
                Renderer::renderImguiColorValue("Glow min health color", "ESP", _glowMinHealthColor);
            }
            else {
                Renderer::renderImguiColorValue("Glow color", "ESP", _glowStaticColor);
            }

            ImGui::Checkbox("Hitbox Position##ESP", &_drawHitboxPosition);
            Renderer::renderImguiFloatValue("Hitbox range in meters", "ESP", &_hitboxRangeInMeters, 25.0f, 500.0f, 1.0f, 25.0f);

            ImGui::EndTabItem();
        }
    }

    void load(const SettingsContext& settingsContext) {
        if(!settingsContext.loadBool(glowEnabledId, _glowEnabled)) {
            _glowEnabled = true;
        }

        int glowTypeValue = 0;
        if(settingsContext.loadInt(glowTypeId, glowTypeValue)) {
            _glowType = static_cast<GlowType>(glowTypeValue);
        }
        else {
            _glowType = GlowType::DynamicColorGlow;
        }

        if(!settingsContext.loadFloat(glowRangeInMetersId, _glowRangeInMeters)) {
            _glowRangeInMeters = 1000;
        }

        if(!settingsContext.loadVector(glowStaticColorId, (float*)&_glowStaticColor, Color::size)) {
            _glowStaticColor = Color(255.0f, 255.0f, 255.0f);
        }

        if(!settingsContext.loadVector(glowMaxShieldColorId, (float*)&_glowMaxShieldColor, Color::size)) {
            _glowMaxShieldColor = Color(0.0f, 0.0f, 255.0f);
        }

        if(!settingsContext.loadVector(glowMinShieldColorId, (float*)&_glowMinShieldColor, Color::size)) {
            _glowMinShieldColor = Color(255.0f, 255.0f, 255.0f);
        }

        if(!settingsContext.loadVector(glowMaxHealthColorId, (float*)&_glowMaxHealthColor, Color::size)) {
            _glowMaxHealthColor = Color(0.0f, 255.0f, 0.0f);
        }

        if(!settingsContext.loadVector(glowMinHealthColorId, (float*)&_glowMinHealthColor, Color::size)) {
            _glowMinHealthColor = Color(255.0f, 0.0f, 0.0f);
        }

        if(!settingsContext.loadBool(drawHitboxPositionId, _drawHitboxPosition)) {
            _drawHitboxPosition = false;
        }

        if(!settingsContext.loadFloat(hitboxRangeInMetersId, _hitboxRangeInMeters), _hitboxRangeInMeters) {
            _hitboxRangeInMeters = 100.0f;
        }
    }

    void save(SettingsContext& settingsContext) const {
        settingsContext.set(glowEnabledId, _glowEnabled);
        settingsContext.set(glowTypeId, static_cast<int>(_glowType));
        settingsContext.set(glowRangeInMetersId, _glowRangeInMeters);
        settingsContext.set(glowStaticColorId, (float*)&_glowStaticColor, Color::size);
        settingsContext.set(glowMaxShieldColorId, (float*)&_glowMaxShieldColor, Color::size);
        settingsContext.set(glowMinShieldColorId, (float*)&_glowMinShieldColor, Color::size);
        settingsContext.set(glowMaxHealthColorId, (float*)&_glowMaxHealthColor, Color::size);
        settingsContext.set(glowMinHealthColorId, (float*)&_glowMinHealthColor, Color::size);

        settingsContext.set(drawHitboxPositionId, _drawHitboxPosition);
        settingsContext.set(hitboxRangeInMetersId, _hitboxRangeInMeters);
    }
};

const std::string EspSettings::glowEnabledId = "esp.glowEnabled";
const std::string EspSettings::glowTypeId = "esp.glowType";
const std::string EspSettings::glowRangeInMetersId = "esp.glowRangeInMeters";
const std::string EspSettings::glowStaticColorId = "esp.glowStaticColor";
const std::string EspSettings::glowMaxShieldColorId = "esp.glowMaxShieldColor";
const std::string EspSettings::glowMinShieldColorId = "esp.glowMinShieldColor";
const std::string EspSettings::glowMaxHealthColorId = "esp.glowMaxHealthColor";
const std::string EspSettings::glowMinHealthColorId = "esp.glowMinHealthColor";

const std::string EspSettings::drawHitboxPositionId = "esp.drawHitboxPosition";
const std::string EspSettings::hitboxRangeInMetersId = "esp.hitboxRangeInMeters";