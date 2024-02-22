//
// Created by koelion on 05.02.24.
//

#ifndef APEX_PLAYER_H
#define APEX_PLAYER_H
#include <stdint.h>
#include <cmath>
#include "../Misc/Memory.h"

#include "../Math/Vector3D.h"
#include "../Math/Matrix.h"

#include "Offsets.h"
#include "HitboxType.h"

#include "../Misc/Logger.h"
#include "../Misc/Timer.h"

namespace SDK
{
    struct Player
    {
    private:
        uintptr_t ptr;
        int index;

        bool dead;
        bool knocked;
        int team;
        int health;
        int maxHealth;
        int shield;
        int maxShield;

        int lastTimeVisible;
        int lastTimeVisiblePrev;
        bool visible;

        int lastTimeAimedAt;
        int lastTimeAimedAtPrevious;
        bool isAimedAt;

        Math::Vector3D vecAbsOrigin;
        Math::Vector3D vecAbsVelocity;//OFF_ABSVELOCITY

        int headBone = -1;
        int neckBone = -1;
        int upperChestBone = -1;
        int lowerChestBone = -1;
        int stomachBone = -1;
        int hipBone = -1;

        float lastVisiblityCheck = -1;

        template <typename T>
        inline bool ReadOffset(uintptr_t offset, T* value) const
        {
            return Memory::getInstance().read<T>(ptr + offset, value);
        }

        inline bool ReadBasePointer(int entityIndex)
        {
            index = entityIndex;
            if (!Memory::getInstance().read<uintptr_t>(OFF_REGION + OFF_ENTITY_LIST + ((entityIndex + 1) << 5), &ptr))
            {
                ptr = 0;
            }
            return ptr != 0x0;
        }

        template <typename T>
        inline bool Write(uintptr_t offset, T value) const
        {
            return Memory::getInstance().write(ptr + offset, value);
        }

        void Reset()
        {
            ptr = 0x0;
            headBone = -1;
            neckBone = -1;
            upperChestBone = -1;
            lowerChestBone = -1;
            stomachBone = -1;
            hipBone = -1;
        }

    public:

        Player()
        {
            ptr = 0;
            index = 0;

            dead = true;
            knocked = false;
            team = 0;
            health = 0;
            shield = 0;

            lastTimeVisible = 0;
            lastTimeVisiblePrev = 0;
            visible = false;

            vecAbsOrigin = Math::Vector3D(0, 0, 0);
            vecAbsVelocity = Math::Vector3D(0, 0, 0);
        }

        bool Update(int entityIndex, PTimer timer)
        {
            if (!ReadBasePointer(entityIndex)) {
                Reset();
                return false;
            }

            int8_t lifeState = 0;
            if (!ReadOffset(OFF_LIFE_STATE, &lifeState)) {
                Reset();
                return false;
            }

            dead = lifeState > 0;
            if (dead) {
                Reset();
                return true;
            }

            int8_t bleedoutState = 0;
            if (!ReadOffset(OFF_BLEEDOUT_STATE, &bleedoutState)) {
                Reset();
                return false;
            }

            knocked = bleedoutState > 0;

            if (!ReadOffset(OFF_TEAM_NUMBER, &team)) {
                Reset();
                return false;
            }

            if (!ReadOffset(OFF_HEALTH, &health)) {
                Reset();
                return false;
            }


            if (!ReadOffset(OFF_MAXHEALTH, &maxHealth)) {
                Reset();
                return false;
            }

            if (!ReadOffset(OFF_SHIELD, &shield)) {
                Reset();
                return false;
            }

            if (!ReadOffset(OFF_MAXSHIELD, &maxShield)) {
                Reset();
                return false;
            }


            if (!ReadOffset(OFF_ABS_ORIGIN, &vecAbsOrigin)) {
                Reset();
                return false;
            }

            if (!ReadOffset(OFF_ABSVELOCITY, &vecAbsVelocity)) {
                Reset();
                return false;
            }


            if (!ReadOffset(OFF_LAST_AIMEDAT_TIME, &lastTimeAimedAt)) {
                Reset();
                return false;
            }

            isAimedAt = lastTimeAimedAtPrevious < lastTimeAimedAt;
            lastTimeAimedAtPrevious = lastTimeAimedAt;

            if(lastVisiblityCheck < 0 || lastVisiblityCheck + 0.005f < timer->Time())
            {
                if (!ReadOffset(OFF_LAST_VISIBLE_TIME, &lastTimeVisible)) {
                    return false;
                }
                visible = isAimedAt || lastTimeVisiblePrev < lastTimeVisible;
                lastTimeVisiblePrev = lastTimeVisible;
            }
            else
            {
                visible = visible || isAimedAt;
            }

            return true;
        }

        inline int GetIndex() const {
            return index;
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

        inline int GetMaxHealth() const {
            return maxHealth;
        }

        inline int GetShield() const {
            return shield;
        }

        inline int GetMaxShield() const {
            return maxShield;
        }

        inline float GetLastTimeVisible() const {
            return lastTimeVisible;
        }

        inline bool IsVisible() const {
            return visible;
        }

        inline bool IsAimedAt() const {
            return isAimedAt;
        }

        inline bool IsValid() const {
            return ptr != 0x0;
        }

        inline Math::Vector3D GetPosition() const {
            return vecAbsOrigin;
        }

        inline Math::Vector3D GeVelocity() const {
            return vecAbsVelocity;
        }

        int GetBoneFromHitbox(HitboxType hitboxType) {
            if (ptr == 0x0) {
                return -1;
            }

            uintptr_t modelPtr = 0x0;
            if (!ReadOffset<uintptr_t>(OFF_STUDIOHDR, &modelPtr) || modelPtr == 0x0) {
                return -1;
            }

            uintptr_t studioHdr = 0x0;
            if (!Memory::getInstance().read<uintptr_t>(modelPtr + 0x8, &studioHdr) || studioHdr == 0x0) {
                return -1;
            }

            uint16_t hitboxCache = 0;
            if (!Memory::getInstance().read<uint16_t>(studioHdr + 0x34, &hitboxCache)) {
                return -1;
            }
            auto hitboxArray = studioHdr + ((uint16_t)(hitboxCache & 0xFFFE) << (4 * (hitboxCache & 1)));
            if (hitboxArray == 0x0) {
                return -1;
            }

            uint16_t indexCache = 0;
            if (!Memory::getInstance().read<uint16_t>(hitboxArray + 0x4, &indexCache))
            {
                return -1;
            }

            auto hitboxIndex = ((uint16_t)(indexCache & 0xFFFE) << (4 * (indexCache & 1)));
            auto bonePtr = hitboxArray + hitboxIndex + (static_cast<int>(hitboxType) * 0x20);

            uint16_t bone = 0;
            if (!Memory::getInstance().read<uint16_t>(bonePtr, &bone))
            {
                return -1;
            }

            return bone;
        }

        int GetCachedBone(HitboxType hitboxType)
        {
            switch (hitboxType) {

                case HitboxType::Head:
                    if(headBone == -1)
                    {
                        headBone = GetBoneFromHitbox(HitboxType::Head);
                    }
                    return headBone;
                case HitboxType::Neck:
                    if(neckBone == -1)
                    {
                        neckBone = GetBoneFromHitbox(HitboxType::Neck);
                    }
                    return neckBone;
                case HitboxType::UpperChest:
                    if(upperChestBone == -1)
                    {
                        upperChestBone = GetBoneFromHitbox(HitboxType::UpperChest);
                    }
                    return upperChestBone;
                case HitboxType::LowerChest:
                    if(lowerChestBone == -1)
                    {
                        lowerChestBone = GetBoneFromHitbox(HitboxType::LowerChest);
                    }
                    return lowerChestBone;
                case HitboxType::Stomach:
                    if(stomachBone == -1)
                    {
                        stomachBone = GetBoneFromHitbox(HitboxType::Stomach);
                    }
                    return stomachBone;
                case HitboxType::Hip:
                    if(hipBone == -1)
                    {
                        hipBone = GetBoneFromHitbox(HitboxType::Head);
                    }
                    return hipBone;
            }

            return -1;
        }

        bool GetBonePosition(HitboxType hitboxType, Math::PVector3D bonePosition) {
            Math::Vector3D offset = Math::Vector3D(1.5f, 0.0f, 0.0f);

            auto bone = GetCachedBone(hitboxType);
            if (bone == -1) {
                return false;
            }

            uintptr_t bonesPtr = 0;

            if (!ReadOffset<uintptr_t>(OFF_BONES, &bonesPtr) || bonesPtr == 0x0) {
                return false;
            }

            Math::Matrix3x4 boneMatrix = {};
            if (!Memory::getInstance().read<Math::Matrix3x4>(bonesPtr + bone * sizeof(Math::Matrix3x4), &boneMatrix))
            {
                return false;
            }

            bonePosition->CopyFrom(vecAbsOrigin + boneMatrix.getPosition());
            return bonePosition->IsValid();
        }

        inline float GetDistance(Math::Vector3D targetPosition) const {
            return this->vecAbsOrigin.Distance(targetPosition);
        }

        inline float GetDistanceInMeters(Math::Vector3D targetPosition) const {
            const float inchesToMeter = 39.3701f;
            return GetDistance(targetPosition) / inchesToMeter;
        }
        //7 = enabled, 2 = disabled
        inline bool SetGlowEnabled(int glowEnable)
        {
            return Write<int>(OFF_GLOW_ENABLE, glowEnable);
        }
        //2 = enabled, 5 = disabled
        inline bool SetGlowThroughWall(int glowThroughWall)
        {
            return Write<int>(OFF_GLOW_THROUGH_WALL, glowThroughWall);
        }

        bool SetGlow(uint8_t settingIndex)
        {
            if (!SetGlowEnabled(7))
            {
                return false;
            }

            if (!SetGlowThroughWall(2))
            {
                return false;
            }

            if (!Write<uint8_t>(OFF_GLOW_HIGHLIGHT_ID, settingIndex))
            {
                return false;
            }

            return Write<int>(OFF_GLOW_FIX, 0);
        }

        bool DisableGlow() {
            if (!SetGlowEnabled(2))
            {
                return false;
            }

            if (!SetGlowThroughWall(5))
            {
                return false;
            }

            return true;
        }
    };

    typedef Player* PPlayer;
}

#endif //APEX_PLAYER_H
