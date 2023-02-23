#pragma once
#include "../Math/Color.hpp"
#include "../Game/Player.hpp"
#include "../Game/GlowMode.hpp"
#include "../Game/GameManager.hpp"
#include "../Game/TimeManager.hpp"
#include "Settings.hpp"

class Glow
{
private:
    Glow() {}
    ~Glow() {}

    Color getGlowColor(const Player* player) const {
        if(player->getShield() > 0 && player->getMaxShield() > 0) {
            return Color::lerp(Color(255.0f,  255.0f, 255.0f), Color(0.0f, 0.0f, 255.0f), (float)player->getShield() / (float)player->getMaxShield());
        }
        if(player->getHealth() > 0 && player->getMaxHealth() > 0) {
            return Color::lerp(Color(0.0f, 255.0f, 0.0f), Color(255.0f, 0.0f, 0.0f), (float)player->getHealth() / (float)player->getMaxHealth());
        }
        return Color(255.0f, 255.0f, 255.0f);
    }

    bool isValidTarget(Player* player, EspSettings& settings) const {

        if(player == nullptr || !player->isValid()) {
            return false;
        }

        if(player->isDead() ||
            player->isKnocked() ||
            !player->isEnemy() ||
            !player->isInRange(settings.glowRangeInMeters())) {
            return false;
        }

        return true;
    }

    float _lastUpdate = 0;

public:
    static Glow& getInstance() {
        static Glow instance;
        return instance;
    }

    Glow(const Glow&) = delete;
    Glow& operator=(const Glow&) = delete;

    void update() {
        auto settings = Settings::getInstance().getEspSettings();

        if(!settings.isGlowEnabled() || _lastUpdate + 0.5f > TimeManager::getInstance().getTime()){
            return;
        }

        _lastUpdate = TimeManager::getInstance().getTime();

        for(auto player : PlayerManager::getInstance()) {
            if(isValidTarget(player, settings)) {
                player->setGlowState(1, 2);
                player->setGlowMode(GlowMode(HighlightFill::CustomColor, HighlightOutline::CustomColor, 46, 90));
                player->setGlowColor(getGlowColor(player));
            }
        }
    }
};
