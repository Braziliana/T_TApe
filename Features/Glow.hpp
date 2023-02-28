#pragma once
#include "../Math/Color.hpp"
#include "../Game/Player.hpp"
#include "../Game/GlowMode.hpp"
#include "../Game/GameManager.hpp"
#include "../Game/TimeManager.hpp"
#include "EspSettings.hpp"
#include "Settings.hpp"

class Glow
{
private:
    Glow() {}
    ~Glow() {}

    Color getGlowColor(const Player* player, const EspSettings& settings) const {

        if(settings.getGlowType() == GlowType::StaticColorGlow) {
            return settings.getGlowStaticColor();
        }

        if(player->getShield() > 0 && player->getMaxShield() > 0) {
            return Color::lerp(settings.getGlowMinShieldColor(), settings.getGlowMaxShieldColor(), (float)player->getShield() / (float)player->getMaxShield());
        }

        if(player->getHealth() > 0 && player->getMaxHealth() > 0) {
            return Color::lerp(settings.getGlowMinHealthColor(), settings.getGlowMaxHealthColor(), (float)player->getHealth() / (float)player->getMaxHealth());
        }

        return Color(1.0f, 1.0f, 1.0f);
    }

    bool isValidTarget(Player* player, const EspSettings& settings) const {

        if(player == nullptr || !player->isValid()) {
            return false;
        }

        if(player->isDead() || !player->isEnemy()) {
            return false;
        }

        return true;
    }

    float _lastUpdate = 0;

    void enableGlow(Player* player, const EspSettings& settings) const {
        
        if(settings.getGlowType() == GlowType::BasicGlow) {
            if(player->isVisible()) {
                player->setGlowState(5, 1);
            }
            else {
                player->setGlowState(7, 2);
            }
        }
        else {
            player->setGlowState(1, 2);
            
            if(settings.isGlowModeEnabled()) {
                player->setGlowMode(GlowMode(HighlightFill::CustomColor, HighlightOutline::CustomColor, settings.getGlowBorder(), settings.getGlowTransparentLevel()));
            }

            player->setGlowColor(getGlowColor(player, settings).roundColor());
        }
    }

    void disableGlow(Player* player) const {
        player->setGlowState(2, 5);
    }

public:
    static Glow& getInstance() {
        static Glow instance;
        return instance;
    }

    Glow(const Glow&) = delete;
    Glow& operator=(const Glow&) = delete;

    void update() {
        const EspSettings& settings = Settings::getInstance().getEspSettings();

        if(!LocalPlayer::getInstance().isValid() || LocalPlayer::getInstance().isKnocked() || LocalPlayer::getInstance().isDead()) {
            _lastUpdate = 0;
            return;
        }

        if(!settings.isGlowEnabled() || _lastUpdate + 0.1f > TimeManager::getInstance().getTime()){
            return;
        }

        _lastUpdate = TimeManager::getInstance().getTime();

        for(auto player : PlayerManager::getInstance()) {
            if(isValidTarget(player, settings)) {
                if(player->isInRange(settings.glowRangeInMeters())) {
                    enableGlow(player, settings);
                }
                else {
                    disableGlow(player);
                }
            }
        }
    }
};
