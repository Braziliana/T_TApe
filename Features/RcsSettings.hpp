#pragma once
#include "../imgui/imgui.h"

struct RcsSettings
{
private:
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

            ImGui::Checkbox("Enabled", &_enabled);

            ImGui::SliderFloat("Vertical Power##RCS", &_verticalPower, 0.0f, 1.0f, "%.2f");
            ImGui::SameLine();
            ImGui::PushItemWidth(100); 
            ImGui::InputFloat("##RCS Vertical Power1", &_verticalPower, 0.01f, 0.1f);
            ImGui::PopItemWidth(); 

            ImGui::SliderFloat("Horizontal Power##RCS", &_horizontalPower, 0.0f, 1.0f, "%.2f");
            ImGui::SameLine();
            ImGui::PushItemWidth(100); 
            ImGui::InputFloat("##RCS Horizontal Power 1", &_horizontalPower, 0.01f, 0.1f);
            ImGui::PopItemWidth(); 

            ImGui::EndTabItem();
        }
    }
};