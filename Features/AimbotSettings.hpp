#pragma once
#include "../imgui/imgui.h"
#include "../Utils/SettingsContext.hpp"
#include "../RenderEngine/Renderer.hpp"
#include "../Game/Enums/HitboxType.hpp"
#include "../Math/QAngle.hpp"
#include "../Utils/InputTypes.hpp"

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
    static const std::string useHotkeyId;
    static const std::string aimHotkeyId;
    static const std::string predictMovementEnabledId;
    static const std::string predictBulletDropEnabledId;
    static const std::string allowForTargetSwitchId;
    static const std::string addRandomOffsetEnabledId;
    static const std::string randomOffsetValueId;

    static const std::string randomWriteDelayValueId;

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
    bool _useHotkey;
    InputKeyType _aimHotkey;
    bool _predictMovementEnabled;
    bool _predictBulletDropEnabled;
    bool _allowForTargetSwitch;
    bool _addRandomOffsetEnabled;
    float _randomOffsetValue;
    float _randomWriteDelayValue;

public:
    AimbotSettings() : _enabled(false),
        _rage(false),
        _verticalPower(1.0f),
        _horizontalPower(1.0f),
        _speed(5.0f),
        _maxAngleChangePerTick(0.25f),
        _rangeInMeters(250.0f),
        _hitbox(HitboxType::UpperChest),
        _fieldOfView(45.0f),
        _angleSmoothType(AngleSmoothType::LerpSmoothing),
        _useHotkey(false),
        _aimHotkey(InputKeyType::MOUSE_X1),
        _predictMovementEnabled(false),
        _predictBulletDropEnabled(false),
        _allowForTargetSwitch(true),
        _addRandomOffsetEnabled(true),
        _randomOffsetValue(0.03f) {}

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

    bool useHotkey() const {
        return _useHotkey;
    }

    InputKeyType getAimHotkey() const {
        return _aimHotkey;
    }

    bool predictMovementEnabled() const {
        return _predictMovementEnabled;
    }

    bool predictBulletDropEnabled() const {
        return _predictBulletDropEnabled;
    }

    bool allowForTargetSwitch() const {
        return _allowForTargetSwitch;
    }

    bool addRandomOffsetEnabled() const {
        return _addRandomOffsetEnabled;
    }

    float getRandomOffsetValue() const {
        return _randomOffsetValue;
    }

    float getRandomWriteDelayValue() const {
        return _randomWriteDelayValue;
    }

    void render()  {
        if(ImGui::BeginTabItem("Aimbot Settings")) {

            ImGui::Checkbox("Enabled##Aimbot", &_enabled);
            ImGui::Checkbox("Rage##Aimbot", &_rage);

            ImGui::Spacing();

            const char* hitboxTypes[] = { "Head", "Neck", "Upper chest", "Lower chest", "Stomach", "Hip" };
            int hitboxTypeIndex = static_cast<int>(_hitbox);
            ImGui::Combo("Hitbox type##Aimbot", &hitboxTypeIndex, hitboxTypes, IM_ARRAYSIZE(hitboxTypes));
            _hitbox = static_cast<HitboxType>(hitboxTypeIndex);

            ImGui::Spacing();

            ImGui::Checkbox("Use hotkey##Aimbot", &_useHotkey);

            int aimHotkey = static_cast<int>(_aimHotkey);
            ImGui::Combo("Aim hotkey##Aimbot", &aimHotkey, InputKeyTypeNames, IM_ARRAYSIZE(InputKeyTypeNames));
            _aimHotkey = static_cast<InputKeyType>(aimHotkey);

            if(!_rage)
            {
                ImGui::Checkbox("Allow for target switch##Aimbot", &_allowForTargetSwitch);

                Renderer::renderImguiFloatValue("Write delay value", "Aimbot", &_randomWriteDelayValue, 0.0f, 0.5f, 0.001f, 0.01f);
                ImGui::Spacing();

                Renderer::renderImguiFloatValue("Vertical power", "Aimbot", &_verticalPower, 0.0f, 1.0f, 0.01f, 0.1f);
                Renderer::renderImguiFloatValue("Horizontal power", "Aimbot", &_horizontalPower, 0.0f, 1.0f, 0.01f, 0.1f);
                ImGui::Spacing();

                Renderer::renderImguiFloatValue("Speed", "Aimbot", &_speed, 0.0f, 1000.0f, 0.1f, 5.0f);
                Renderer::renderImguiFloatValue("Max angle change per tick", "Aimbot", &_maxAngleChangePerTick, 0.01f, 10.0f, 0.01f, 0.1f);
                ImGui::Spacing();

                Renderer::renderImguiFloatValue("Range In Meters", "Aimbot", &_rangeInMeters, 10.0f, 2000.0f, 1.0f, 10.0f);
                Renderer::renderImguiFloatValue("Field of view", "Aimbot", &_fieldOfView, 1.0f, 360.0f, 1.0f, 5.0f);
                ImGui::Spacing();

                // const char* angleSmoothTypeNames[] = {
                //     "Lerp smoothing",
                //     "Linear smoothing",
                //     "Exponential smoothing",
                //     "S-Curve smoothing",
                //     "Bezier smoothing",
                //     "Acceleration smoothing",
                //     "Jerk-Limited smoothing"
                // };

                // int angleSmoothTypeIndex = static_cast<int>(_angleSmoothType);
                // if (ImGui::Combo("Angle smooth type##Aimbot", &angleSmoothTypeIndex, angleSmoothTypeNames, IM_ARRAYSIZE(angleSmoothTypeNames))) {
                //     _angleSmoothType = static_cast<AngleSmoothType>(angleSmoothTypeIndex);
                // }

                ImGui::Checkbox("Predict movement##Aimbot", &_predictMovementEnabled);
                ImGui::Checkbox("Predict bullet drop##Aimbot", &_predictBulletDropEnabled);

                ImGui::Spacing();

                ImGui::Checkbox("Add random offset to aim angle##Aimbot", &_addRandomOffsetEnabled);
                if(_addRandomOffsetEnabled) {
                    Renderer::renderImguiFloatValue("Random offset value", "Aimbot", &_randomOffsetValue, 0.0f, 5.0f, 0.01f, 0.1f);
                }
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

        if(!settingsContext.loadBool(useHotkeyId, _useHotkey)) {
            _useHotkey = false;
        }

        int aimHotkey = 0;
        if(settingsContext.loadInt(aimHotkeyId, aimHotkey)) {
            _aimHotkey = static_cast<InputKeyType>(aimHotkey);
        }
        else {
            _aimHotkey = InputKeyType::INPUT_UNKNOWN;
        }

        if(!settingsContext.loadBool(predictMovementEnabledId, _predictMovementEnabled)) {
            _predictMovementEnabled = true;
        }

        if(!settingsContext.loadBool(predictBulletDropEnabledId, _predictBulletDropEnabled)) {
            _predictBulletDropEnabled = false;
        }

        if(!settingsContext.loadBool(allowForTargetSwitchId, _allowForTargetSwitch)) {
            _allowForTargetSwitch = false;
        }

        if(!settingsContext.loadBool(addRandomOffsetEnabledId, _addRandomOffsetEnabled)) {
            _addRandomOffsetEnabled = false;
        }

        if(!settingsContext.loadFloat(randomOffsetValueId, _randomOffsetValue)) {
            _randomOffsetValue = 0.03f;
        }

        if(!settingsContext.loadFloat(randomWriteDelayValueId, _randomWriteDelayValue)) {
            _randomWriteDelayValue = 0.00f;
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
        settingsContext.set(useHotkeyId, _useHotkey);
        settingsContext.set(aimHotkeyId, static_cast<int>(_aimHotkey));
        settingsContext.set(predictMovementEnabledId, _predictMovementEnabled);
        settingsContext.set(predictBulletDropEnabledId, _predictBulletDropEnabled);
        settingsContext.set(allowForTargetSwitchId, _allowForTargetSwitch);
        settingsContext.set(addRandomOffsetEnabledId, _addRandomOffsetEnabled);
        settingsContext.set(randomOffsetValueId, _randomOffsetValue);
        settingsContext.set(randomWriteDelayValueId, _randomWriteDelayValue);
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
const std::string AimbotSettings::useHotkeyId = "aimbot.useHotkey";
const std::string AimbotSettings::aimHotkeyId = "aimbot.aimHotkey";
const std::string AimbotSettings::predictMovementEnabledId = "aimbot.predictMovementEnabled";
const std::string AimbotSettings::predictBulletDropEnabledId = "aimbot.predictBulletDropEnabled";
const std::string AimbotSettings::allowForTargetSwitchId = "aimbot.allowForTargetSwitch";
const std::string AimbotSettings::addRandomOffsetEnabledId = "aimbot.addRandomOffsetEnabled";
const std::string AimbotSettings::randomOffsetValueId = "aimbot.randomOffsetValue";
const std::string AimbotSettings::randomWriteDelayValueId = "aimbot.randomWriteDelayValue";