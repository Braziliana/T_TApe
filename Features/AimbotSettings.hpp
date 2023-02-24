#pragma once
#include "../imgui/imgui.h"
#include "../Utils/SettingsContext.hpp"
#include "../RenderEngine/Renderer.hpp"
#include "../Game/Enums/HitboxType.hpp"
#include "../Math/QAngle.hpp"

struct AimbotSettings
{
private:
    static const std::string enabledId;
    static const std::string rageId;
    static const std::string verticalPowerId;
    static const std::string horizontalPowerId;
    static const std::string speedId;
    static const std::string maxAngleChangePerTickId;
    static const std::string rangeInMetersId;
    static const std::string hitboxId;
    static const std::string fieldOfViewId;
    static const std::string angleSmoothTypeId;

    bool _enabled;
    bool _rage;
    float _verticalPower;
    float _horizontalPower;
    float _speed;
    float _maxAngleChangePerTick;
    float _rangeInMeters;
    HitboxType _hitbox;
    float _fieldOfView;
    AngleSmoothType _angleSmoothType;

public:
    AimbotSettings() : _enabled(false),
        _rage(false),
        _verticalPower(1.0f),
        _horizontalPower(1.0f),
        _speed(5.0f),
        _maxAngleChangePerTick(0.25f),
        _rangeInMeters(250.0f),
        _hitbox(HitboxType::UpperChest),
        _fieldOfView(45.0f) {}
    
    AimbotSettings(bool enabled, bool rage, float verticalPower, float horizontalPower, float speed, float maxAngleChangePerTick, float rangeInMeters, HitboxType hitbox, float fieldOfView) : 
        _enabled(enabled),
        _rage(rage),
        _verticalPower(verticalPower),
        _horizontalPower(horizontalPower),
        _speed(speed),
        _maxAngleChangePerTick(maxAngleChangePerTick),
        _rangeInMeters(rangeInMeters),
        _hitbox(hitbox),
        _fieldOfView(fieldOfView) {}

    bool isEnabled() const {
        return _enabled;
    }

    bool isRage() const {
        return _rage;
    }

    float getVerticalPower() const {
        return _verticalPower;
    }

    float getHorizontalPower() const {
        return _horizontalPower;
    }

    float getSpeed() const {
        return _speed;
    }

    float getMaxAngleChangePerTick() const {
        return _maxAngleChangePerTick;
    }

    float getRangeInMeters() const {
        return _rangeInMeters;
    }

    HitboxType getHitbox() const {
        return _hitbox;
    }

    float getFieldOfView() const {
        return _fieldOfView;
    }

    AngleSmoothType getAngleSmoothType() const {
        return _angleSmoothType;
    }

    void render()  {
        if(ImGui::BeginTabItem("Aimbot Settings")) {

            ImGui::Checkbox("Enabled##Aimbot", &_enabled);
            ImGui::Checkbox("Rage##Aimbot", &_rage);

            Renderer::renderImguiFloatValue("Vertical power", "Aimbot", &_verticalPower, 0.0f, 1.0f, 0.01f, 0.1f);
            Renderer::renderImguiFloatValue("Horizontal power", "Aimbot", &_horizontalPower, 0.0f, 1.0f, 0.01f, 0.1f);
            Renderer::renderImguiFloatValue("Speed", "Aimbot", &_speed, 0.0f, 1000.0f, 0.1f, 5.0f);
            Renderer::renderImguiFloatValue("Max angle change per tick", "Aimbot", &_maxAngleChangePerTick, 0.01f, 10.0f, 0.01f, 0.1f);
            Renderer::renderImguiFloatValue("Range In Meters", "Aimbot", &_rangeInMeters, 10.0f, 2000.0f, 1.0f, 10.0f);
            Renderer::renderImguiFloatValue("Field of view", "Aimbot", &_fieldOfView, 1.0f, 360.0f, 1.0f, 5.0f);

            const char* hitboxTypes[] = { "Head", "Neck", "Upper chest", "Lower chest", "Stomach", "Hip" };
            int hitboxTypeIndex = static_cast<int>(_hitbox);
            ImGui::Combo("Hitbox type##Aimbot", &hitboxTypeIndex, hitboxTypes, IM_ARRAYSIZE(hitboxTypes));
            _hitbox = static_cast<HitboxType>(hitboxTypeIndex);

            const char* angleSmoothTypeNames[] = {
                "Lerp smoothing",
                "Linear smoothing",
                "Exponential smoothing",
                "S-Curve smoothing",
                "Bezier smoothing",
                "Acceleration smoothing",
                "Jerk-Limited smoothing"
            };

            int angleSmoothTypeIndex = static_cast<int>(_angleSmoothType);
            if (ImGui::Combo("Angle smooth type##Aimbot", &angleSmoothTypeIndex, angleSmoothTypeNames, IM_ARRAYSIZE(angleSmoothTypeNames))) {
                _angleSmoothType = static_cast<AngleSmoothType>(angleSmoothTypeIndex);
            }

            ImGui::EndTabItem();
        }
    }

    void load(const SettingsContext& settingsContext) {
        if(!settingsContext.loadBool(enabledId, _enabled)) {
            _enabled = false;
        }

        if(!settingsContext.loadBool(rageId,_rage)) {
            _rage = false;
        }

        if(!settingsContext.loadFloat(verticalPowerId, _verticalPower)) {
            _verticalPower = 1.0f;
        }

        if(!settingsContext.loadFloat(horizontalPowerId, _horizontalPower)) {
            _horizontalPower = 1.0f;
        }

        if(!settingsContext.loadFloat(speedId, _speed)) {
            _speed = 5.0f;
        }

        if(!settingsContext.loadFloat(maxAngleChangePerTickId, _maxAngleChangePerTick)) {
            _maxAngleChangePerTick = 0.25f;
        }

        if(!settingsContext.loadFloat(rangeInMetersId, _rangeInMeters)) {
            _rangeInMeters = 250.0f;
        }

        int hitboxValue = 0;
        if(settingsContext.loadInt(hitboxId, hitboxValue)) {
            _hitbox = static_cast<HitboxType>(hitboxValue);
        }
        else {
            _hitbox = HitboxType::UpperChest;
        }

        if(!settingsContext.loadFloat(fieldOfViewId, _fieldOfView)) {
            _fieldOfView = 45.0f;
        }

        int angleSmoothTypeValue = 0;
        if(settingsContext.loadInt(angleSmoothTypeId, angleSmoothTypeValue)) {
            _angleSmoothType = static_cast<AngleSmoothType>(angleSmoothTypeValue);
        }
        else {
            _angleSmoothType = AngleSmoothType::LerpSmoothing;
        }
    }

    void save(SettingsContext& settingsContext) const {
        settingsContext.set(enabledId, _enabled);
        settingsContext.set(rageId, _rage);
        settingsContext.set(verticalPowerId, _verticalPower);
        settingsContext.set(horizontalPowerId, _horizontalPower);
        settingsContext.set(speedId, _speed);
        settingsContext.set(maxAngleChangePerTickId, _maxAngleChangePerTick);
        settingsContext.set(rangeInMetersId, _rangeInMeters);
        settingsContext.set(hitboxId, static_cast<int>(_hitbox));
        settingsContext.set(fieldOfViewId, _fieldOfView);
        settingsContext.set(angleSmoothTypeId, static_cast<int>(_angleSmoothType));
    }
};

const std::string AimbotSettings::enabledId = "aimbot.enabled";
const std::string AimbotSettings::rageId = "aimbot.rage";
const std::string AimbotSettings::verticalPowerId = "aimbot.verticalPower";
const std::string AimbotSettings::horizontalPowerId = "aimbot.horizontalPower";
const std::string AimbotSettings::speedId = "aimbot.speed";
const std::string AimbotSettings::maxAngleChangePerTickId = "aimbot.maxAngleChangePerTick";
const std::string AimbotSettings::rangeInMetersId = "aimbot.rangeInMeters";
const std::string AimbotSettings::hitboxId = "aimbot.hitbox";
const std::string AimbotSettings::fieldOfViewId = "aimbot.fieldOfView";
const std::string AimbotSettings::angleSmoothTypeId = "aimbot.angleSmoothType";