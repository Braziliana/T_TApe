#pragma once
#include <cmath>
#include "Vector3d.hpp"
#include "QAngle.hpp"

class AimResolver
{
private:

public:

    static QAngle calcualteAngle(Vector3d from, Vector3d to) {
        Vector3d newDirection = to - from;
        newDirection.normalize();
        
        float pitch = -asinf(newDirection.z) * (180 / M_PI);
        float yaw = atan2f(newDirection.y, newDirection.x) * (180 / M_PI);

        return QAngle(pitch, yaw);
    }

    static Vector3d getTargetPosition(const Vector3d& targetPosition, Vector3d targetVelocity, float time) {
        return targetPosition + (targetVelocity * time);
    }

    static float getTimeToTarget(Vector3d startPosition, Vector3d endPosition, float bulletSpeed) {
        float distance = (endPosition - endPosition).length();

        return distance / bulletSpeed;
    }

    static float getBasicBulletDrop(Vector3d startPosition, Vector3d endPosition, float bulletSpeed, float bulletDropRate) {
        float time = getTimeToTarget(startPosition, endPosition, bulletSpeed);
        bulletDropRate *= 750.0f;

        return 0.5f * bulletDropRate * time * time;
    }

    static Vector3d getTargetPosition(Vector3d startPosition, Vector3d endPosition, Vector3d targetVelocity, float bulletSpeed) {
        float time = getTimeToTarget(startPosition, endPosition, bulletSpeed);

        return getTargetPosition(endPosition, targetVelocity, bulletSpeed);
    }

    //aim at moving target no bullet drop predicion
    static bool calculateAimRotation(Vector3d startPosition, Vector3d endPosition, Vector3d targetVelocity, float bulletSpeed, QAngle& result) {
        endPosition = getTargetPosition(startPosition, endPosition, targetVelocity, bulletSpeed);
        result = calcualteAngle(startPosition, endPosition);

        return true;
    }


    static void calcualteAimRotationBasicDrop(Vector3d startPosition, Vector3d endPosition, Vector3d targetVelocity, float bulletSpeed, float bulletScale, QAngle& result) {
        endPosition.z += getBasicBulletDrop(startPosition, endPosition, bulletSpeed, bulletScale);
        result = calcualteAngle(startPosition, endPosition);
    }

    /*The optimal angle formula involves finding the roots of the quadratic equation:
    g * d^2 + 2 * h * v^2 = v^4 / tan^2(a)
    where g is the gravitational acceleration, d is the horizontal distance to the target, h is the height difference to the target, v is the initial velocity of the projectile, and a is the firing angle.

    To solve for tan(a), we can rearrange this equation to the standard quadratic form:
    v^4 / tan^2(a) - g * d^2 - 2 * h * v^2 = 0

    which has coefficients:
    a = 1
    b = -2 * h * v^2 - g * d^2
    c = v^4

    We can then use the quadratic formula to find the roots of this equation:
    tan^2(a) = (-b +/- sqrt(b^2 - 4 * a * c)) / (2 * a)

    Taking the square root of both sides gives:
    tan(a) = sqrt((-b +/- sqrt(b^2 - 4 * a * c)) / (2 * a))

    This is equivalent to the optimal angle formula that I provided earlier:
    a = atan((v^2 +/- sqrt(v^4 - g * (g * d^2 + 2 * h * v^2))) / (g * d))

    */

    static bool calcualteAimRotationNew(Vector3d start, Vector3d targetPosition, Vector3d targetVelocity, float bulletSpeed, float bulletScale, int steps, QAngle& result) {
        const float gravity = 750.0f / bulletScale;

        float angle = 0;
        float time = 0.0;
        float timeStep = 1.0f / steps;

        for(int i = 0; i < steps; i++) {
            Vector3d predictedPosition = getTargetPosition(targetPosition, targetVelocity, time);
            if(!optimalAngle(start, predictedPosition, bulletSpeed, gravity, angle)){
                
                continue;
            }
            
            Vector3d direction = predictedPosition - start;
            float horizontalDistance = direction.length2D();
            float travelTime = horizontalDistance / (bulletSpeed * cosf(angle));

            if(travelTime <= time){
                result.x = -angle  * (180 / M_PI);
                result.y = atan2f(direction.y, direction.x)  * (180 / M_PI);
                return true;
            }

            time += timeStep;
        }

        targetPosition = getTargetPosition(start, targetPosition, targetVelocity, bulletSpeed);
        result = calcualteAngle(start, targetPosition);

        return true;
        //return false;
    }

    static float getTimeToTarget(const Vector3d start, const Vector3d end, const float speed, const float gravity) {

        float horizontalDistance = start.distance2DTo(end);
        float heightDifference = end.z - start.z;

        return (speed * sqrtf(2 * sinf(M_PI / 4) / gravity)) + (sqrtf(2 * sinf(M_PI / 4) * (sinf(M_PI / 4) - 2 * heightDifference) / gravity));
    }

    static bool optimalAngle(const Vector3d start, const Vector3d end, const float speed, const float gravity, float& angle) {

        float horizontalDistance = start.distance2DTo(end);
        float heightDifference = end.z - start.z;
        
        float root = powf(speed, 4) - gravity * (gravity * powf(horizontalDistance, 2) + 2 * heightDifference * powf(speed, 2));

        if(root < 0.0) {
            return false;
        }

        float term1 = powf(speed, 2) - sqrt(root);
        float term2 = gravity * horizontalDistance;

        angle = atanf(term1 / term2);

        return true;
    }

    static bool optimaAngle2(const Vector3d start, const Vector3d end, const float speed, const float gravity, float& angle) {
        float horizontalDistance = start.distance2DTo(end);
        float heightDifference = end.z - start.z;
        float time = horizontalDistance / (speed * cosf(atanf((heightDifference)/horizontalDistance)));
        angle = asinf((1.0f/2.0f)*(gravity*powf(time,2))/speed);
        //float maxHeight = speed*powf(sinf(angle),2)/(2*gravity);
        return true;
    }

    static bool optimaAngle3(const Vector3d start, const Vector3d end, const float speed, const float gravity, float& angle) {
        float horizontalDistance = start.distance2DTo(end);
        float heightDifference = end.z - start.z;

        float vSqr = speed * speed;
        float gSqr = gravity * gravity;
        float root = vSqr * vSqr - gSqr * (gSqr * horizontalDistance * horizontalDistance + 2 * heightDifference * vSqr);
        if(root < 0.0f) {
            return false;
        }

        float sqrt = sqrtf(root);
        float angle1 = atanf((vSqr - sqrt) / (gravity * horizontalDistance));

        if (!std::isnan(angle1) && angle1 >= 0 && angle1 <= M_PI / 2) {
            angle = angle1;
            return true;
        }

        float angle2 = atanf((vSqr + sqrt) / (gravity * horizontalDistance));
        
        if (!std::isnan(angle2) && angle2 >= 0 && angle2 <= M_PI / 2) {
            angle = angle2;
            return true;
        }

        return false;
    }

};
