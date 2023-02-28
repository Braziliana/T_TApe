#pragma once
#include "../Game/LocalPlayer.hpp"
#include "Rcs.hpp"
#include "Esp.hpp"
#include "Glow.hpp"
#include "Aimbot.hpp"

class FeaturesManager
{
private:

    FeaturesManager() {}
    ~FeaturesManager() {}

public:

    static FeaturesManager& getInstance() {
        static FeaturesManager instance;
        return instance;
    }

    FeaturesManager(const FeaturesManager&) = delete;
    FeaturesManager& operator=(const FeaturesManager&) = delete;

    void update() {
        if(!LocalPlayer::getInstance().isValid() || LocalPlayer::getInstance().isDead()) {
            return;
        }
        
        Glow::getInstance().update();
        Aimbot::getInstance().update();
        Rcs::getInstance().update();
    }

    void render() {
        if(!LocalPlayer::getInstance().isValid() || LocalPlayer::getInstance().isDead()) {
            return;
        }

        Esp::getInstance().render();
    }
};