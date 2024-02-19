#pragma once
#include "../Math/Color.h"
#include "../imgui/imgui.h"
#include "../Drawings/Renderer.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

struct EspSettings
{
private:
    bool espEnabled;
    bool espShowDistance;
    float espDistance;
    Math::Color espEnemyColor;

    bool bonesEspEnabled;
    float bonesEspDistance;
    Math::Color bonesEspEnemyColor;

    bool healthEspEnabled;
    float healthEspDistance;

public:
    EspSettings()
    {
        espEnabled = true;
        espShowDistance = true;
        espDistance = 100.0f;
        espEnemyColor = Math::Color(1.0f, 0.0f, 0.0f);

        bonesEspEnabled = true;
        bonesEspDistance = 100.0f;
        bonesEspEnemyColor = Math::Color(1.0f, 1.0f, 1.0f);

        healthEspEnabled = true;
        healthEspDistance = 100.0f;
    }

    inline bool IsEspEnabled() const {
        return espEnabled;
    }

    inline bool IsEspDistanceEnabled() const {
        return espShowDistance;
    }

    inline float GetEspDistance() const {
        return espDistance;
    }
    
    inline Math::Color GetEspEnemyColor() const {
        return espEnemyColor;
    }

    inline bool IsBonesEspEnabled() const {
        return bonesEspEnabled;
    }

    inline float GetBonesEspDistance() const {
        return bonesEspDistance;
    }
    
    inline Math::Color GetBonesEspEnemyColor() const {
        return bonesEspEnemyColor;
    }

    inline bool IsHealthEspEnabled() const {
        return healthEspEnabled;
    }

    inline float GetHealthEspDistance() const {
        return healthEspDistance;
    }

    void Render() {
        ImGui::Checkbox("Esp enabled##Esp", &espEnabled);
        ImGui::Checkbox("Esp show distance##Esp", &espShowDistance);
        Renderer::renderImguiFloatValue("Esp distance", "Esp", &espDistance, 0.0f, 1000.0f, 5.0f, 25.0f);
        Renderer::renderImguiColorValue("Esp enemy color", "Esp", espEnemyColor);

        ImGui::Spacing();

        ImGui::Checkbox("Bone esp enabled##Esp", &bonesEspEnabled);
        Renderer::renderImguiFloatValue("Bone esp distance", "Esp", &bonesEspDistance, 0.0f, 1000.0f, 5.0f, 25.0f);
        Renderer::renderImguiColorValue("Bone esp enemy color", "Esp", bonesEspEnemyColor);

        ImGui::Spacing();

        ImGui::Checkbox("Health esp enabled##Esp", &healthEspEnabled);
        Renderer::renderImguiFloatValue("Health esp distance", "Esp", &healthEspDistance, 0.0f, 1000.0f, 5.0f, 25.0f);
    }


    void Load(const json& j)
    {
        espEnabled = j.at("espEnabled").get<bool>();
        espShowDistance = j.at("espShowDistance").get<bool>();
        espDistance = j.at("espDistance").get<float>();
        espEnemyColor = Math::Color(
            j.at("espEnemyColor").at("r").get<float>(),
            j.at("espEnemyColor").at("g").get<float>(),
            j.at("espEnemyColor").at("b").get<float>()
        );

        bonesEspEnabled = j.at("bonesEspEnabled").get<bool>();
        bonesEspDistance = j.at("bonesEspDistance").get<float>();
        bonesEspEnemyColor = Math::Color(
            j.at("bonesEspEnemyColor").at("r").get<float>(),
            j.at("bonesEspEnemyColor").at("g").get<float>(),
            j.at("bonesEspEnemyColor").at("b").get<float>()
        );

        healthEspEnabled = j.at("healthEspEnabled").get<bool>();
        healthEspDistance = j.at("healthEspDistance").get<float>();
    }

    json ToJson() const {
        return {
            {"espEnabled", espEnabled},
            {"espShowDistance", espShowDistance},
            {"espDistance", espDistance},
            {"espEnemyColor", {{"r", espEnemyColor.r}, {"g", espEnemyColor.g}, {"b", espEnemyColor.b}}},

            {"bonesEspEnabled", bonesEspEnabled},
            {"bonesEspDistance", bonesEspDistance},
            {"bonesEspEnemyColor", {{"r", bonesEspEnemyColor.r}, {"g", bonesEspEnemyColor.g}, {"b", bonesEspEnemyColor.b}}},

            {"healthEspEnabled", healthEspEnabled},
            {"healthEspDistance", healthEspDistance},
        };
    }
};

typedef EspSettings* PEspSettings;
