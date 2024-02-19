//
// Created by koelion on 05.02.24.
//

#ifndef APEX_MISC_H
#define APEX_MISC_H
#include <stdint.h>
#include "../Misc/Memory.h"

#include "../SDK/LocalPlayer.h"
#include "../SDK/EntityManager.h"
#include "../SDK/GameContext.h"
#include "../SDK/Offsets.h"
#include "../SDK/ButtonType.h"
#include "../SDK/GameContext.h"

#include "Settings.h"

namespace Features
{
    void SuperGlide(SDK::PGameContext gameContext)
    {
        static int sgState = 0;
        static int sgFrameTime = 0;

        auto localPlayer = gameContext->GetLocalPlayer();

        float traversalStartTime = 0.0f;
        if (!localPlayer->ReadOffset(OFF_TRAVERSAL_START_TIME, &traversalStartTime)) {
            return;
        }

        float traversalProgress = 0.0f;
        if (!localPlayer->ReadOffset(OFF_TRAVERSAL_PROGRESS, &traversalProgress)) {
            return;
        }

        float traversalReleaseTime = 0.0f;
        if (!localPlayer->ReadOffset(OFF_TRAVERSAL_RELEASE_TIME, &traversalReleaseTime)) {
            return;
        }

        auto time = gameContext->GetTime();
        auto hangOnWall = time - traversalStartTime;

        switch (sgState)
        {
            case 0:
                if (traversalProgress > 0.88f && hangOnWall > 0.0f && hangOnWall < 1.5f && traversalReleaseTime == 0.0f)
                {
                    Memory::getInstance().write2<int>(OFF_REGION + OFF_IN_JUMP + 0x8, 5);
                    sgState = 1;
                    sgFrameTime = gameContext->GetFrameCount();
                }
                else if (hangOnWall > 0.1f && hangOnWall < 0.12f && traversalProgress < 0.85f) {
                    Memory::getInstance().write2<int>(OFF_REGION + OFF_IN_JUMP + 0x8, 4);
                }
                break;
            case 1:
                if(sgFrameTime + 1 <= gameContext->GetFrameCount())
                {
                    Memory::getInstance().write2<int>(OFF_REGION + OFF_IN_DUCK + 0x8, 5);
                    sgState = 2;
                    sgFrameTime = gameContext->GetFrameCount();
                }
                break;
            case 2:
                if (time - traversalReleaseTime > 0.1f && sgFrameTime + 1 <= gameContext->GetFrameCount())
                {
                    Memory::getInstance().write2<int>(OFF_REGION + OFF_IN_JUMP + 0x8, 4);
                    Memory::getInstance().write2<int>(OFF_REGION + OFF_IN_DUCK + 0x8, 4);
                    sgState = 0;
                }
                break;
        }
    }

    void AutoWallJump(SDK::PGameContext gameContext) {
        auto localPlayer = gameContext->GetLocalPlayer();

        float wallStartTime = 0.0f;
        if (!localPlayer->ReadOffset(OFF_WALL_RUN_START_TIME, &wallStartTime)) {
            return;
        }

        float wallProgress = 0x0;
        if (!localPlayer->ReadOffset(OFF_TRAVERSAL_PROGRESS, &wallProgress)) {
            return;
        }

        static float onWallTmp = 0;
        if (wallStartTime > onWallTmp + 0.1)
        {
            int forward = 0;
            if (Memory::getInstance().read<int>(OFF_REGION + OFFSET_IN_FORWARD, &forward) && forward == 0)
            {
                Memory::getInstance().write2<int>(OFF_REGION + OFF_IN_JUMP + 0x8, 5);
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
                Memory::getInstance().write2<int>(OFF_REGION + OFF_IN_JUMP + 0x8, 4);
            }
        }
        onWallTmp = wallStartTime;

        static float onEdgeTmp = 0;
        if (wallProgress > onEdgeTmp + 0.1)
        {
            int forward = 0;
            if (Memory::getInstance().read<int>(OFF_REGION + OFFSET_IN_FORWARD, &forward) && forward == 0)
            {
                Memory::getInstance().write2<int>(OFF_REGION + OFF_IN_JUMP + 0x8, 5);
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
                Memory::getInstance().write2<int>(OFF_REGION + OFF_IN_JUMP + 0x8, 4);
            }
        }
        onEdgeTmp = wallProgress;
    }

    void BunnyHop(SDK::PGameContext gameContext)
    {

    }


    void Misc(SDK::PGameContext gameContext)
    {
        auto misSettings = Settings::getInstance().GetMiscSettings();
        int forward = 0;
        if(Memory::getInstance().read<int>(OFF_REGION + OFFSET_IN_FORWARD, &forward) && forward != 0)
        {
            if(misSettings->IsSuperGlideEnabled()){
                SuperGlide(gameContext);
            }
        }
        else {
            if(misSettings->IsWallJumpEnabled()){
            AutoWallJump(gameContext);
            }
        }
    }
}

#endif //APEX_MISC_H
