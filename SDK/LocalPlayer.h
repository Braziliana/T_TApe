//
// Created by koelion on 05.02.24.
//

#ifndef APEX_LOCALPLAYER_H
#define APEX_LOCALPLAYER_H
#include <stdint.h>
#include "../Misc/Memory.h"

#include "../Math/Vector3D.h"
#include "../Math/Matrix.h"

#include "../Misc/Logger.h"

#include "Offsets.h"
#include "HitboxType.h"
#include "Weapon.h"

namespace SDK
{
    struct LocalPlayer
    {
    private:
        uintptr_t ptr;
        int index;

        bool dead;
        bool knocked;
        int team;
        int health;
        int shield;

        Math::Vector3D position;
        Math::Vector3D cameraPosition;
        Math::QAngle viewAngle;
        Math::QAngle punchAngle;
        Math::QAngle lastPunch;

        Weapon weapon;
        bool isZooming;

        inline bool ReadBasePointer()
        {
            ptr = 0x0;
            if (!Memory::getInstance().read<uintptr_t>(OFF_REGION + OFF_LOCAL_PLAYER, &ptr))
            {
                return false;
            }
            return ptr != 0x0;
        }

        template <typename T>
        inline bool Write(uintptr_t offset, T value) const
        {
            return Memory::getInstance().write(ptr + offset, value);
        }

    public:

        LocalPlayer()
        {
            ptr = 0;
            index = 0;

            dead = 0;
            knocked = 0;
            team = 0;
            health = 0;
            shield = 0;

            position = Math::Vector3D(0, 0, 0);
            cameraPosition = Math::Vector3D(0, 0, 0);
            viewAngle = Math::QAngle(0, 0);
            punchAngle = Math::QAngle(0, 0);
            lastPunch = Math::QAngle(0, 0);

            weapon = Weapon();

            isZooming = false;
        }

        template <typename T>
        inline bool ReadOffset(uintptr_t offset, T* value) const
        {
            return Memory::getInstance().read<T>(ptr + offset, value);
        }

        bool Update()
        {
            if (!ReadBasePointer()) {
                ptr = 0x0;
                return false;
            }

            int8_t lifeState = 0;
            if (!ReadOffset(OFF_LIFE_STATE, &lifeState)) {
                return false;
            }

            dead = lifeState > 0;
            if (dead) {
                return true;
            }

            int8_t bleedoutState = 0;
            if (!ReadOffset(OFF_BLEEDOUT_STATE, &bleedoutState)) {
                return false;
            }

            knocked = bleedoutState > 0;

            if (!ReadOffset(OFF_TEAM_NUMBER, &team)) {
                return false;
            }

            if (!ReadOffset(OFF_HEALTH, &health)) {
                return false;
            }

            if (!ReadOffset(OFF_SHIELD, &shield)) {
                return false;
            }


            if (!ReadOffset(OFF_ABS_ORIGIN, &position)) {
                return false;
            }

            if (!ReadOffset(OFF_CAMERAORIGIN, &cameraPosition)) {
                return false;
            }

            if (!ReadOffset(OFF_VIEW_ANGLES, &viewAngle)) {
                return false;
            }
            if (viewAngle.isValid()) {
                viewAngle = viewAngle.fixAngle();
            }

            lastPunch = punchAngle;
            if (!ReadOffset(OFF_PUNCH_ANGLES, &punchAngle)) {
                return false;
            }
            if (punchAngle.isValid()) {
                punchAngle = punchAngle.fixAngle();
            }

            int8_t zoomingState = 0;
            if (!ReadOffset(OFF_ZOOMING, &zoomingState)) {
                return false;
            }

            int weaponHandle = 0;
            if (ReadOffset(OFF_WEAPON_HANDLE, &weaponHandle)) {
                if (!weapon.Update(weaponHandle)) {
                }
            }
            else {
                weapon.SetInvalid();
            }

            isZooming = zoomingState > 0;


            return true;
        }

        inline bool IsDead() const {
            return dead;
        }

        inline bool IsKnocked() const {
            return knocked;
        }

        inline int GetTeam() const {
            return team;
        }

        inline int GetHealth() const {
            return health;
        }

        inline int GetShield() const {
            return shield;
        }

        inline bool IsValid() const {
            return ptr != 0x0;
        }

        inline Math::Vector3D GetPosition() const {
            return position;
        }

        inline Math::Vector3D GetCameraPosition() const {
            return cameraPosition;
        }

        inline float GetDistance(Math::Vector3D targetPosition) const {
            return this->cameraPosition.Distance(targetPosition);
        }

        inline float GetDistanceInMeters(Math::Vector3D targetPosition) const {
            const float inchesToMeter = 39.3701f;
            return GetDistance(targetPosition) / inchesToMeter;
        }

        inline Math::QAngle GetViewAngle() const {
            return viewAngle;
        }

        inline bool SetViewAngle(Math::QAngle angle) {
            if (!angle.isValid()) {
                return false;
            }

            angle = angle.fixAngle();

            if (Write(OFF_VIEW_ANGLES, angle)) {
                viewAngle = angle;
                return true;
            }

            return false;
        }

        Math::QAngle GetAngleTo(Math::Vector3D targetPosition) {
            return cameraPosition.GetAngleTo(targetPosition);
        }

        inline Math::QAngle GetPunchAngle() const {
            return punchAngle;
        }


        inline Math::QAngle GetPunchAngleDiffrence() const {
            return punchAngle - lastPunch;
        }

        PWeapon GetWeapon() {
            return &weapon;
        }

        inline bool IsZooming() const {
            return isZooming;
        }
    };

    typedef LocalPlayer* PLocalPlayer;
}

#endif //APEX_LOCALPLAYER_H
