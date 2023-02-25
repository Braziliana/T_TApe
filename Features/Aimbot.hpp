#pragma once

#include "../Game/LocalPlayer.hpp"
#include "../Game/Player.hpp"
#include "../Game/PlayerManager.hpp"
#include "../Game/TimeManager.hpp"
#include "../Utils/Logger.hpp"
#include "../Utils/InputManager.hpp"
#include "../Utils/InputTypes.hpp"
#include "Settings.hpp"
#include "AimbotSettings.hpp"

class Aimbot
{
private:

    Player* _currentTarget;

    Aimbot() {}
    ~Aimbot() {}

    bool isValidTarget(Player* player, const AimbotSettings& settings) const {

        if(player == nullptr ||
            !player->isValid() ||
            !player->isAlive() ||
            !player->isVisible() ||
            !player->isInRange(settings.getRangeInMeters()) || 
            !player->isEnemy()) {
    
            return false;
        }

        return true;
    }

    Player* findBestTarget(const AimbotSettings& settings) const {
        float minDistance = 9999;
        Player* bestTarget = nullptr;
        Vector3d cameraPosition = LocalPlayer::getInstance().getCameraPosition();
        QAngle currentAngle = LocalPlayer::getInstance().getViewAngle();
        for(auto player : PlayerManager::getInstance()) {

            if(!isValidTarget(player, settings)) {
                continue;
            }

            Vector3d targetPos = player->getAimBonePosition();

            if(cameraPosition.distanceTo(targetPos) <= 0.0001f){
                continue;
            }

            QAngle targetAngle = QAngle::lookAt(cameraPosition, targetPos);
            if(!targetAngle.isValid()) {
                continue;
            }

            double distanceFromCroshair = currentAngle.distanceTo(targetAngle);

            if(!settings.isRage() && distanceFromCroshair > settings.getFieldOfView()) {
                continue;
            }

            if (distanceFromCroshair < minDistance)
            {
                bestTarget = player;
                minDistance = distanceFromCroshair;
            }
        }

        return bestTarget;
    }

public:
    static Aimbot& getInstance() {
        static Aimbot instance;
        return instance;
    }

    Aimbot(const Aimbot&) = delete;
    Aimbot& operator=(const Aimbot&) = delete;

    void update() {

        auto settings = Settings::getInstance().getAimbotSettings();

        if(!LocalPlayer::getInstance().isValid()) {
            return;
        }
        
        if ((!LocalPlayer::getInstance().isInAttack() && !settings.useHotkey()) || (settings.useHotkey() && !InputManager::isKeyDownOrPress(settings.getAimHotkey()))) {
            _currentTarget = nullptr;
            return;
        }

        Player* target = _currentTarget;
        if(!isValidTarget(target, settings))
        {
            target = findBestTarget(settings);
            if(!isValidTarget(target, settings))
            {
                _currentTarget = nullptr;
                return;
            }
            
            _currentTarget = target;
        }

        Vector3d targePosition = target->getAimBonePosition();

        if(targePosition.x == 0 && targePosition.y == 0 && targePosition.z == 0) {
            return;
        }

        QAngle targetAngle = getAngle(targePosition, settings);

        if(!targetAngle.isValid()) {
            return;
        }
        
        LocalPlayer::getInstance().setViewAngle(targetAngle);
    }

    QAngle getAngle(const Vector3d& targePosition, const AimbotSettings& settings) const {
        const Vector3d cameraPosition = LocalPlayer::getInstance().getCameraPosition();
        const QAngle currentAngle = LocalPlayer::getInstance().getViewAngle();
        QAngle targetAngle = currentAngle.lookAt(cameraPosition, targePosition);
        
        if(settings.isRage()) {
            return targetAngle;
        }
        
        const float deltaTime = TimeManager::getInstance().getDeltaTime();
        const float smoothingFactor = settings.getSpeed() * deltaTime;

        switch (settings.getAngleSmoothType())
        {
        case AngleSmoothType::LerpSmoothing:
            targetAngle = QAngle::lerpSmoothing(currentAngle, targetAngle, smoothingFactor);
            break;
        case AngleSmoothType::LinearSmoothing:
            targetAngle = QAngle::linearSmoothing(currentAngle, targetAngle, smoothingFactor);
            break;
        case AngleSmoothType::ExponentialSmoothing:
            targetAngle = QAngle::exponentialSmoothing(currentAngle, targetAngle, smoothingFactor);
            break;
        case AngleSmoothType::SCurveSmoothing:
            targetAngle = QAngle::sCurveSmoothing(currentAngle, targetAngle, smoothingFactor);
            break;
        case AngleSmoothType::BezierSmoothing:
            targetAngle = QAngle::bezierSmoothing(currentAngle, targetAngle, smoothingFactor);
            break;
        case AngleSmoothType::AccelerationSmoothing:
            targetAngle = QAngle::accelerationSmoothing(currentAngle, targetAngle, deltaTime, settings.getSpeed());
            break;
        case AngleSmoothType::JerkLimitedSmoothing: 
            targetAngle = QAngle::jerkLimitedSmoothing(currentAngle, targetAngle, deltaTime, settings.getSpeed());
            break;
        default:
            break;
        }

        QAngle angleChange = targetAngle - currentAngle;

        const float maxAngleChange = settings.getMaxAngleChangePerTick();
        angleChange = angleChange.clamp(-maxAngleChange, maxAngleChange);
        
        angleChange.x *= settings.getVerticalPower(); 
        angleChange.y *= settings.getHorizontalPower();

        return currentAngle + angleChange;
    }


    /*
    
     float bulletSpeed = curWeap.get_projectile_speed();
    float bulletGravity = curWeap.get_projectile_gravity();
    */
   
};

