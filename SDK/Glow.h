//
// Created by koelion on 05.02.24.
//

#ifndef APEX_GLOW_H
#define APEX_GLOW_H

#include <stdint.h>
#include "Offsets.h"
#include "../Math/Color.h"
#include "../Misc/Memory.h"

namespace SDK
{
    struct HilightFunctionBits
    {
        uint8_t Inside;
        uint8_t Outside;
        uint8_t Size;
        uint8_t Flags;

        HilightFunctionBits(uint8_t inside, uint8_t outside, uint8_t size, uint8_t flags)
                : Inside(inside), Outside(outside), Size(size), Flags(flags)
        {

        }
    };

    bool WriteGlowSettings(uintptr_t highlightSettingsPtr, uint8_t settingIndex, HilightFunctionBits hilightFunctionBits, Math::Color color)
    {
        uintptr_t settingsPtr = highlightSettingsPtr + OFF_HIGHLIGHT_TYPE_SIZE * settingIndex;
        if (!Memory::getInstance().write<HilightFunctionBits>(settingsPtr, hilightFunctionBits))
        {
            return false;
        }

        return Memory::getInstance().write<Math::Color>(settingsPtr + sizeof(HilightFunctionBits), color);
    }

    bool WriteGlowSettings(uintptr_t highlightSettingsPtr, uint8_t settingIndex, HilightFunctionBits hilightFunctionBits)
    {
        uintptr_t settingsPtr = highlightSettingsPtr + OFF_HIGHLIGHT_TYPE_SIZE * settingIndex;
        return Memory::getInstance().write<HilightFunctionBits>(settingsPtr, hilightFunctionBits);
    }
}

#endif //APEX_GLOW_H
