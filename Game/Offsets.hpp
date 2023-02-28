#pragma once

class Offsets
{
private:
    Offsets() {
        glowEnable += highlightServerContextID;
        glowThroughWall += highlightServerContextID;
        glowColor += highlightParams;
        glowMode += highlightFunctionBits;
        breathAngles = viewAngle - 0x10;
    }

    ~Offsets() {}
public:
    static Offsets& getInstance() {
        static Offsets instance;
        return instance;
    }

    Offsets(const Offsets&) = delete;
    Offsets& operator=(const Offsets&) = delete;

    ulong region = 0x140000000;

    ulong inAttack = 0x076782c8; //[Buttons] in_attack=0x076782c8

    ulong level = 0x1462450;//Miscellaneous LevelName=0x1462450

    ulong localPlayer = 0x01ee8cb0 + 0x8; //0x1388eb8 //.?AVC_GameMovement@@=0x01ee8cb0

    ulong entityList = 0x1b37a78; //cl_entitylist=0x1b37938

    ulong viewRender = 0x7677d20; //[Miscellaneous] ViewRender=0x7677288
    ulong viewMatrix = 0x11a210; //[Miscellaneous] ViewMatrix=0x11a210

    ulong lifeState = 0x0798; //m_lifeState=0x0798
    ulong bleedOutState = 0x2740; //m_bleedoutState=0x2740
    ulong health = 0x043c; //[RecvTable.DT_Player] m_iHealth=0x043c
    ulong maxHealth = 0x0578; //[RecvTable.DT_Player] m_iMaxHealth=0x0578
    ulong shield = 0x0170; //[RecvTable.DT_TitanSoul] m_shieldHealth=0x0170
    ulong maxShield = 0x0174; //[RecvTable.DT_TitanSoul] m_shieldHealthMax=0x0174
    ulong lastVisibleTime = 0x1A78;
    ulong viewAngle = 0x25ac - 0x14; //
    ulong weaponPunch = 0x24b0; //m_currentFrameLocalPlayer.m_vecPunchWeapon_Angle=0x24b0
    ulong breathAngles = 0x0;
    ulong team = 0x044c; //m_iTeamNum=0x044c
    ulong localOrigin = 0x0158; //m_localOrigin=0x0158
    ulong vecAbsOrigin = 0x014c; //m_vecAbsOrigin=0x014c
    ulong vecAbsVelocity = 0x0140; //[RecvTable.DT_BaseEntity] m_vecAbsVelocity
    ulong bones = 0x0e98 + 0x48; //m_bConstrainBetweenEndpoints=0x0f50
    ulong studioHdr = 0x10f0; //CBaseAnimating!m_pStudioHdr=0x10f0
    ulong cameraPosition = 0x1f48; //[Miscellaneous] CPlayer!camera_origin=0x1f48
    ulong cameraAngles = 0x1f54; //[Miscellaneous] CPlayer!camera_angles=0x1f54
    ulong name = 0x0589; //[RecvTable.DT_BaseEntity] m_iName
    ulong latestPrimaryWeapons = 0x1a1c; //[RecvTable.DT_BaseCombatCharacter] m_latestPrimaryWeapons



    ulong highlightServerContextID = 0x03c0; //[RecvTable.DT_HighlightSettings] m_highlightServerContextID=0x03c0
    ulong glowEnable = 0x008;
    ulong glowThroughWall = 0x0010;

    ulong highlightParams = 0x01b8; //[RecvTable.DT_HighlightSettings] m_highlightParams=0x01b8
    ulong glowColor = 0x0018;

    ulong highlightFunctionBits=0x02c0; // [RecvTable.DT_HighlightSettings] m_highlightFunctionBits=0x02c0
    ulong glowMode = 0x0004;
    
    ulong highlightServerFadeEndTimes = 0x0380;
    ulong glowDistance = 0x34; // [RecvTable.DT_HighlightSettings]m_highlightServerFadeEndTimes + 52(0x34);

    ulong projectileSpeed = 0x1f18; //CWeaponX!m_flProjectileSpeed=0x1f18
    ulong projectileScale = 0x1f20; //CWeaponX!m_flProjectileScale=0x1f20
    ulong ammoInClip = 0x1670;
    //[RecvTable.DT_PropSurvival] m_customScriptInt=0x1648

    /*
    
     
[RecvTable.DT_WeaponX_LocalWeaponData]
m_nNextThinkTick=0x0574
m_lastPrimaryAttackTime=0x1644
m_nextReadyTime=0x1648
m_nextPrimaryAttackTime=0x164c
m_attackTimeThisFrame=0x1650
m_ammoInClip=0x1670
m_ammoInStockpile=0x1674
m_lifetimeShots=0x1678
m_flTimeWeaponIdle=0x167c
m_bInReload=0x1686
    */

};
