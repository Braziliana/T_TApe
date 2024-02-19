#pragma once
#include "../Math/Color.h"
#include "../imgui/imgui.h"
#include "../Drawings/Renderer.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

struct GlowSettings
{
private:
    bool enabled;
    float glowDistance;
    Math::Color enemyColor;
    Math::Color visibleEnemyColor;
    Math::Color knockedColor;

public:
    GlowSettings()
    {
        enabled = true;
        glowDistance = 500.0f;
        enemyColor = Math::Color(1.0f, 0.0f, 0.0f);
        visibleEnemyColor = Math::Color(0.0f, 1.0f, 0.0f);
        knockedColor = Math::Color(0.5f, 0.5f, 0.5f);
    }

    inline bool IsEnabled() const {
        return enabled;
    }

    inline float GetGlowDistance() const {
        return glowDistance;
    }
    
    inline Math::Color GetEnemyColor() const {
        return enemyColor;
    }

    inline Math::Color GetVisibleEnemyColor() const {
        return visibleEnemyColor;
    }

    inline Math::Color GetKnockedColor() const {
        return knockedColor;
    }

    void Render() {
        ImGui::Checkbox("Enabled##Glow", &enabled);
        Renderer::renderImguiFloatValue("Distance", "Glow", &glowDistance, 0.0f, 1000.0f, 5.0f, 25.0f);
        Renderer::renderImguiColorValue("Enemy color", "Glow", enemyColor);
        Renderer::renderImguiColorValue("Enemy visible color", "Glow", visibleEnemyColor);
        Renderer::renderImguiColorValue("Enemy knocked color", "Glow", knockedColor);
    }


    void Load(const json& j)
    {
        enabled = j.at("enabled").get<bool>();
        glowDistance = j.at("glowDistance").get<float>();
        enemyColor = Math::Color(
            j.at("enemyColor").at("r").get<float>(),
            j.at("enemyColor").at("g").get<float>(),
            j.at("enemyColor").at("b").get<float>()
        );
        visibleEnemyColor = Math::Color(
            j.at("visibleEnemyColor").at("r").get<float>(),
            j.at("visibleEnemyColor").at("g").get<float>(),
            j.at("visibleEnemyColor").at("b").get<float>()
        );
        knockedColor = Math::Color(
            j.at("knockedColor").at("r").get<float>(),
            j.at("knockedColor").at("g").get<float>(),
            j.at("knockedColor").at("b").get<float>()
        );
    }

    json ToJson() const {
        return {
            {"enabled", enabled},
            {"glowDistance", glowDistance},
            {"enemyColor", {{"r", enemyColor.r}, {"g", enemyColor.g}, {"b", enemyColor.b}}},
            {"visibleEnemyColor", {{"r", visibleEnemyColor.r}, {"g", visibleEnemyColor.g}, {"b", visibleEnemyColor.b}}},
            {"knockedColor", {{"r", knockedColor.r}, {"g", knockedColor.g}, {"b", knockedColor.b}}},
        };
    }
};

typedef GlowSettings* PGlowSettings;
