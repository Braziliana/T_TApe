#pragma once
#include "../Utils/Types.hpp"

enum class HighlightFill : byte {
    FillNone = 0,
    CustomColor = 101,
    InteractButton = 103,
    LobbyInMatch = 109,
    Sonar = 103,
    LootDefault = 135,
    LootFocused = 136,
    CryptoDrone = 139,
    Objective = 126,
    MenuModelReveal = 75,
    Bloodhound = 12,
    BloodhoundClone = 13,
    CausticThreat = 133,
    CausticCanister = 134,
    Crypto = 133
};

enum class HighlightOutline : byte {
    FillNone = 0,
    CustomColor = 101,
    CustomColorWeaponPickup = 110,
    CustomColorPulse = 120,
    CustomColorObeyZ = 121,
    CustomColorOccludedNoScanlines = 129,
    CustomColorNozNoScanlines = 169,
    Sonar = 103,
    InteractButton = 105,
    Objective = 125,
    VmCustomColor = 114,
    LootDefault = 135,
    LootFocused = 136,
    CryptoDrone = 140,
    MenuModelReveal = 75
};

struct GlowMode
{
public:
	HighlightFill generalGlowMode;
    HighlightOutline borderGlowMode;
    byte borderSize;
    byte transparentLevel;

    GlowMode() : generalGlowMode(HighlightFill::CustomColor), borderGlowMode(HighlightOutline::CustomColor), borderSize(0), transparentLevel(0) { }
    GlowMode(HighlightFill generalGlowMode, HighlightOutline borderGlowMode, byte borderSize, byte transparentLevel) :
        generalGlowMode(generalGlowMode), borderGlowMode(borderGlowMode), borderSize(borderSize), transparentLevel(transparentLevel) { }

    bool operator==(const GlowMode& other) const {
        return generalGlowMode == other.generalGlowMode &&
            borderGlowMode == other.borderGlowMode &&
            borderSize == borderSize &&
            transparentLevel == transparentLevel;
    }
};