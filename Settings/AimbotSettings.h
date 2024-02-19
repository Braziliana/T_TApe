#pragma once
#include "../imgui/imgui.h"

#include "../SDK/HitboxType.h"
#include "../SDK/ButtonType.h"
#include "../Misc/InputTypes.h"
#include "../Drawings/Renderer.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

struct AimbotSettings
{
private:
    bool enabled;
    float fov;
    float ignoreFovCheckDistance;
    float distance;
    SDK::HitboxType hitbox;

    bool smoothEnabled;
    float smoofing;

    float smoofingMinSpeed;
    float smoofingMaxSpeed;


    bool rcsEnabled;
    float rcsPitchPower;
    float rcsYawPower;

    int aimbotKey;

    bool predictionEnabled;

    bool singleTargetPerAimUse;
    float switchTargetDelay;

    bool aimAssist;
    float aimTime;
    float aimBreak;

    bool aimActiveOnlyWhenShooting;

public:
    AimbotSettings()
    {
        enabled = true;
        fov = 25.0f;
        ignoreFovCheckDistance = 10.0f;
        distance = 250.0f;
        hitbox = SDK::HitboxType::UpperChest;

        rcsEnabled = true;
        rcsPitchPower = 0.7f;
        rcsYawPower = 0.7f;

        aimbotKey = (int)SDK::ButtonType::MOUSE_5;

        smoothEnabled = false;
        smoofing = 5.0f;
        smoofingMinSpeed = 5.0f;
        smoofingMaxSpeed = 15.0f;

        predictionEnabled = false;

        singleTargetPerAimUse = false;
        switchTargetDelay = 0.0f;

        aimAssist = false;
        aimTime = 0.5f;
        aimBreak = 0.5f;

        aimActiveOnlyWhenShooting = false;
    }

    
		inline bool IsEnabled() const {
			return enabled;
		}

		inline float GetFov() const {
			return fov;
		}

		inline float GetIgnoreFovCheckDistance() const {
			return ignoreFovCheckDistance;
		}

		inline float GetDistance() const {
			return distance;
		}

		inline SDK::HitboxType GetHitbox() const {
			return hitbox;
		}

		inline bool IsRcsEnabled() const {
			return rcsEnabled;
		}

		inline float GetRCSPitchPower() const {
			return rcsPitchPower;
		}

		inline float GetRCSYawPower() const {
			return rcsYawPower;
		}

		inline int GetAimbotKey() const {
			return aimbotKey;
		}

		inline bool IsSmoothEnabled() const {
			return smoothEnabled;
		}

		inline float GetSmoofing() const {
			return smoofing;
		}

		inline float GetSmoofingMinSpeed() const {
			return smoofingMinSpeed;
		}

		inline float GetSmoofingMaxSpeed() const {
			return smoofingMaxSpeed;
		}

		inline bool IsPredictionEnabled() const {
			return predictionEnabled;
		}

		inline bool IsSingleTargetPerAimUse() const {
			return singleTargetPerAimUse;
		}

		inline float GetswitchTargetDelay() const {
			return switchTargetDelay;
		}

		inline bool IsAimAssistEnabled() const {
			return aimAssist;
		}

		inline float GetAimTime() const {
			return aimTime;
		}

		inline float GetAimBreak() const {
			return aimBreak;
		}

		inline bool IsAimActiveOnlyWhenShooting() const {
			return aimActiveOnlyWhenShooting;
		}

    void Render() {

        /*
        aimbotKey = (int)SDK::ButtonType::MOUSE_5;
        */

        ImGui::Checkbox("Enabled##Aimbot", &enabled);
        ImGui::Checkbox("Active only when shooting##Aimbot", &aimActiveOnlyWhenShooting);
        ImGui::Checkbox("Prediction enabled##Aimbot", &predictionEnabled);
        ImGui::Checkbox("Single target per use##Aimbot", &singleTargetPerAimUse);
        Renderer::renderImguiFloatValue("Switch target delay", "Aimbot", &switchTargetDelay, 0.0f, 2.0f, 0.01f, 0.1f);

        //            int aimHotkey = static_cast<int>(aimHotkey);
        //            ImGui::Combo("Aim hotkey##Aimbot", &aimHotkey, InputKeyTypeNames, IM_ARRAYSIZE(InputKeyTypeNames));
        //            aimHotkey = static_cast<InputKeyType>(aimHotkey);

        int hitboxId = static_cast<int>(hitbox);

        const char* hitboxeTypeNames[] = {
            "Head",
            "Neck",
            "UpperChest",
            "LowerChest",
            "Stomach",
            "Hip"
        };

        ImGui::Combo("Hitbox##Aimbot", &hitboxId, hitboxeTypeNames, IM_ARRAYSIZE(hitboxeTypeNames));
        hitbox = static_cast<SDK::HitboxType>(hitboxId);

        ImGui::Spacing();

        ImGui::Checkbox("Aim assist##Aimbot", &aimAssist);
        Renderer::renderImguiFloatValue("Aim time", "Aimbot", &aimTime, 0.0f, 10.0f, 0.01f, 0.1f);
        Renderer::renderImguiFloatValue("Aim break", "Aimbot", &aimBreak, 0.0f, 10.0f, 0.01f, 0.1f);

        ImGui::Spacing();

        Renderer::renderImguiFloatValue("Fov", "Aimbot", &fov, 0.0f, 3000.0f, 5.0f, 25.01f);
        Renderer::renderImguiFloatValue("Ignore Fov check distance", "Aimbot", &ignoreFovCheckDistance, 0.0f, 3000.0f, 5.0f, 25.01f);
        Renderer::renderImguiFloatValue("Distance", "Aimbot", &distance, 0.0f, 500.0f, 1.0f, 10.01f);

        ImGui::Spacing();

        ImGui::Checkbox("Rcs enabled##Aimbot", &rcsEnabled);
        Renderer::renderImguiFloatValue("Rcs vertical power", "Aimbot", &rcsPitchPower, 0.0f, 1.0f, 0.01f, 0.1f);
        Renderer::renderImguiFloatValue("Rcs horizontal power", "Aimbot", &rcsYawPower, 0.0f, 1.0f, 0.01f, 0.1f);

        ImGui::Spacing();

        ImGui::Checkbox("Smooth enabled##Aimbot", &smoothEnabled);
        Renderer::renderImguiFloatValue("Smooth", "Aimbot", &smoofing, 0.0f, 180.0f, 1.0f, 5.0f);
        Renderer::renderImguiFloatValue("Smoofing min speed", "Aimbot", &smoofingMinSpeed, 1.0f, 500.0f, 1.0f, 10.0f);
        Renderer::renderImguiFloatValue("Smoofing max speed", "Aimbot", &smoofingMaxSpeed, 1.0f, 500.0f, 1.0f, 10.0f);
    }

    void Load(const json& j)
    {
        enabled = j.at("enabled").get<bool>();
        fov = j.at("fov").get<float>();
        ignoreFovCheckDistance = j.at("ignoreFovCheckDistance").get<float>();
        hitbox = static_cast<SDK::HitboxType>(j.at("hitbox").get<int>());
        aimbotKey = j.at("aimbotKey").get<int>();
        predictionEnabled = j.at("predictionEnabled").get<bool>();
        singleTargetPerAimUse = j.at("singleTargetPerAimUse").get<bool>();
        switchTargetDelay = j.at("switchTargetDelay").get<float>();
        aimActiveOnlyWhenShooting = j.at("aimActiveOnlyWhenShooting").get<bool>();

        smoothEnabled = j.at("smoothEnabled").get<bool>();
        smoofing = j.at("smoofing").get<float>();
        smoofingMinSpeed = j.at("smoofingMinSpeed").get<float>();
        smoofingMaxSpeed = j.at("smoofingMaxSpeed").get<float>();

        rcsEnabled = j.at("rcsEnabled").get<bool>();
        rcsPitchPower = j.at("rcsPitchPower").get<float>();
        smoofingMaxSpeed = j.at("smoofingMaxSpeed").get<float>();

        aimAssist = j.at("aimAssist").get<bool>();
        aimTime = j.at("aimTime").get<float>();
        aimBreak = j.at("aimBreak").get<float>();
    }

    json ToJson() const {
        return {
            {"enabled", enabled},
            {"fov", fov},
            {"ignoreFovCheckDistance", ignoreFovCheckDistance},
            {"hitbox", static_cast<int>(hitbox)},
            {"aimbotKey", static_cast<int>(aimbotKey)},
            {"predictionEnabled", predictionEnabled},
            {"singleTargetPerAimUse", singleTargetPerAimUse},
            {"switchTargetDelay", switchTargetDelay},
            {"aimActiveOnlyWhenShooting", aimActiveOnlyWhenShooting},

            {"smoothEnabled", smoothEnabled},
            {"smoofing", smoofing},
            {"smoofingMinSpeed", smoofingMinSpeed},
            {"smoofingMaxSpeed", smoofingMaxSpeed},

            {"rcsEnabled", rcsEnabled},
            {"rcsPitchPower", rcsPitchPower},
            {"rcsYawPower", rcsYawPower},

            {"aimAssist", aimAssist},
            {"aimTime", aimTime},
            {"aimBreak", aimBreak},
        };
    }
};

typedef AimbotSettings* PAimbotSettings;