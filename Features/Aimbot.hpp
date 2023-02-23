#pragma once

#include "../Game/LocalPlayer.hpp"
#include "../Game/Player.hpp"
#include "../Game/PlayerManager.hpp"
#include "../Game/TimeManager.hpp"
#include "../Utils/Logger.hpp"
#include "../Utils/InputManager.hpp"

class Aimbot
{
private:

    Player* _currentTarget;

    Aimbot() {}
    ~Aimbot() {}

    bool isValidTarget(Player* player) const {

        if(player == nullptr ||
            !player->isValid() ||
            !player->isAlive() ||
            !player->isVisible() ||
            !player->isInRange(200) || 
            !player->isEnemy()) {
    
            return false;
        }

        return true;
    }

    Player* findBestTarget() const {
        float minDistance = 9999;
        Player* bestTarget = nullptr;
        Vector3d cameraPosition = LocalPlayer::getInstance().getCameraPosition();
        QAngle currentAngle = LocalPlayer::getInstance().getViewAngle();
        for(auto player : PlayerManager::getInstance()) {

            if(!isValidTarget(player)) {
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

        if (!LocalPlayer::getInstance().isInAttack())
        {
            _currentTarget = nullptr;
            return;
        }   

        Player* target = _currentTarget;
        if(!isValidTarget(target))
        {
            target = findBestTarget();
            if(!isValidTarget(target))
            {
                _currentTarget = nullptr;
                return;
            }
            
            _currentTarget = target;
        }

        Vector3d targePosition = target->getAimBonePosition();

        if(targePosition.x == 0 && targePosition.y == 0 && targePosition.z == 0){
            return;
        }

        Vector3d cameraPosition = LocalPlayer::getInstance().getCameraPosition();

        QAngle newAngle;

        if(!settings.isRage()) {
            newAngle = LocalPlayer::getInstance()
                .getViewAngle()
                .lookAt(cameraPosition,
                    targePosition,
                    settings.getSpeed() * TimeManager::getInstance().getDeltaTime(),
                    settings.getMaxAngleChangePerTick(),
                    settings.getVerticalPower(),
                    settings.getHorizontalPower());
        }
        else {
            newAngle = LocalPlayer::getInstance().getViewAngle().lookAt(cameraPosition, targePosition);
        }

        if(!newAngle.isValid()) {
            return;
        }
        
        LocalPlayer::getInstance().setViewAngle(newAngle);
    }

    QAngle predict(Vector3d start_pos, Vector3d target_pos, float projectile_speed, float gravity) {
        Vector3d delta_pos = target_pos - start_pos;
        float delta_x = delta_pos.y;
        float delta_y = delta_pos.x;
        float delta_z = delta_pos.z;

        float horizontal_distance = std::sqrt(delta_x * delta_x + delta_y * delta_y);

        float projectile_speed_squared = projectile_speed * projectile_speed;
        float a = projectile_speed_squared * projectile_speed_squared - gravity * (gravity * horizontal_distance * horizontal_distance + 2 * delta_y * projectile_speed_squared);
        float b = 2 * delta_x * projectile_speed_squared;
        float c = delta_x * delta_x + delta_z * delta_z;

        float pitch_rad, yaw_rad;
        if (a == 0) {
            pitch_rad = std::atan(projectile_speed_squared / gravity / horizontal_distance);
        } else {
            float discriminant = b * b - 4 * a * c;
            float t = (-b - std::sqrt(discriminant)) / (2 * a);
            float time_to_impact = t > 0 ? t : (-b + std::sqrt(discriminant)) / (2 * a);
            float vx = delta_x / time_to_impact;
            float vz = delta_z / time_to_impact;
            pitch_rad = std::atan2(projectile_speed_squared - gravity * time_to_impact, std::sqrt(vx * vx + vz * vz));
        }
        yaw_rad = std::atan2(delta_z, delta_x);
        return QAngle(yaw_rad * 180.0 / M_PI, pitch_rad * 180.0 / M_PI);
    }
    /*
    
     float bulletSpeed = curWeap.get_projectile_speed();
    float bulletGravity = curWeap.get_projectile_gravity();
    */
   
};

