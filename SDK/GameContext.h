//
// Created by koelion on 05.02.24.
//

#ifndef APEX_GAMECONTEXT_H
#define APEX_GAMECONTEXT_H

#include "../Misc/Memory.h"
#include "../Misc/Timer.h"

#include "Offsets.h"
#include "GameLevel.h"
#include "Camera.h"
#include "LocalPlayer.h"
#include "EntityManager.h"
#include "Player.h"
#include "ButtonType.h"

namespace SDK
{
    struct GameContext
    {
    private:

        int frameCount;
        float gameTime;
        float deltaTime;
        float sensivity;
        GameLevel gameLevel;
        Camera camera;
        LocalPlayer localPlayer;
        EntityManager entityManager;

        bool UpdateTime() {
            float prev = gameTime;
            //frameCount = Memory::getInstance().read<ulong>(OFF_REGION + OFF_GLOBAL_VARS + sizeof(double));
            if(!Memory::getInstance().read(OFF_REGION + OFF_GLOBAL_VARS + sizeof(double), &frameCount))
            {
                return false;
            }
            if (!localPlayer.ReadOffset(OFF_TIME_BASE, &gameTime)) {
                return false;
            }
            deltaTime = gameTime - prev;
            return true;
        }

    public:

        GameContext()
        {
            sensivity=0;
            frameCount = 0;
            gameTime = 0;
            deltaTime = 0;
            gameLevel = GameLevel();
            camera = Camera();
            localPlayer = LocalPlayer();
            entityManager = EntityManager();
        }

        bool Update(PTimer timer)
        {
            if (!gameLevel.Update()) {
                //Logger::log("Failed update gamecontext gameLevel");
                return false;
            }

            if (!camera.Update()) {
                //Logger::log("Failed update gamecontext camera");
                return false;
            }

            if (!localPlayer.Update()) {
                //Logger::log("Failed update gamecontext localPlayer");
                return false;
            }

            if(!Memory::getInstance().read(OFF_REGION + OFF_MOUSE_SENSITIVITY, & sensivity)){
                sensivity = 1.0f;
            }

            if (!UpdateTime()) {
                //Logger::log("Failed update gamecontext UpdateTime");
                return false;
            }

            if (!entityManager.Update(timer)) {
                //Logger::log("Failed update gamecontext entityManager");
                return false;
            }

            return true;
        }

        inline int GetFrameCount() const {
            return frameCount;
        }

        inline float GetTime() const {
            return gameTime;
        }

        inline float GetDeltaTime() const {
            return deltaTime;
        }

        inline PGameLevel GetGameLevel() {
            return &gameLevel;
        }

        inline PLocalPlayer GetLocalPlayer() {
            return &localPlayer;
        }

        inline PEntityManager GetEntityManager() {
            return &entityManager;
        }

        inline PPlayer GetPlayer(int index) {
            return entityManager[index];
        }

        inline PCamera GetCamera() {
            return &camera;
        }

        inline bool IsButtonDown(int button) {
            if (button == 0) {
                return false;
            }

            unsigned long state = 0;
            if(!Memory::getInstance().read(OFF_REGION + OFF_INPUT_SYSTEM + ((button >> 5) * 4) + 0xb0, &state))
            {
                return false;
            }

            return (state >> (button & 31)) & 1;
        }

        inline bool IsButtonDown(ButtonType button) {
            return IsButtonDown((int)button);
        }

        inline float GetMouseSensivity(){
            return sensivity;
        }

    };

    typedef GameContext* PGameContext;
}

#endif //APEX_GAMECONTEXT_H
