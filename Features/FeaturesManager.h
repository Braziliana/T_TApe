//
// Created by koelion on 05.02.24.
//

#ifndef APEX_FEATURESMANAGER_H
#define APEX_FEATURESMANAGER_H

#include "../SDK/GameContext.h"
#include "Aimbot.h"
#include "Misc.h"
#include "Rcs.h"
#include "Glow.h"
#include "Esp.h"
#include "../Misc/Strings.h"
#include "../Misc/Timer.h"

namespace Features
{
    class FeaturesManager
    {
    private:
        Aimbot aimbot;
        Rcs rcs;
        int lastFrame;
        FeaturesManager() : lastFrame(0) {}
        ~FeaturesManager() {}

        void DebugInfo(SDK::PGameContext gameContext){
            auto player = gameContext->GetLocalPlayer();
            auto viewAngle = player->GetViewAngle();

            auto y = 0.0f;
            Renderer::drawText(concatenateStrings("View angles: ", viewAngle.x, viewAngle.y), Math::Vector2D(100, 50+y), Math::Color(1.0f, 0.0f, 1.0f));
            y += 20;

            auto weapon = player->GetWeapon();
            if(weapon->IsValid())
            {
                Renderer::drawText(concatenateStrings("Projectile speed: ", weapon->ProjectileSpeed()), Math::Vector2D(100, 50+y), Math::Color(1.0f, 0.0f, 1.0f));
                y += 20;
            }

            auto velocity = Math::Vector3D();
            if(player->ReadOffset(OFF_ABSVELOCITY, &velocity))
            {
                Renderer::drawText(concatenateStrings("Velocity: ", velocity.x, velocity.y, velocity.z), Math::Vector2D(100, 50+y), Math::Color(1.0f, 0.0f, 1.0f));
                y += 20;
            }
        }

    public:
        static FeaturesManager& getInstance() {
            static FeaturesManager instance;
            return instance;
        }

        FeaturesManager(const FeaturesManager&) = delete;
        FeaturesManager& operator=(const FeaturesManager&) = delete;

        void Update(SDK::PGameContext gameContext, PTimer timer)
        {
            //DebugInfo(gameContext);
            
            Esp(gameContext);
            if(lastFrame == gameContext->GetFrameCount())
            {
                return;
            }
            
            lastFrame = gameContext->GetFrameCount();

            auto aimbotRun = false;
            auto aimbotSettings = Settings::getInstance().GetAimbotSettings();

            //!InputManager::isKeyDownOrPress(settings.getAimHotkey())
            //if(gameContext->IsButtonDown(aimbotSettings->GetAimbotKey())){
            if(InputManager::isKeyDownOrPress(static_cast<InputKeyType>(aimbotSettings->GetAimbotKey())))
            {
                aimbotRun = aimbot.Update(gameContext, timer);
            }
            else 
            {
                aimbot.ResetValues();
            }

            if(!aimbotRun)
            {
                aimbot.Reset();
                rcs.Update(gameContext);
            }
            else
            {
                rcs.Reset();
            }

            Misc(gameContext);
            Glow(gameContext);
        }
    };
}

#endif //APEX_FEATURESMANAGER_H
