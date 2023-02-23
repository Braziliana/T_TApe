#pragma once
#include "../imgui/imgui.h"
#include "../Utils/SettingsContext.hpp"
#include "../RenderEngine/Renderer.hpp"

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

    bool _enabled;
    bool _rage;
    float _verticalPower;
    float _horizontalPower;
    float _speed;
    float _maxAngleChangePerTick;
    float _rangeInMeters;
    int _hitbox;

public:
    AimbotSettings() : _enabled(false), _rage(false), _verticalPower(1.0f), _horizontalPower(1.0f), _speed(5.0f), _maxAngleChangePerTick(0.25f), _rangeInMeters(250.0f), _hitbox(2) {}
    
    AimbotSettings(bool enabled, bool rage, float verticalPower, float horizontalPower, float speed, float maxAngleChangePerTick, float rangeInMeters, int hitbox)
    {
        _enabled = enabled;
        _verticalPower = verticalPower;
        _horizontalPower = horizontalPower;
        _speed = speed;
        _maxAngleChangePerTick = maxAngleChangePerTick;
        _rangeInMeters = rangeInMeters;
        _hitbox = hitbox;
        _rage = rage;
    }

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

    int getHitbox() const {
        return _hitbox;
    }

    void render()  {
        if(ImGui::BeginTabItem("Aimbot Settings")) {

            ImGui::Checkbox("Enabled", &_enabled);
            ImGui::Checkbox("Rage", &_rage);

            Renderer::renderImguiFloatValue("Vertical power", "Aimbot", &_verticalPower, 0.0f, 1.0f, 0.01f, 0.1f);
            Renderer::renderImguiFloatValue("Horizontal power", "Aimbot", &_horizontalPower, 0.0f, 1.0f, 0.01f, 0.1f);
            Renderer::renderImguiFloatValue("Speed", "Aimbot", &_speed, 0.0f, 1.0f, 0.01f, 0.1f);
            Renderer::renderImguiFloatValue("Max angle change per tick", "Aimbot", &_maxAngleChangePerTick, 0.0f, 1.0f, 0.01f, 0.1f);
            Renderer::renderImguiFloatValue("Range In Meters", "Aimbot", &_rangeInMeters, 0.0f, 1.0f, 0.01f, 0.1f);
            Renderer::renderImguiIntValue("Hitbox", "Aimbot", &_hitbox, 0, 5, 1, 1);

            ImGui::EndTabItem();
        }
    }

    void load(const SettingsContext& settingsContext) {
        _enabled = settingsContext.loadBool(enabledId);
        _rage = settingsContext.loadBool(rageId);
        _verticalPower = settingsContext.loadFloat(verticalPowerId);
        _horizontalPower = settingsContext.loadFloat(horizontalPowerId);
        _speed = settingsContext.loadFloat(speedId);
        _maxAngleChangePerTick = settingsContext.loadFloat(maxAngleChangePerTickId);
        _rangeInMeters = settingsContext.loadFloat(rangeInMetersId);
        _hitbox = settingsContext.loadInt(hitboxId);
    }

    void save(SettingsContext& settingsContext) const {
        settingsContext.set(enabledId, _enabled);
        settingsContext.set(rageId, _rage);
        settingsContext.set(verticalPowerId, _verticalPower);
        settingsContext.set(horizontalPowerId, _horizontalPower);
        settingsContext.set(speedId, _speed);
        settingsContext.set(maxAngleChangePerTickId, _maxAngleChangePerTick);
        settingsContext.set(rangeInMetersId, _rangeInMeters);
        settingsContext.set(hitboxId, _hitbox);
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