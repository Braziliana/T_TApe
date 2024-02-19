//
// Created by koelion on 05.02.24.
//

#ifndef APEX_WEAPON_H
#define APEX_WEAPON_H
#include <stdint.h>
#include "../Misc/Memory.h"

#include "../Math/Vector3D.h"

#include "Offsets.h"

namespace SDK
{

    struct Weapon {
    private:
        uintptr_t ptr;
        ulong handle;
        int index;
        bool discarded;
        float projectileSpeed;
        float projectileScale;
        int ammoInClip;
        float currentZoomFov;
        float targetZoomFov;

        template <typename T>
        inline bool ReadOffset(uintptr_t offset, T* value) const
        {
            return Memory::getInstance().read<T>(ptr + offset, value);
        }

        inline bool ReadBasePointer(int weaponHandle)
        {
            ptr = 0x0;
            handle = weaponHandle & 0xFFFF;
            if (!Memory::getInstance().read<uintptr_t>(OFF_REGION + OFF_ENTITY_LIST + (handle << 5), &ptr))
            {
                return false;
            }
            return ptr != 0x0;
        }

    public:
        bool Update(int weaponHandle) {

            if (!ReadBasePointer(weaponHandle)) {
                ptr = 0x0;
                return false;
            }

            if (!ReadOffset(OFF_WEAPON_INDEX, &index)) {
                return false;
            }

            int discardedState = 0;
            if (!ReadOffset(OFF_WEAPON_DISCARDED, &discardedState)) {
                return false;
            }
            discarded = discardedState == 1;


            if (!ReadOffset(OFF_PROJECTILESPEED, &projectileSpeed)) {
                return false;
            }

            if (!ReadOffset(OFF_PROJECTILESCALE, &projectileScale)) {
                return false;
            }

            if (!ReadOffset(OFF_WEAPON_AMMO, &ammoInClip)) {
                return false;
            }

            if (!ReadOffset(OFF_WEAPON_CURRENT_ZOOM_FOV, &currentZoomFov)) {
                return false;
            }

            if (!ReadOffset(OFF_WEAPON_TARGET_ZOOM_FOV, &targetZoomFov)) {
                return false;
            }

            return true;
        }

        void SetInvalid() {
            ptr = 0x0;
        }

        bool IsValid() {
            return ptr != 0x0;
        }

        inline int Index() const {
            return index;
        }

        inline bool Discarded() const {
            return discarded;
        }

        inline float ProjectileSpeed() const {
            return projectileSpeed;
        }

        inline float ProjectileScale() const {
            return projectileScale;
        }

        inline int AmmoInClip() const {
            return ammoInClip;
        }

        inline float CurrentZoomFov() const {
            return currentZoomFov;
        }

        inline float TargetZoomFov() const {
            return targetZoomFov;
        }

        float GetTimeToTarget(const Math::Vector3D& startPosition, const Math::Vector3D& endPosition) {
            if (projectileSpeed <= 1.0f) {
                return 0;
            }

            const float distance = (endPosition - startPosition).Magnitude();
            if(distance < 15.0f){
                return 0;
            }

            return distance / projectileSpeed;
        }
    };

    typedef Weapon* PWeapon;
}

#endif //APEX_WEAPON_H
