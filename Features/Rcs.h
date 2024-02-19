//
// Created by koelion on 05.02.24.
//

#ifndef APEX_RCS_H
#define APEX_RCS_H

#include "../Misc/Memory.h"

#include "../Math/QAngle.h"

#include "../SDK/LocalPlayer.h"
#include "../SDK/GameContext.h"
#include "../SDK/Offsets.h"
#include "MoveMouse.h"

namespace Features
{
    struct Rcs 
	{
    private:
		Math::Vector2D mouseMove;

		bool SetMouse(SDK::PGameContext gameContext, Math::QAngle punchAngle, float fov) {
			auto pitch = -punchAngle.x;
			auto yaw = punchAngle.y;

			if (pitch < 0) {
				pitch = 0;
			}

			auto scale = (0.088f * (fov / 360.0f)) * gameContext->GetMouseSensivity();

			mouseMove.x += yaw / scale;
			mouseMove.y += pitch / scale;

			auto x = std::round(mouseMove.x);
			auto y = std::round(mouseMove.y);

			int lx = static_cast<int>(x);
			int ly = static_cast<int>(y);

			if (lx == 0 && ly == 0) {
				return false;
			}

			mouseMove.x -= x;
			mouseMove.y -= y;

			MoveMouse(lx, ly);
			return true;
		}

    public:
        void Reset() {
			mouseMove = Math::Vector2D(0, 0);
		}

        void Update(SDK::PGameContext gameContext)
        {
        	auto rcsSettings = Settings::getInstance().GetRcsSettings();
            if (!rcsSettings->IsEnabled()) {
				return;
			}

			auto localPlayer = gameContext->GetLocalPlayer();
			if (!localPlayer->IsValid() || localPlayer->IsDead() || localPlayer->IsKnocked()) {
				return;
			}

			auto punchAngle = localPlayer->GetPunchAngleDiffrence();
			int8_t isInAttack = 0;
			if (!Memory::getInstance().read<int8_t>(OFF_IN_ATTACK, &isInAttack) || isInAttack <= 0)
			{
				Reset();
				return;
			}

			if (!punchAngle.isValid()) {
				return;
			}

			punchAngle.x *= rcsSettings->GetRCSPitchPower();
			punchAngle.y *= rcsSettings->GetRCSYawPower();
            SetMouse(gameContext, punchAngle, localPlayer->GetWeapon()->TargetZoomFov());

        }
    };
}

#endif //APEX_RCS_H
