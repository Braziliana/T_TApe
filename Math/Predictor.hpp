#pragma once
#include "Vector3d.hpp"
#include "QAngle.hpp"

class Predictor
{
private:

public:

    static Vector3d predictPosition(const Vector3d& startPosition, const Vector3d& velocityVector, const float& time, const float& precision) {
        return startPosition + (velocityVector * time) * precision;
    }

    static float predictBulletDrop(const Vector3d& startPosition, const Vector3d& targetPosition, const float& bulletSpeed, const float& bulletDropRate) {
        float distance = startPosition.distanceTo(targetPosition);
        return (bulletDropRate / 2) * (distance * distance) / (bulletSpeed * bulletSpeed);
    }

    // QAngle predict(Vector3d start_pos, Vector3d target_pos, float projectile_speed, float gravity) {
    //     Vector3d delta_pos = target_pos - start_pos;
    //     float delta_x = delta_pos.y;
    //     float delta_y = delta_pos.x;
    //     float delta_z = delta_pos.z;

    //     float horizontal_distance = std::sqrt(delta_x * delta_x + delta_y * delta_y);

    //     float projectile_speed_squared = projectile_speed * projectile_speed;
    //     float a = projectile_speed_squared * projectile_speed_squared - gravity * (gravity * horizontal_distance * horizontal_distance + 2 * delta_y * projectile_speed_squared);
    //     float b = 2 * delta_x * projectile_speed_squared;
    //     float c = delta_x * delta_x + delta_z * delta_z;

    //     float pitch_rad, yaw_rad;
    //     if (a == 0) {
    //         pitch_rad = std::atan(projectile_speed_squared / gravity / horizontal_distance);
    //     } else {
    //         float discriminant = b * b - 4 * a * c;
    //         float t = (-b - std::sqrt(discriminant)) / (2 * a);
    //         float time_to_impact = t > 0 ? t : (-b + std::sqrt(discriminant)) / (2 * a);
    //         float vx = delta_x / time_to_impact;
    //         float vz = delta_z / time_to_impact;
    //         pitch_rad = std::atan2(projectile_speed_squared - gravity * time_to_impact, std::sqrt(vx * vx + vz * vz));
    //     }
    //     yaw_rad = std::atan2(delta_z, delta_x);
    //     return QAngle(yaw_rad * 180.0 / M_PI, pitch_rad * 180.0 / M_PI);
    // }

    /*

        Vector3 position(0, 0, 0);
        Vector3 target(10, 5, 20);
        float bullet_speed = 100.0f;
        float bullet_gravity = 9.81f;

        float pitch, yaw;
        CalculateTrajectory(position, target, bullet_speed, bullet_gravity, pitch, yaw);
    
    */

    // void CalculateTrajectory(Vector3d position, Vector3d target, float speed, float gravity, float& out_pitch, float& out_yaw) {
    //     Vector3d direction = (target - position).Normalized();
    //     float distance = (target - position).Magnitude();   
    //     float time = distance / speed;
    //     float height = position.y - target.y;
    //     float vertical_speed = (height + 0.5f * gravity * time * time) / time;
    //     Vector3d horizontal_direction(direction.x, 0.0f, direction.z);
    //     float horizontal_speed = speed / horizontal_direction.Magnitude();
    //     out_pitch = -atan2(vertical_speed, horizontal_speed);
    //     out_yaw = atan2(-direction.x, -direction.z);
    // }

    // void CalculateTrajectory(Vector3d position, Vector3d target, float speed, float gravity, float& out_pitch, float& out_yaw) {
    //     Vector3d direction(target.y - position.y, position.x - target.x, target.z - position.z);
    //     float distance = sqrt(direction.x * direction.x + direction.z * direction.z);
    //     float time = distance / speed;
    //     float height = position.z - target.z;
    //     float vertical_speed = (height + 0.5f * gravity * time * time) / time;
    //     Vector3d horizontal_direction(-direction.y, direction.x, 0.0f);
    //     float horizontal_speed = speed / sqrt(horizontal_direction.x * horizontal_direction.x + horizontal_direction.y * horizontal_direction.y);
    //     out_pitch = atan2(direction.z, distance);
    //     out_yaw = atan2(vertical_speed, horizontal_speed);
    // }

};
