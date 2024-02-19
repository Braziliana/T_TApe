#pragma once
#include "../imgui/imgui.h"
#include "../Drawings/Renderer.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

struct RcsSettings
{
private:
    bool enabled;
    float rcsPitchPower;
    float rcsYawPower;

public:
    RcsSettings()
    {
        enabled = true;
        rcsPitchPower = 0.7f;
        rcsYawPower = 0.7f;
    }

    inline bool IsEnabled() const {
        return enabled;
    }

    inline float GetRCSPitchPower() const {
        return rcsPitchPower;
    }

    inline float GetRCSYawPower() const {
        return rcsYawPower;
    }

    void Render() {
        ImGui::Checkbox("Enabled##Rcs", &enabled);
        Renderer::renderImguiFloatValue("RCS Vertical power", "Rcs", &rcsPitchPower, 0.0f, 1.0f, 0.01f, 0.1f);
        Renderer::renderImguiFloatValue("RCS Horizontal power", "Rcs", &rcsYawPower, 0.0f, 1.0f, 0.01f, 0.1f);
    }

    void Load(const json& j)
    {
        enabled = j.at("enabled").get<bool>();
        rcsPitchPower = j.at("rcsPitchPower").get<float>();
        rcsYawPower = j.at("rcsYawPower").get<float>();
    }

    json ToJson() const {
        return {
            {"enabled", enabled},
            {"rcsPitchPower", rcsPitchPower},
            {"rcsYawPower", rcsYawPower},
        };
    }
};

typedef RcsSettings* PRcsSettings;
