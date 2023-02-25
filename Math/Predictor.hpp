#pragma once
#include "Vector3d.hpp"
#include "QAngle.hpp"

class Predictor
{
private:

public:
    // Vector3d PredictBulletDrop(const Vector3d& startPosition, const Vector3d& targetPosition, const float& bullerSpeed, const float& bulletDrop) {
        
    //     float y0 = startPosition.y;
    //     float y1 = targetPosition.y;

    //     double v0x = v0 * cos(theta);
    //     double v0y = v0 * sin(theta);
    //     double t_flight = x / v0x; // time of flight
    //     double drop = 0.5 * g * t_flight * t_flight; // drop of the bullet during flight
    //     double aim_point = y + drop; 

    //     return targetPosition;
    // }    

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
};
