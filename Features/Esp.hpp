#pragma once
#include "../Math/Vector2d.hpp"
#include "../Game/Camera.hpp"
#include "../Game/PlayerManager.hpp"
#include "../RenderEngine/Renderer.hpp"
#include "Settings.hpp"

class Esp
{
private:

    Esp() {}
    ~Esp() {}

    bool isValidTarget(Player* player, EspSettings& settings) const {

        if(player == nullptr || !player->isValid()) {
            return false;
        }

        if(player->isDead() || !player->isEnemy() || !player->isInRange(settings.hitboxRangeInMeters())) {
            return false;
        }

        return true;
    }

public:
    
    static Esp& getInstance() {
        static Esp instance;
        return instance;
    }

    Esp(const Esp&) = delete;
    Esp& operator=(const Esp&) = delete;

    void render() {
        
        auto settings = Settings::getInstance().getEspSettings();

        if(!settings.isDrawHitboxPositionEnabled()) {
            return;
        }

        for(auto player : PlayerManager::getInstance()) {
            if(!isValidTarget(player, settings)) {
                continue;
            }

            Vector2d screenPosition;
            if(Camera::getInstance().worldToScreen(player->getAimBonePosition(), screenPosition)) {
                Renderer::drawRectangleOutline(screenPosition.x - 10, screenPosition.y - 10, 20, 20, 1.0f, 1.0f, 1.0f, 2.0f);
            }

        }
    }

};
