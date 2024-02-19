#pragma once
#include "../Math/Color.h"

#include "../SDK/Player.h"
#include "../SDK/LocalPlayer.h"
#include "../SDK/EntityManager.h"
#include "../SDK/GameContext.h"
#include "../SDK/Glow.h"
#include "Settings.h"

namespace Features
{
	#define GLOW_KNOCKED_INDEX 65
	#define GLOW_ENEMY_INDEX 66
	#define GLOW_VISIBLE_ENEMY_INDEX 67

	bool InitGlowSettings() {
		auto glowSettings = Settings::getInstance().GetGlowSettings();
		uintptr_t highlightSettingsPtr = 0x0;
		if (!Memory::getInstance().read<uintptr_t>(OFF_REGION + OFF_GLOW_HIGHLIGHTS, &highlightSettingsPtr) || highlightSettingsPtr == 0x0) {
			return false;
		}

		//knocked
		if (!SDK::WriteGlowSettings(highlightSettingsPtr, GLOW_KNOCKED_INDEX, SDK::HilightFunctionBits(2, 125, 64, 64), glowSettings->GetKnockedColor())) {
			return false;
		}

		//enemy
		if (!SDK::WriteGlowSettings(highlightSettingsPtr, GLOW_ENEMY_INDEX, SDK::HilightFunctionBits(2, 125, 64, 64), glowSettings->GetEnemyColor())) {
			return false;
		}

		//visible enemy
		if (!SDK::WriteGlowSettings(highlightSettingsPtr, GLOW_VISIBLE_ENEMY_INDEX, SDK::HilightFunctionBits(2, 125, 64, 64), glowSettings->GetVisibleEnemyColor())) {
			return false;
		}

		//items glow
		for (uint8_t settingsIndex = 34; settingsIndex < 39; settingsIndex++) {
			SDK::WriteGlowSettings(highlightSettingsPtr, settingsIndex, SDK::HilightFunctionBits(137, 138, 35, 127));//(137, 0, 0, 127));
		}

		return true;
	}

	void Glow(SDK::PGameContext gameContext)
	{
		auto glowSettings = Settings::getInstance().GetGlowSettings();
		auto localPlayer = gameContext->GetLocalPlayer();
		if (localPlayer == nullptr) {
			return;
		}

		const auto entityManager = gameContext->GetEntityManager();
		if (entityManager == nullptr) {
			return;
		}

		if (!InitGlowSettings()) {
			return;
		}

		for (auto i = 0; i < entityManager->PlayersCount(); i++) {
			auto player = entityManager->GetPlayer(i);
			if (player == nullptr || !player->IsValid() || player->IsDead() || player->GetTeam() == localPlayer->GetTeam()) {
				continue;
			}

			if (player->GetDistanceInMeters(localPlayer->GetPosition()) > glowSettings->GetGlowDistance()) {

				player->DisableGlow();
				continue;
			}

			uint8_t settingIndex = 0;

			if (player->IsKnocked()) {
				settingIndex = GLOW_KNOCKED_INDEX;
			}
			else if (player->IsVisible()) {
				settingIndex = GLOW_VISIBLE_ENEMY_INDEX;
			}
			else {
				settingIndex = GLOW_ENEMY_INDEX;
			}

			if (settingIndex == 0) {
				continue;
			}

			player->SetGlow(settingIndex);
		}
	}
}