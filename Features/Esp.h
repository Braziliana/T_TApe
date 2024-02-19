#pragma once

#include "../Math/Color.h"
#include "../Math/Vector2D.h"
#include "../Math/Vector3D.h"

#include "../SDK/Camera.h"
#include "../SDK/Player.h"
#include "../SDK/LocalPlayer.h"
#include "../SDK/GameContext.h"

#include "../imgui/imgui.h"
#include "../Drawings/Renderer.h"
#include "Settings.h"
#include "../Settings/EspSettings.h"

namespace Features
{
    bool GetBoneScreenPosition(SDK::PCamera camera, SDK::PPlayer  player, SDK::HitboxType hitboxType, Math::Vector2D& screenPosition){
        auto bonePosition = Math::Vector3D();
        if (!player->GetBonePosition(hitboxType, &bonePosition))
        {
            return false;
        }

        return camera->WorldToScreen(bonePosition, screenPosition);
    }

    void EspBox(SDK::PGameContext gameContext, SDK::PPlayer player, float distance)
    {
        auto espSettings = Settings::getInstance().GetEspSettings();
        auto camera = gameContext->GetCamera();
        auto headPosition = Math::Vector3D();
        if (!player->GetBonePosition(SDK::HitboxType::Head, &headPosition))
        {
            return;
        }

        Math::Vector2D headScreenPosition = {};
        if (!camera->WorldToScreen(headPosition, headScreenPosition)) {
            return;
        }

        auto feetPosition = player->GetPosition();
        Math::Vector2D feetScreenPosition = {};
        if (!camera->WorldToScreen(feetPosition, feetScreenPosition)) {
            return;
        }

        auto h = feetScreenPosition.y - headScreenPosition.y;
        if (h < 10) {
            return;
        }

        auto w = h * 0.45f;

        Renderer::drawRectangleOutline(Math::Vector2D(headScreenPosition.x, headScreenPosition.y), Math::Vector2D(w, h), espSettings->GetEspEnemyColor(), 2.0f);

        if(espSettings->IsEspDistanceEnabled())
        {
            Renderer::drawText(concatenateStrings(std::round(distance), "m"), feetScreenPosition + Math::Vector2D(0, 5), espSettings->GetEspEnemyColor());
        }
    }

    void Skeleton(SDK::PGameContext gameContext, SDK::PPlayer player)
    {
        auto espSettings = Settings::getInstance().GetEspSettings();
        auto camera = gameContext->GetCamera();

        auto headPosition = Math::Vector2D();
        if(!GetBoneScreenPosition(camera, player, SDK::HitboxType::Head, headPosition)){
            return;
        }

        auto neckPosition = Math::Vector2D();
        if(!GetBoneScreenPosition(camera, player, SDK::HitboxType::Neck, neckPosition)){
            return;
        }

        auto upperChestPosition = Math::Vector2D();
        if(!GetBoneScreenPosition(camera, player, SDK::HitboxType::UpperChest, upperChestPosition)){
            return;
        }

        auto lowerChestPosition = Math::Vector2D();
        if(!GetBoneScreenPosition(camera, player, SDK::HitboxType::LowerChest, lowerChestPosition)){
            return;
        }

        auto stomachPosition = Math::Vector2D();
        if(!GetBoneScreenPosition(camera, player, SDK::HitboxType::Stomach, stomachPosition)){
            return;
        }

        Renderer::drawLine(headPosition, neckPosition, espSettings->GetBonesEspEnemyColor(), 2.0f);
        Renderer::drawLine(neckPosition, upperChestPosition, espSettings->GetBonesEspEnemyColor(), 2.0f);
        Renderer::drawLine(upperChestPosition, lowerChestPosition, espSettings->GetBonesEspEnemyColor(), 2.0f);
        Renderer::drawLine(lowerChestPosition, stomachPosition, espSettings->GetBonesEspEnemyColor(), 2.0f);
    }

    void HealthEsp(SDK::PGameContext gameContext, SDK::PPlayer player) {

        auto espSettings = Settings::getInstance().GetEspSettings();
        auto camera = gameContext->GetCamera();
        auto headPosition = Math::Vector3D();
        if (!player->GetBonePosition(SDK::HitboxType::Head, &headPosition))
        {
            return;
        }

        Math::Vector2D headScreenPosition = {};
        if (!camera->WorldToScreen(headPosition, headScreenPosition)) {
            return;
        }

        int segments = 2;
        auto shiledColor = Math::Color(0.6f, 0.6f, 0.6f);

        switch (player->GetMaxShield())
        {
        case 75:
            segments=3;
            shiledColor = Math::Color(0.0f, 0.0f, 1.0f);
            break;
        case 100:
            segments=4;
            shiledColor = Math::Color(119.0f/255.0f, 0.0f, 200.0f/255.0f);
            break;
        case 125:
            segments=5;
            shiledColor = Math::Color(1.0f, 0.0f, 0.0f);
            break;
        
        default:
            break;
        }

        Renderer::drawSheildBar(
            headScreenPosition + Math::Vector2D(0, -20),
            Math::Vector2D(150, 10),
            shiledColor,
            static_cast<float>(player->GetShield())/player->GetMaxShield(),
            segments);

        auto healthPercent = static_cast<float>(player->GetHealth())/player->GetMaxHealth();
        //auto healthColor = Math::Color(0.0f, 1.0f, 0.0f);
        auto healthColor = Math::Color::lerp(Math::Color(1.0f, 0.0f, 0.0f), Math::Color(0.0f, 1.0f, 0.0f), healthPercent);
        // if(healthPercent > 0.5f){
        //     healthColor = Math::Color::lerp(Math::Color(0.0f, 1.0f, 0.0f), Math::Color(1.0f, 1.0f, 0.0f), healthPercent);
        // }
        // else
        // {
        //     healthColor = Math::Color::lerp(Math::Color(1.0f, 1.0f, 0.0f), Math::Color(1.0f, 0.0f, 0.0f), healthPercent);
        // }

        Renderer::drawHealthBar(
            headScreenPosition + Math::Vector2D(0, -12),
            Math::Vector2D(150, 5),
            healthColor,
            healthPercent);
    }

    void Esp(SDK::PGameContext gameContext)
    {
        auto espSettings = Settings::getInstance().GetEspSettings();
        auto localPlayer = gameContext->GetLocalPlayer();
        const auto entityManager = gameContext->GetEntityManager();
        auto camera = gameContext->GetCamera();

        for (auto i = 0; i < entityManager->PlayersCount(); i++) {
            auto player = entityManager->GetPlayer(i);
            if (player == nullptr || !player->IsValid() || player->IsDead() ||
                player->GetTeam() == localPlayer->GetTeam()) {
                continue;
            }

            auto distance = player->GetDistanceInMeters(localPlayer->GetPosition());

            if(espSettings->IsEspEnabled() && distance <= espSettings->GetEspDistance())
            {
                EspBox(gameContext, player, distance);
            }
            
            if(espSettings->IsBonesEspEnabled() && distance <= espSettings->GetBonesEspDistance())
            {
                Skeleton(gameContext, player);
            }

            if(espSettings->IsHealthEspEnabled() && distance <= espSettings->GetHealthEspDistance()){
                HealthEsp(gameContext, player);
            }
        }
    }
}
