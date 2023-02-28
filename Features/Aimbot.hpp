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
#include "../Math/AimResolver.hpp"
#include "../Math/Random.hpp"
#include <cmath>

class Aimbot
{
private:

    Player* _currentTarget;
    bool _targetSelected;
    float _nextWriteAllowedAt;

    Aimbot() : _targetSelected(0), _currentTarget(nullptr), _nextWriteAllowedAt(0) {}
    ~Aimbot() {}

    bool isValidTarget(Player* player, const AimbotSettings& settings) const {

        if(player == nullptr ||
            !player->isValid() ||
            !player->isAlive() ||
            !player->isVisible() ||
            !player->isInRange(settings.getRangeInMeters()) || 
            !player->isEnemy()
            ) {
    
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

            QAngle targetAngle = AimResolver::calcualteAngle(cameraPosition, targetPos);
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


    // bool isValidTarget(Player* player, const AimbotSettings& settings) const {

    //     if(player == nullptr) {
    
    //         return false;
    //     }

    //     return true;
    // }

    // Player* currentAi;
    // Player* findBestTarget(const AimbotSettings& settings) {
    //     Vector3d cameraPosition = LocalPlayer::getInstance().getCameraPosition();
    //     QAngle currentAngle = LocalPlayer::getInstance().getViewAngle();
    //     for(auto i=0; i<2000; i++) {
    //         auto className = Player::getClientClassName(i);
    //         if (className != "CAI_BaseNPC") {
    //             continue;
    //         }
    //         std::cout << className << std::endl;
    //         if(currentAi != nullptr){
    //             delete currentAi;
    //         }

    //         currentAi = new Player(i);
    //         currentAi->update();

    //         if(isValidTarget(currentAi, settings)) {
    //             Vector3d targetPos = currentAi->getAimBonePosition();
    //             QAngle targetAngle = AimResolver::calcualteAngle(cameraPosition, targetPos);
    //             if(!targetAngle.isValid()) {
    //                 continue;
    //             }
    //             double distanceFromCroshair = currentAngle.distanceTo(targetAngle);

    //             if(!settings.isRage() && distanceFromCroshair > settings.getFieldOfView()) {
    //                 continue;
    //             }
    //             return currentAi;
    //         }

    //     }


    //     return nullptr;
    // }

public:
    static Aimbot& getInstance() {
        static Aimbot instance;
        return instance;
    }

    Aimbot(const Aimbot&) = delete;
    Aimbot& operator=(const Aimbot&) = delete;

    void update() {

        const AimbotSettings& settings = Settings::getInstance().getAimbotSettings();

        //return local player not valid
        if(!LocalPlayer::getInstance().isValid() || LocalPlayer::getInstance().isKnocked() || LocalPlayer::getInstance().isDead()) {
            _targetSelected = false;
            _nextWriteAllowedAt = 0;
            currentVelocityQX = 0;
            currentVelocityQY = 0;
            return;
        }
        
        //return aimbot aimbot should not be used
        if (!settings.isEnabled() || (!LocalPlayer::getInstance().isInAttack() && !settings.useHotkey()) || (settings.useHotkey() && !InputManager::isKeyDownOrPress(settings.getAimHotkey()))) {
            _currentTarget = nullptr;
            _targetSelected = false;
            _nextWriteAllowedAt = 0;
            currentVelocityQX = 0;
            currentVelocityQY = 0;
            return;
        }

        //return cant write atm
        const float currentTime = TimeManager::getInstance().getTime();
        if(!settings.isRage() && _nextWriteAllowedAt > currentTime) {
            return;
        }

        Player* target = _currentTarget;
        // if(target != nullptr) {
        //     target->update();
        // }
        if(!isValidTarget(target, settings))
        {
            //cant select new target
            if(_targetSelected && !settings.allowForTargetSwitch()) {
                return;
            }

            target = findBestTarget(settings);
            //no valid target found
            if(!isValidTarget(target, settings))
            {
                _currentTarget = nullptr;
                return;
            }
            
            _currentTarget = target;
            _targetSelected = true;
        }

        Vector3d targePosition = target->getAimBonePosition();
        //sth wrong with target position
        if(targePosition.x == 0 && targePosition.y == 0 && targePosition.z == 0) {
            return;
        }
        
        QAngle targetAngle = QAngle(0, 0);
        //failed to calcualte angle
        if(!getAngle(target, settings, targetAngle)) {
            return;
        }
        
        LocalPlayer::getInstance().setViewAngle(targetAngle);

        const float randomDelay = settings.getRandomWriteDelayValue();
        if(randomDelay > 0) {
            _nextWriteAllowedAt = currentTime + randRange(0.0001, randomDelay);
        }
        else {
            _nextWriteAllowedAt = 0;
        }
    }

    void addRandomOffset(QAngle& angle, const float& value) const {
        angle.x += randRange(-value, value);
        angle.y += randRange(-value, value);
    }

    bool getAngle(const Player* target, const AimbotSettings& settings, QAngle& angle) {
        
        const QAngle currentAngle = LocalPlayer::getInstance().getViewAngle();
        
        if(!currentAngle.isValid()) {
            return false;
        }

        if(!getAngleToTarget(target, settings, angle)) {
            return false;
        }

        if(!settings.isRage() && settings.addRandomOffsetEnabled()) {
            const float randomOffset = settings.getRandomOffsetValue();
            addRandomOffset(angle, randomOffset);
        }

        if(!settings.isRage()) {
            angle = smoothAngleChange(currentAngle, settings, angle);
        }

        return true;
    }

    bool getAngleToTarget(const Player* target, const AimbotSettings& settings, QAngle& angle) const {
        auto weapon = LocalPlayer::getInstance().getWeapon();

        if(!weapon.isValid()) {
            return false;
        }

        const Vector3d targePosition = target->getAimBonePosition();
        const Vector3d targetVelocity = target->getVecAbsVelocity();
        const Vector3d cameraPosition = LocalPlayer::getInstance().getCameraPosition();
        const QAngle currentAngle = LocalPlayer::getInstance().getViewAngle();

        float projectileSpeed = weapon.getProjectileSpeed();
        float projectileDropRate = weapon.getProjectileScale();

        bool predictMovement = settings.predictMovementEnabled() || settings.isRage();
        bool predictBulletDrop = settings.predictBulletDropEnabled() || settings.isRage();
        
        if(projectileSpeed > 1.0f) {

            if(predictBulletDrop && predictMovement) {
                return AimResolver::calcualteAimRotationNew(cameraPosition, targePosition, targetVelocity, projectileSpeed, projectileDropRate, 255, angle);
            }
            else if (predictBulletDrop) {
                return AimResolver::calcualteAimRotationNew(cameraPosition, targePosition, Vector3d::zero(), projectileSpeed, projectileDropRate, 255, angle);
            }
            else if(predictMovement) {
                return AimResolver::calculateAimRotation(cameraPosition, targePosition, targetVelocity, projectileSpeed, angle);
            }
        }

        angle = AimResolver::calcualteAngle(cameraPosition, targePosition);
        return true;   
    }

    float smoothDamp(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed, float deltaTime)
    {
        float omega = 2.0f / smoothTime;
        float x = omega * deltaTime;
        float alpha = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
        float delta = target - current;
        float maxDelta = maxSpeed * smoothTime;

        delta = std::clamp(delta, -maxDelta, maxDelta);
        float error = current - target;
        target = current - delta;

        float temp = (currentVelocity + omega * error) * deltaTime;
        currentVelocity = (currentVelocity - omega * temp) * alpha;
        float output = target + (delta + temp) * alpha;

        if (error > 0.0f == output > target)
        {
            output = target;
            currentVelocity = (output - target) / deltaTime;
        }

        return output;
    }

    float currentVelocityQX = 0;
    float currentVelocityQY = 0;

    QAngle smoothDampQAngle(const QAngle& currentAngle, const QAngle& targetAngle, float smoothTime, float maxSpeed, float deltaTime) {
        QAngle angle;
        angle.x = smoothDamp(currentAngle.x, targetAngle.x, currentVelocityQX, smoothTime, maxSpeed, deltaTime);
        angle.y = smoothDamp(currentAngle.y, targetAngle.y, currentVelocityQY, smoothTime, maxSpeed, deltaTime);
        return angle;
    }

    QAngle smoothAngleChange(const QAngle& currentAngle, const AimbotSettings& settings, const QAngle& targetAngle) {
     
        const float deltaTime = TimeManager::getInstance().getDeltaTime();
        const float smoothingFactor = settings.getSpeed() * deltaTime;

        QAngle angle = 
            //smoothDampQAngle(currentAngle, targetAngle, settings.getSpeed(), 99.0f, deltaTime);
            currentAngle.lerp(targetAngle, std::clamp(smoothingFactor, 0.0f, 1.0f)).fixAngle();

        QAngle angleChange = angle - currentAngle;

        const float maxAngleChange = settings.getMaxAngleChangePerTick() + randRange(-0.05, 0.05);
        if(maxAngleChange > 0.001) {
            angleChange = angleChange.clamp(-maxAngleChange, maxAngleChange);
        }

        angleChange.x *= settings.getVerticalPower(); 
        angleChange.y *= settings.getHorizontalPower();

        return currentAngle + angleChange;
    }

};

