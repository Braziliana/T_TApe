#pragma once
#include "../imgui/imgui.h"

struct EspSettings
{
private:
    bool _glow;
    float _glowRangeInMeters;
    bool _drawHitboxPosition;
    float _hitboxRangeInMeters;

public:
    EspSettings() : _glow(true), _glowRangeInMeters(1000), _drawHitboxPosition(false), _hitboxRangeInMeters(100) {}
    
    EspSettings(bool glow, float glowRangeInMeters, bool drawHitboxPosition, float hitboxRangeInMeters) :
        _glow(glow), _glowRangeInMeters(glowRangeInMeters),
        _drawHitboxPosition(drawHitboxPosition), _hitboxRangeInMeters(hitboxRangeInMeters) {}

    bool isGlowEnabled() const {
        return _glow;
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

            ImGui::Checkbox("Glow", &_glow);
            ImGui::SliderFloat("Glow range in meters", &_glowRangeInMeters, 50.0f, 10000.0f, "%.2f");
            ImGui::SameLine();
            ImGui::PushItemWidth(100); 
            ImGui::InputFloat("", &_glowRangeInMeters, 1.0f, 10.0f);
            ImGui::PopItemWidth(); 


            ImGui::Checkbox("Hitbox Position", &_drawHitboxPosition);
            ImGui::SliderFloat("Hitbox range in meters", &_hitboxRangeInMeters, 5.0f, 500.0f, "%.2f");
            ImGui::SameLine();
            ImGui::PushItemWidth(100); 
            ImGui::InputFloat("", &_hitboxRangeInMeters, 1.0f, 10.0f);
            ImGui::PopItemWidth(); 


            ImGui::EndTabItem();
        }
    }
};