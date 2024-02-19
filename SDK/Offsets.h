#pragma once

constexpr long OFF_REGION = 0x140000000;                      //[Static]->Region
constexpr long OFF_LEVEL = 0x16da6b0;                         //[Miscellaneous]->LevelName
constexpr long OFF_LOCAL_PLAYER = 0x2165e48;                  //[Miscellaneous]->LocalPlayer
constexpr long OFF_ENTITY_LIST = 0x1db73e8;                   //[Miscellaneous]->cl_entitylist
constexpr long OFF_GLOBAL_VARS = 0x16da1b0; //[Miscellaneous]->GlobalVars
//TODO
constexpr long OFF_CAMERAORIGIN = 0x1eb0;                     //[Miscellaneous]->CPlayer!camera_origin
constexpr long OFF_STUDIOHDR = 0xfd0;                         //[Miscellaneous]->CBaseAnimating!m_pStudioHdr

constexpr long OFF_INPUT_SYSTEM = 0x1759640; //[Miscellaneous] InputSystem
constexpr long OFF_MOUSE_SENSITIVITY = 0x02152a20; //mouse_sensitivity

// HUD
constexpr long OFF_VIEW_RENDER = 0x73df958;                    //[Miscellaneous]->ViewRender
constexpr long OFF_VIEW_MATRIX = 0x11a350;                     //[Miscellaneous]->ViewMatrix

// Buttons
constexpr long OFF_IN_ATTACK = 0x073e0bd8;                     //[Buttons]->in_attack
constexpr long OFF_IN_JUMP = 0x073e0cf0;                      //[Buttons]->in_jump
constexpr long OFF_IN_DUCK = 0x073e0dd8;                      //[Buttons]->in_duck
constexpr long OFFSET_IN_FORWARD = 0x073e0b28;                //[Buttons]->in_forward

// Player
constexpr long OFF_HEALTH = 0x0318;                           //[RecvTable.DT_Player]->m_iHealth
constexpr long OFF_MAXHEALTH = 0x0460;                        //[RecvTable.DT_Player]->m_iMaxHealth
constexpr long OFF_SHIELD = 0x01a0;                           //[RecvTable.DT_TitanSoul]->m_shieldHealth
constexpr long OFF_MAXSHIELD = 0x01a4;                        //[RecvTable.DT_TitanSoul]->m_shieldHealthMax
constexpr long OFF_BONES = 0x0d80 + 0x48;                    //m_nForceBone
constexpr long OFF_ABS_ORIGIN = 0x017c;                     //[DataMap.C_BaseEntity]->m_vecAbsOrigin
constexpr long OFF_ABSVELOCITY = 0x0170;                      //[DataMap.C_BaseEntity]->m_vecAbsVelocity
constexpr long OFF_ZOOMING = 0x1bb1;                          //[RecvTable.DT_Player]->m_bZooming
constexpr long OFF_TEAM_NUMBER = 0x0328;                      //[RecvTable.DT_BaseEntity]->m_iTeamNum
constexpr long OFF_LIFE_STATE = 0x0680;                       //[RecvTable.DT_Player]->m_lifeState
constexpr long OFF_BLEEDOUT_STATE = 0x26c0;                   //[RecvTable.DT_Player]->m_bleedoutState
constexpr long OFF_LAST_VISIBLE_TIME = 0x196d + 0x3;          //[RecvTable.DT_BaseCombatCharacter]->m_hudInfo_visibilityTestAlwaysPasses + 0x3
constexpr long OFF_LAST_AIMEDAT_TIME = 0x196d + 0x3 + 0x8;    //[RecvTable.DT_BaseCombatCharacter]->m_hudInfo_visibilityTestAlwaysPasses + 0x3 + 0x8
constexpr long OFF_VIEW_ANGLES = 0x2514 - 0x14;            //[DataMap.C_Player]-> m_ammoPoolCapacity - 0x14
constexpr long OFF_PUNCH_ANGLES = 0x2418;                     //[DataMap.C_Player]->m_currentFrameLocalPlayer.m_vecPunchWeapon_Angle

constexpr long OFF_TIME_BASE = 0x2068; //m_currentFramePlayer.timeBase

// Weapon
constexpr long OFF_WEAPON_HANDLE = 0x1914;                    //[RecvTable.DT_Player]->m_latestPrimaryWeapons
constexpr long OFF_WEAPON_INDEX = 0x1758;                     //[RecvTable.DT_WeaponX]->m_weaponNameIndex
constexpr long OFF_PROJECTILESCALE = 0x1e84;   //CWeaponX!m_flProjectileScale //0x04ec + 0x1978;         //projectile_gravity_scale + [WeaponSettingsMeta]base
constexpr long OFF_PROJECTILESPEED = 0x1e7c;//CWeaponX!m_flProjectileSpeed//0x04e4 + 0x1978;         //projectile_launch_speed + [WeaponSettingsMeta]base
constexpr long OFF_OFFHAND_WEAPON = 0x1924;                   //m_latestNonOffhandWeapons
constexpr long OFF_WEAPON_CURRENT_ZOOM_FOV = 0x15b0 + 0x00b8;          //m_playerData + m_curZoomFOV
constexpr long OFF_WEAPON_TARGET_ZOOM_FOV = 0x15b0 + 0x00bc;           //m_playerData + m_targetZoomFOV
constexpr long OFF_WEAPON_DISCARDED = 0x1579;                 //[RecvTable.DT_WeaponX]->m_discarded
constexpr long OFF_WEAPON_AMMO = 0x1560;                 //[RecvTable.DT_WeaponX]m_ammoInClip


//glow
constexpr long OFF_HIGHLIGHT_TYPE_SIZE = 0x34;
constexpr long OFF_GLOW_ENABLE = 0x28c;                       //[DT_HighlightSettings].?
constexpr long OFF_GLOW_THROUGH_WALL = 0x26c;                 //[DT_HighlightSettings].?
constexpr long OFF_GLOW_FIX = 0x268;
constexpr long OFF_GLOW_HIGHLIGHT_ID = 0x28C;                 //[DT_HighlightSettings].m_highlightServerActiveStates
constexpr long OFF_GLOW_HIGHLIGHTS = 0xB943CB0;


constexpr long OFF_TRAVERSAL_START_TIME = 0x2ad0;          //[RecvTable.DT_LocalPlayerExclusive]->m_traversalStartTime
constexpr long OFF_TRAVERSAL_PROGRESS = 0x2acc;            //[RecvTable.DT_LocalPlayerExclusive]->m_traversalProgress
constexpr long OFF_TRAVERSAL_RELEASE_TIME = 0x2ad8;		   //[RecvTable.DT_LocalPlayerExclusive]->m_traversalReleaseTime

constexpr long OFF_WALL_RUN_START_TIME = 0x355c;           //[RecvTable.DT_LocalPlayerExclusive]->m_wallRunStartTime
constexpr long OFF_WALL_RUN_CLEAR_TIME = 0x3560;           //[RecvTable.DT_LocalPlayerExclusive]->m_wallRunClearTime







// constexpr long OFF_LEVEL = 0x16d7de0;                         //[Miscellaneous]->LevelName
// constexpr long OFF_LOCAL_PLAYER = 0x2162818;                  //[Miscellaneous]->LocalPlayer
// constexpr long OFF_ENTITY_LIST = 0x1db4118;                   //[Miscellaneous]->cl_entitylist
// constexpr long OFF_NAME_LIST = 0xc5eb2d0;                     //[Miscellaneous]->NameList
// constexpr long OFF_NAME_INDEX = 0x38;                         //nameIndex
// constexpr long OFF_GLOBAL_VARS = 0x16d78e0; //[Miscellaneous]->GlobalVars

// // HUD
// constexpr long OFF_VIEW_RENDER = 0x73cd6e0;                    //[Miscellaneous]->ViewRenderer
// constexpr long OFF_VIEW_MATRIX = 0x11a350;                     //[Miscellaneous]->ViewMatrix

// // Buttons
// constexpr long OFF_IN_ATTACK = 0x073ce9a0;                     //[Buttons]->in_attack
// constexpr long OFF_IN_JUMP = 0x073ceaa0;                      //[Buttons]->in_jump
// constexpr long OFF_IN_DUCK = 0x073ceba0;                      //[Buttons]->in_duck
// constexpr long OFFSET_IN_FORWARD = 0x073ce8f0;                //[Buttons]->in_forward
// constexpr long OFF_GRAPPLE_ACTIVE = 0x2c98;		      //
// constexpr long OFF_GRAPPLE = 0x2c08;		              //
// constexpr long OFF_GRAPPLE_ATTACHED = 0x0048;		      //

// // Player
// constexpr long OFF_HEALTH = 0x02fc;                           //[RecvTable.DT_Player]->m_iHealth
// constexpr long OFF_MAXHEALTH = 0x0438;                        //[RecvTable.DT_Player]->m_iMaxHealth
// constexpr long OFF_SHIELD = 0x01a0;                           //[RecvTable.DT_TitanSoul]->m_shieldHealth
// constexpr long OFF_MAXSHIELD = 0x01a4;                        //[RecvTable.DT_TitanSoul]->m_shieldHealthMax
// constexpr long OFF_CAMERAORIGIN = 0x1e90;                     //[Miscellaneous]->CPlayer!camera_origin
// constexpr long OFF_STUDIOHDR = 0xfb0;                         //[Miscellaneous]->CBaseAnimating!m_pStudioHdr
// constexpr long OFF_BONES = 0x0d60 + 0x48;                    //m_nForceBone
// constexpr long OFF_ABS_ORIGIN = 0x017c;                     //[DataMap.C_BaseEntity]->m_vecAbsOrigin
// constexpr long OFF_ABSVELOCITY = 0x0170;                      //[DataMap.C_BaseEntity]->m_vecAbsVelocity
// constexpr long OFF_ZOOMING = 0x1b91;                          //[RecvTable.DT_Player]->m_bZooming
// constexpr long OFF_TEAM_NUMBER = 0x030c;                      //[RecvTable.DT_BaseEntity]->m_iTeamNum
// constexpr long OFF_NAME = 0x0449;                             //[RecvTable.DT_BaseEntity]->m_iName
// constexpr long OFF_LIFE_STATE = 0x0658;                       //[RecvTable.DT_Player]->m_lifeState
// constexpr long OFF_BLEEDOUT_STATE = 0x26a0;                   //[RecvTable.DT_Player]->m_bleedoutState
// constexpr long OFF_LAST_VISIBLE_TIME = 0x194d + 0x3;          //[RecvTable.DT_BaseCombatCharacter]->m_hudInfo_visibilityTestAlwaysPasses + 0x3
// constexpr long OFF_LAST_AIMEDAT_TIME = 0x194d + 0x3 + 0x8;    //[RecvTable.DT_BaseCombatCharacter]->m_hudInfo_visibilityTestAlwaysPasses + 0x3 + 0x8
// constexpr long OFF_VIEW_ANGLES = 0x24f4 - 0x14;            //[DataMap.C_Player]-> m_ammoPoolCapacity - 0x14
// constexpr long OFF_PUNCH_ANGLES = 0x23f8;                     //[DataMap.C_Player]->m_currentFrameLocalPlayer.m_vecPunchWeapon_Angle
// constexpr long OFF_YAW = 0x21fc - 0x8;                        //m_currentFramePlayer.m_ammoPoolCount - 0x8
// constexpr long LOCAL_ORIGIN = 0x0188;                         // [DataMap.CBaseViewModel] -> m_localOrigin
// constexpr long OFF_MODELNAME = 0x0030;                        //m_ModelName

// constexpr long OFF_TRAVERSAL_START_TIME = 0x2ac0;          //[RecvTable.DT_LocalPlayerExclusive]->m_traversalStartTime
// constexpr long OFF_TRAVERSAL_PROGRESS = 0x2abc;            //[RecvTable.DT_LocalPlayerExclusive]->m_traversalProgress
// constexpr long OFF_TRAVERSAL_RELEASE_TIME = 0x2ac8;		   //[RecvTable.DT_LocalPlayerExclusive]->m_traversalReleaseTime

// constexpr long OFF_WALL_RUN_START_TIME = 0x3524;           //[RecvTable.DT_LocalPlayerExclusive]->m_wallRunStartTime
// constexpr long OFF_WALL_RUN_CLEAR_TIME = 0x3528;           //[RecvTable.DT_LocalPlayerExclusive]->m_wallRunClearTime
// constexpr long OFF_TIME_BASE = 0x2048;                        //m_currentFramePlayer.timeBase


// // Weapon
// constexpr long OFF_WEAPON_HANDLE = 0x18f4;                    //[RecvTable.DT_Player]->m_latestPrimaryWeapons
// constexpr long OFF_WEAPON_INDEX = 0x1738;                     //[RecvTable.DT_WeaponX]->m_weaponNameIndex
// constexpr long OFF_PROJECTILESCALE = 0x1e64;   //CWeaponX!m_flProjectileSpeed //0x04ec + 0x1978;         //projectile_gravity_scale + [WeaponSettingsMeta]base
// constexpr long OFF_PROJECTILESPEED = 0x1e5c;//CWeaponX!m_flProjectileSpeed//0x04e4 + 0x1978;         //projectile_launch_speed + [WeaponSettingsMeta]base
// constexpr long OFF_OFFHAND_WEAPON = 0x1904;                   //m_latestNonOffhandWeapons
// constexpr long OFF_WEAPON_CURRENT_ZOOM_FOV = 0x1590 + 0x00b8;          //m_playerData + m_curZoomFOV
// constexpr long OFF_WEAPON_TARGET_ZOOM_FOV = 0x1590 + 0x00bc;           //m_playerData + m_targetZoomFOV
// constexpr long OFF_SKIN = 0x0d18;                             //m_nSkin
// constexpr long OFF_WEAPON_DISCARDED = 0x1559;                 //[RecvTable.DT_WeaponX]->m_discarded
// constexpr long OFF_WEAPON_AMMO = 0x1540;                 //[RecvTable.DT_WeaponX]m_ammoInClip


// //glow
// constexpr long OFF_HIGHLIGHT_TYPE_SIZE = 0x34;
// constexpr long OFF_GLOW_ENABLE = 0x28c;                       //[DT_HighlightSettings].?
// constexpr long OFF_GLOW_THROUGH_WALL = 0x26c;                 //[DT_HighlightSettings].?
// constexpr long OFF_GLOW_FIX = 0x268;
// constexpr long OFF_GLOW_HIGHLIGHT_ID = 0x28C;                 //[DT_HighlightSettings].m_highlightServerActiveStates
// constexpr long OFF_GLOW_HIGHLIGHTS = 0xB93F2D0;

// constexpr long OFF_INPUT_SYSTEM = 0x1756ec0;
// constexpr long mouse_sensitivity=0x0214f5d0;