//
// Created by koelion on 05.02.24.
//

#ifndef APEX_AIMBOT_H
#define APEX_AIMBOT_H

#include <cmath>
#include <iostream>

#include "../Math/Vector2D.h"
#include "../Math/Vector3D.h"
#include "../Math/QAngle.h"

#include "../SDK/Player.h"
#include "../SDK/LocalPlayer.h"
#include "../SDK/GameContext.h"
#include "MoveMouse.h"
#include "Settings.h"
#include "../Misc/Timer.h"
#include "TargetSelector.h"
#include "../Drawings/Renderer.h"

namespace Features
{
    enum class AimState {
		Idle,
		Break,
		Aiming
	};

	struct Aimbot 
	{
	private:
		Math::Vector2D rcsMouseMove;
		SDK::PPlayer currentTarget;
		int targetId=-1;

		bool canGetNextTarget;
		float lostTarget;

		AimState aimState;
		float aimTimer;

	public:

		Aimbot() {
			ResetValues();
		}

		void Reset() {
			rcsMouseMove = Math::Vector2D(0, 0);
		}

		void ResetValues() {
			canGetNextTarget = true;
			lostTarget = -9999;
			aimState = AimState::Idle;
			aimTimer = -9999;
			currentTarget = nullptr;
			targetId=-1;
			Reset();
		}

		SDK::PPlayer GetTarget(SDK::PGameContext gameContext, PTimer timer) 
        {
            auto aimbotSettings = Settings::getInstance().GetAimbotSettings();
			auto localPlayer = gameContext->GetLocalPlayer();

			if (IsValidTarget(currentTarget, localPlayer)) {
				return currentTarget;
			}

			auto target = SelectTarget(gameContext);

            if(target != nullptr && targetId != target->GetIndex()){
                if (!canGetNextTarget || lostTarget + aimbotSettings->GetswitchTargetDelay() > timer->Time()) {

                    return nullptr;
                }
            }

			lostTarget = timer->Time();
			if (target != nullptr) {
				canGetNextTarget = !aimbotSettings->IsSingleTargetPerAimUse();
				targetId = target->GetIndex();
			}
			else {
				lostTarget = timer->Time();
				aimTimer = -9999;
				aimState = AimState::Idle;
			}

			currentTarget = target;
			return target;
		}

		void UpdateAimState(PTimer timer) {
            auto aimbotSettings = Settings::getInstance().GetAimbotSettings();
			if (!aimbotSettings->IsAimAssistEnabled()) {
				return;
			}

			if (aimState == AimState::Idle) {
				aimTimer = timer->Time();
				aimState = AimState::Aiming;
				return;
			}

			auto stateTime = 0.0f;
			switch (aimState)
			{
			case AimState::Idle:
				stateTime = -1;
			case AimState::Break:
				stateTime = aimbotSettings->GetAimBreak();
				break;
			case AimState::Aiming:
				stateTime = aimbotSettings->GetAimTime();
				break;
			default:
				break;
			}

            Renderer::drawText(std::to_string(aimTimer + stateTime - timer->Time()), Math::Vector2D(100, 120), Math::Color(1.0f, 0.0, 0.0));

			if (timer->Time() > aimTimer + stateTime) {
				switch (aimState)
				{
				case AimState::Idle:
				case AimState::Break:
					aimState = AimState::Aiming;
					break;
				case AimState::Aiming:
					aimState = AimState::Break;
					break;
				default:
					break;
				}

				aimTimer = timer->Time();
			}
		}

		float SmoothMouse(float distance)
		{
            auto aimbotSettings = Settings::getInstance().GetAimbotSettings();
			float minSmoothStep = aimbotSettings->GetSmoofingMinSpeed();
			float maxSmoothStep = aimbotSettings->GetSmoofingMaxSpeed();

			float step = distance / aimbotSettings->GetSmoofing();
			auto absDistance = std::abs(distance);
			auto absStep  = std::abs(step);

			if (absStep < minSmoothStep) {
				if (absDistance <= minSmoothStep) {
					return distance;
				}
				else 
				{
					if (step < 0) {
						return -minSmoothStep;
					}

					return minSmoothStep;
				}
			}

			if (absStep > maxSmoothStep) {

				if (step < 0) {
					return -maxSmoothStep;
				}

				return maxSmoothStep;
			}

			return step;
		}

		void AimbotMouseRCS(SDK::PGameContext gameContext, Math::QAngle punchAngle, float fov, Math::Vector2D aimMouseMove) {

			auto pitch = -punchAngle.x;
			auto yaw = punchAngle.y;

			if (pitch < 0) {
				pitch = 0;
			}

			auto scale = (0.088f * (fov / 360.0f)) * gameContext->GetMouseSensivity();

			rcsMouseMove.x += yaw / scale;
			rcsMouseMove.y += pitch / scale;

			auto x = std::round(rcsMouseMove.x);
			auto y = std::round(rcsMouseMove.y);

			auto lx = static_cast<int>(x);
			auto ly = static_cast<int>(y);

			if (lx == 0 && ly == 0) {
				return;
			}

			rcsMouseMove.x -= x;
			rcsMouseMove.y -= y;

			aimMouseMove.x += x;
			aimMouseMove.x += y;
		}

		bool AimMoveMouse(SDK::PGameContext gameContext, SDK::PPlayer target)
		{
            auto aimbotSettings = Settings::getInstance().GetAimbotSettings();
			auto localPlayer = gameContext->GetLocalPlayer();
			auto weapon = localPlayer->GetWeapon();
			auto camera = gameContext->GetCamera();

			auto targetPosition = Math::Vector3D();
			if (!target->GetBonePosition(aimbotSettings->GetHitbox(), &targetPosition)) {
				return false;
			}

			if (aimbotSettings->IsPredictionEnabled() && weapon->IsValid()) {
				//AimCorrection(localPlayer->GetCameraPosition(), weapon->ProjectileScale(), &targetPosition, target->GeVelocity(), weapon->ProjectileSpeed());

				auto time = localPlayer->GetCameraPosition().Distance(targetPosition) / weapon->ProjectileSpeed();
				if (time > 0.001f) {
					targetPosition += target->GeVelocity() * time;
				}
			}
			
			auto screenPosition = Math::Vector2D();
			if (!camera->WorldToScreen(targetPosition, screenPosition)) {
				return false;
			}

			auto screenMiddle = camera->GetScreenMiddle();
			auto mouseMove = screenPosition - screenMiddle;

			if (aimbotSettings->IsSmoothEnabled()) {
				mouseMove.x = SmoothMouse(mouseMove.x);
				mouseMove.y = SmoothMouse(mouseMove.y);
			}

			if (aimbotSettings->IsRcsEnabled()) {
				auto punchAngle = localPlayer->GetPunchAngleDiffrence();
				punchAngle.x *= aimbotSettings->GetRCSPitchPower();
				punchAngle.y *= aimbotSettings->GetRCSYawPower();

				AimbotMouseRCS(gameContext, punchAngle, localPlayer->GetWeapon()->TargetZoomFov(), mouseMove);
			}

            Renderer::drawText("Aim", Math::Vector2D(100, 100), Math::Color(1.0f, 0.0f, 0.0f));
			MoveMouse(std::round(mouseMove.x), std::round(mouseMove.y));
		

			return true;
		}

		bool Update(SDK::PGameContext gameContext, PTimer timer)
		{
            Renderer::drawText(std::to_string(timer->Time()), Math::Vector2D(100, 140), Math::Color(1.0f, 1.0, 1.0));

            auto aimbotSettings = Settings::getInstance().GetAimbotSettings();
			auto localPlayer = gameContext->GetLocalPlayer();
			if (localPlayer->IsDead() || localPlayer->IsKnocked()) {
				ResetValues();
				return false;
			}


			auto camera = gameContext->GetCamera();
            Renderer::DrawCircle(camera->GetScreenMiddle(), aimbotSettings->GetFov(), Math::Color(0, 1, 0));

			/*if (localPlayer->GetWeapon()->AmmoInClip() < 1) {
				return false;
			}*/

			int8_t isInAttack = 0;
			if (!Memory::getInstance().read<int8_t>(OFF_IN_ATTACK, &isInAttack) || isInAttack <= 0)
			{
				Reset();

				if (aimbotSettings->IsAimActiveOnlyWhenShooting()) {
					return false;
				}
			}

			auto target = GetTarget(gameContext, timer);
			if (target == nullptr) {
				Reset();
				return false;
			}

			UpdateAimState(timer);
			if (aimState != AimState::Aiming) {
				return false;
			}

			if (!AimMoveMouse(gameContext, target)) {
				Reset();
			}
			return true;
		}
	};
}

#endif //APEX_AIMBOT_H
