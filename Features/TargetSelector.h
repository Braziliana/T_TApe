//
// Created by koelion on 05.02.24.
//

#ifndef TARGETSELECTOR_H
#define TARGETSELECTOR_H

#include <cmath>

#include "../Math/Vector2D.h"
#include "../Math/Vector3D.h"
#include "../Math/QAngle.h"

#include "../SDK/Player.h"
#include "../SDK/LocalPlayer.h"
#include "../SDK/EntityManager.h"
#include "../SDK/GameContext.h"

#include "../Misc/Logger.h"
#include "Settings.h"

namespace Features
{
    bool IsValidTarget(SDK::PPlayer player, SDK::PLocalPlayer localPlayer) {
		return player != nullptr &&
			player->IsValid() &&
			!player->IsDead() &&
			!player->IsKnocked() &&
			player->GetTeam() != localPlayer->GetTeam() &&
			player->IsVisible();
	}

    SDK::PPlayer SelectTarget(SDK::PGameContext gameContext)
	{
        auto aimbotSettings = Settings::getInstance().GetAimbotSettings();

		float bestDistanceFromCroshair = 99999999;
		float bestDistanceFromTarget = 99999999;
		SDK::PPlayer bestTarget = nullptr;
		Math::Vector3D targetPosition = Math::Vector3D();
		auto camera = gameContext->GetCamera();
		auto localPlayer = gameContext->GetLocalPlayer();
		auto entityManager = gameContext->GetEntityManager();

		for (auto i = 0; i < entityManager->PlayersCount(); i++)
		{
			auto player = entityManager->GetPlayer(i);
			if (!IsValidTarget(player, localPlayer))
			{
				continue;
			}

			auto distanceToPlayer = player->GetDistanceInMeters(localPlayer->GetPosition());
			if (distanceToPlayer > aimbotSettings->GetDistance() || distanceToPlayer <= 0.001f) {
				continue;
			}

			auto bonePosition = Math::Vector3D();
			if (!player->GetBonePosition(aimbotSettings->GetHitbox(), &bonePosition)) {
				continue;
			}

			auto screenPosition = Math::Vector2D();
			if (!camera->WorldToScreen(bonePosition, screenPosition)) {
				continue;
			}
			auto screenDistance = camera->DistanceFromCrosshair(screenPosition);

			if (screenDistance > aimbotSettings->GetFov() && distanceToPlayer > aimbotSettings->GetIgnoreFovCheckDistance())
			{
				continue;
			}

			if (player->IsAimedAt()) {
				return player;
			}

			if (screenDistance > bestDistanceFromCroshair) {
				continue;
			}
			bestDistanceFromCroshair = screenDistance;

			//SELECT CLOSEST TO PLAYER
			if (distanceToPlayer > bestDistanceFromTarget) {
				continue;
			}

			bestDistanceFromTarget = distanceToPlayer;
			bestTarget = player;
		}

		return bestTarget;
	}
}

#endif //TARGETSELECTOR_H
