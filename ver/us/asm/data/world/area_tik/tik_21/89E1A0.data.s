.include "macro.inc"

.section .data

dlabel D_80240A00_89E1A0
.word 0x0000004D, 0x00000001, 0x00000000, 0x0000000A, 0x00000002, 0xFE363C85, 0x00000001, 0x0000000A, 0x00000002, 0xFE363C86, 0x00000001, 0x0000000A, 0x00000002, 0xFE363C87, 0x00000001, 0x0000000A, 0x00000002, 0xFE363C88, 0x00000001, 0x00000043, 0x00000001, tik_21_UpdateTexturePanSmooth, 0x00000002, 0x00000000, 0x00000013, 0x00000000, 0x00000013, 0x00000000, 0x00000013, 0x00000000, 0x00000013, 0x00000000, 0x00000043, 0x00000001, tik_21_UpdateTexturePanStepped, 0x00000002, 0x00000000, 0x00000001, 0x00000000

dlabel D_80240A9C_89E23C
.word 0x00000043, 0x00000002, DisablePlayerInput, 0x00000001, 0x00000043, 0x00000002, DisablePlayerPhysics, 0x00000001, 0x00000043, 0x00000002, DisablePartnerAI, 0x00000000, 0x00000043, 0x00000002, HidePlayerShadow, 0x00000001, 0x00000043, 0x00000002, SetPlayerAnimation, 0x00010000, 0x00000043, 0x00000002, GetCurrentPartnerID, 0xFE363C80, 0x0000000B, 0x00000002, 0xFE363C80, 0x00000000, 0x00000043, 0x00000003, EnableNpcShadow, 0xFFFFFFFC, 0x00000000, 0x00000043, 0x00000005, SetNpcPos, 0xFFFFFFFC, 0x00000000, 0xFFFFFC18, 0x00000000, 0x00000013, 0x00000000, 0x00000043, 0x00000002, GetEntryID, 0xFE363C80, 0x00000043, 0x00000001, tik_21_GetEntryPos, 0x00000028, 0x00000002, 0xFE363C82, 0x00000028, 0x00000043, 0x00000004, SetPlayerPos, 0xFE363C81, 0xFE363C82, 0xFE363C83, 0x00000043, 0x00000003, InterpPlayerYaw, 0xFE363C84, 0x00000000, 0x00000043, 0x00000002, PlaySound, 0x00000163, 0x00000043, 0x00000002, func_802D286C, 0x00000100, 0x00000043, 0x00000007, func_802D2520, 0x00010000, 0x00000005, 0x00000002, 0x00000001, 0x00000001, 0x00000000, 0x00000043, 0x00000004, GetPlayerPos, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000005, 0x00000001, 0x00000028, 0x00000027, 0x00000002, 0xFE363C81, 0x00000001, 0x00000043, 0x00000004, SetPlayerPos, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000008, 0x00000001, 0x00000001, 0x00000006, 0x00000000, 0x00000008, 0x00000001, 0x00000003, 0x00000043, 0x00000002, GetCurrentPartnerID, 0xFE363C80, 0x0000000B, 0x00000002, 0xFE363C80, 0x00000000, 0x00000056, 0x00000000, 0x00000043, 0x00000002, DisablePartnerAI, 0x00000000, 0x00000043, 0x00000004, GetPlayerPos, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000028, 0x00000002, 0xFE363C82, 0x00000003, 0x00000043, 0x00000005, SetNpcPos, 0xFFFFFFFC, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000043, 0x00000004, SetNpcFlagBits, 0xFFFFFFFC, 0x00000002, 0x00000000, 0x00000043, 0x00000001, EnablePartnerAI, 0x00000043, 0x00000003, EnableNpcShadow, 0xFFFFFFFC, 0x00000001, 0x00000057, 0x00000000, 0x00000013, 0x00000000, 0x00000008, 0x00000001, 0x00000002, 0x00000043, 0x00000007, func_802D2520, 0x00010000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000008, 0x00000001, 0x00000001, 0x00000043, 0x00000002, SetPlayerAnimation, 0x00010002, 0x00000043, 0x00000002, DisablePlayerPhysics, 0x00000000, 0x00000043, 0x00000002, DisablePlayerInput, 0x00000000, 0x00000043, 0x00000002, HidePlayerShadow, 0x00000000, 0x00000003, 0x00000001, 0x00000000, 0x00000043, 0x00000001, tik_21_GetCurrentFloor, 0x00000008, 0x00000001, 0x00000001, 0x0000000B, 0x00000002, 0xFE363C80, 0xFFFFFFFF, 0x00000004, 0x00000001, 0x00000000, 0x00000013, 0x00000000, 0x00000044, 0x00000001, 0xFE363C8A, 0x00000002, 0x00000000, 0x00000001, 0x00000000, 0x00000043, 0x00000002, DisablePlayerInput, 0x00000001, 0x00000043, 0x00000002, DisablePlayerPhysics, 0x00000001, 0x00000043, 0x00000002, HidePlayerShadow, 0x00000001, 0x00000043, 0x00000004, ModifyColliderFlags, 0x00000000, 0xFE363C8B, 0x7FFFFE00, 0x00000043, 0x00000002, GetEntryID, 0xFE363C80, 0x00000043, 0x00000001, tik_21_GetEntryPos, 0x00000024, 0x00000002, 0xFE363C85, 0xFE363C81, 0x00000024, 0x00000002, 0xFE363C86, 0xFE363C82, 0x00000024, 0x00000002, 0xFE363C87, 0xFE363C83, 0x00000027, 0x00000002, 0xFE363C82, 0x00000002, 0x00000043, 0x00000004, SetPlayerPos, 0xFE363C81, 0xFE363C82, 0xFE363C83, 0x00000043, 0x00000003, InterpPlayerYaw, 0xFE363C84, 0x00000000, 0x0000000A, 0x00000002, 0xFE363C84, 0x0000005A, 0x00000027, 0x00000002, 0xFE363C85, 0x00000028, 0x00000012, 0x00000000, 0x00000028, 0x00000002, 0xFE363C85, 0x00000028, 0x00000013, 0x00000000, 0x00000043, 0x00000005, UseSettingsFrom, 0x00000000, 0xFE363C85, 0xFE363C86, 0xFE363C87, 0x00000043, 0x00000005, SetPanTarget, 0x00000000, 0xFE363C85, 0xFE363C86, 0xFE363C87, 0x00000043, 0x00000003, SetCamSpeed, 0x00000000, 0xF24BE280, 0x00000043, 0x00000004, PanToTarget, 0x00000000, 0x00000000, 0x00000001, 0x00000043, 0x00000002, GetCurrentPartnerID, 0xFE363C80, 0x0000000B, 0x00000002, 0xFE363C80, 0x00000000, 0x00000043, 0x00000002, DisablePartnerAI, 0x00000000, 0x00000043, 0x00000003, EnableNpcShadow, 0xFFFFFFFC, 0x00000000, 0x00000043, 0x00000005, SetNpcPos, 0xFFFFFFFC, 0x00000000, 0xFFFFFC18, 0x00000000, 0x00000043, 0x00000004, InterpNpcYaw, 0xFFFFFFFC, 0xFE363C80, 0x00000000, 0x00000013, 0x00000000, 0x00000008, 0x00000001, 0x00000001, 0x00000043, 0x00000002, PlaySound, 0x00000163, 0x00000056, 0x00000000, 0x00000008, 0x00000001, 0x00000019, 0x00000043, 0x00000002, HidePlayerShadow, 0x00000000, 0x00000057, 0x00000000, 0x00000043, 0x00000002, func_802D286C, 0x00000900, 0x00000043, 0x00000007, func_802D2520, 0x00010002, 0x00000005, 0x00000003, 0x00000001, 0x00000001, 0x00000000, 0x00000005, 0x00000001, 0x00000028, 0x00000043, 0x00000002, tik_21_SomeXYZFunc2, 0xF24A7E80, 0x00000043, 0x00000004, SetPlayerPos, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000008, 0x00000001, 0x00000001, 0x00000006, 0x00000000, 0x00000043, 0x00000002, GetCurrentPartnerID, 0xFE363C80, 0x0000000B, 0x00000002, 0xFE363C80, 0x00000000, 0x00000056, 0x00000000, 0x00000043, 0x00000004, GetPlayerPos, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000028, 0x00000002, 0xFE363C82, 0x00000003, 0x00000043, 0x00000005, SetNpcPos, 0xFFFFFFFC, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000043, 0x00000003, EnableNpcShadow, 0xFFFFFFFC, 0x00000001, 0x00000043, 0x00000001, EnablePartnerAI, 0x00000057, 0x00000000, 0x00000013, 0x00000000, 0x00000008, 0x00000001, 0x00000005, 0x00000043, 0x00000007, func_802D2520, 0x00010002, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000043, 0x00000004, ModifyColliderFlags, 0x00000001, 0xFE363C8B, 0x7FFFFE00, 0x00000043, 0x00000002, DisablePlayerInput, 0x00000000, 0x00000043, 0x00000002, DisablePlayerPhysics, 0x00000000, 0x00000043, 0x00000004, PanToTarget, 0x00000000, 0x00000000, 0x00000000, 0x00000044, 0x00000001, 0xFE363C8A, 0x00000002, 0x00000000, 0x00000001, 0x00000000

dlabel D_802411E0_89E980
.word 0x00000043, 0x00000001, tik_21_UnkFunc25, 0x0000000A, 0x00000002, 0xFE363C80, 0x00000000, 0x00000002, 0x00000000, 0x00000013, 0x00000000, 0x00000043, 0x00000002, GetCurrentPartner, 0xFE363C80, 0x0000000B, 0x00000002, 0xFE363C80, 0x00000000, 0x00000043, 0x00000002, GetCurrentPartnerID, 0xFE363C81, 0x0000000B, 0x00000002, 0xFE363C81, 0x00000006, 0x00000002, 0x00000000, 0x00000012, 0x00000000, 0x00000043, 0x00000001, func_802D2B6C, 0x00000043, 0x00000002, DisablePlayerInput, 0x00000001, 0x00000013, 0x00000000, 0x00000012, 0x00000000, 0x00000043, 0x00000002, DisablePlayerInput, 0x00000001, 0x00000013, 0x00000000, 0x00000046, 0x00000001, D_802412BC_89EA5C, 0x00000002, 0x00000000, 0x00000001, 0x00000000

dlabel D_802412BC_89EA5C
.word 0x00000043, 0x00000001, tik_21_SetPlayerStatusAnimFlags100000, 0x0000004D, 0x00000001, 0x0000001B, 0x00000043, 0x00000002, DisablePlayerPhysics, 0x00000001, 0x00000043, 0x00000002, HidePlayerShadow, 0x00000001, 0x00000024, 0x00000002, 0xFE363C80, 0xFE363C8A, 0x00000043, 0x00000001, tik_21_GetEntryPos, 0x00000043, 0x00000004, PlayerMoveTo, 0xFE363C81, 0xFE363C83, 0x00000003, 0x00000024, 0x00000002, 0xFE363C80, 0xFE363C8A, 0x00000043, 0x00000001, tik_21_GetEntryPos, 0x00000043, 0x00000004, SetPlayerPos, 0xFE363C81, 0xFE363C82, 0xFE363C83, 0x00000043, 0x00000003, SetPlayerFlagBits, 0x00200000, 0x00000001, 0x00000043, 0x00000001, tik_21_GetCurrentCameraYawClamped180, 0x00000043, 0x00000003, InterpPlayerYaw, 0xFE363C80, 0x00000000, 0x00000008, 0x00000001, 0x00000002, 0x00000043, 0x00000003, SetPlayerFlagBits, 0x00200000, 0x00000000, 0x00000043, 0x00000002, PlaySound, 0x00000163, 0x00000043, 0x00000004, GetPlayerPos, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000056, 0x00000000, 0x00000008, 0x00000001, 0x00000004, 0x00000005, 0x00000001, 0x00000028, 0x00000028, 0x00000002, 0xFE363C81, 0x00000001, 0x00000043, 0x00000004, SetPlayerPos, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000008, 0x00000001, 0x00000001, 0x00000006, 0x00000000, 0x00000057, 0x00000000, 0x00000043, 0x00000002, func_802D286C, 0x00000800, 0x00000043, 0x00000007, func_802D2520, 0x00010002, 0x00000005, 0x00000002, 0x00000001, 0x00000001, 0x00000000, 0x00000008, 0x00000001, 0x00000019, 0x00000046, 0x00000001, 0xFE363C8C, 0x00000002, 0x00000000, 0x00000001, 0x00000000, 0x00000043, 0x00000002, IsPlayerOnValidFloor, 0xFE363C80, 0x0000000A, 0x00000002, 0xFE363C80, 0x00000000, 0x00000002, 0x00000000, 0x00000013, 0x00000000, 0x00000043, 0x00000002, GetPlayerActionState, 0xFE363C80, 0x0000000A, 0x00000002, 0xFE363C80, 0x0000001A, 0x00000002, 0x00000000, 0x00000013, 0x00000000, 0x00000043, 0x00000002, GetCurrentPartner, 0xFE363C80, 0x0000000B, 0x00000002, 0xFE363C80, 0x00000000, 0x00000043, 0x00000002, GetCurrentPartnerID, 0xFE363C81, 0x0000000B, 0x00000002, 0xFE363C81, 0x00000006, 0x00000002, 0x00000000, 0x00000012, 0x00000000, 0x00000043, 0x00000001, func_802D2B6C, 0x00000043, 0x00000002, DisablePlayerInput, 0x00000001, 0x00000013, 0x00000000, 0x00000012, 0x00000000, 0x00000043, 0x00000002, DisablePlayerInput, 0x00000001, 0x00000013, 0x00000000, 0x0000004D, 0x00000001, 0x0000001B, 0x00000043, 0x00000001, tik_21_SetPlayerStatusAnimFlags100000, 0x00000043, 0x00000002, DisablePlayerPhysics, 0x00000001, 0x00000043, 0x00000004, ModifyColliderFlags, 0x00000000, 0xFE363C8B, 0x7FFFFE00, 0x00000024, 0x00000002, 0xFE363C80, 0xFE363C8A, 0x00000043, 0x00000001, tik_21_GetEntryPos, 0x00000024, 0x00000002, 0xFE363C85, 0xFE363C81, 0x00000024, 0x00000002, 0xFE363C86, 0xFE363C82, 0x00000027, 0x00000002, 0xFE363C86, 0x00000002, 0x00000024, 0x00000002, 0xFE363C87, 0xFE363C83, 0x00000024, 0x00000002, 0xFE363C88, 0xFE363C84, 0x00000027, 0x00000002, 0xFE363C88, 0x000000B4, 0x0000000F, 0x00000002, 0xFE363C84, 0x00000168, 0x00000028, 0x00000002, 0xFE363C84, 0x00000168, 0x00000013, 0x00000000, 0x00000043, 0x00000003, InterpPlayerYaw, 0xFE363C88, 0x00000001, 0x00000008, 0x00000001, 0x00000001, 0x00000043, 0x00000002, PlaySound, 0x00000163, 0x00000043, 0x00000004, GetPlayerPos, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000043, 0x00000004, SetPlayerPos, 0xFE363C80, 0xFE363C86, 0xFE363C87, 0x00000043, 0x00000002, SetPlayerAnimation, 0x00010000, 0x00000043, 0x00000002, func_802D286C, 0x00000800, 0x00000043, 0x00000007, func_802D2520, 0x00010000, 0x00000005, 0x00000003, 0x00000001, 0x00000001, 0x00000000, 0x00000056, 0x00000000, 0x00000008, 0x00000001, 0x00000008, 0x00000043, 0x00000002, HidePlayerShadow, 0x00000001, 0x00000057, 0x00000000, 0x00000056, 0x00000000, 0x00000008, 0x00000001, 0x00000003, 0x00000005, 0x00000001, 0x00000028, 0x00000043, 0x00000002, tik_21_SomeXYZFunc2, 0xF24A7E80, 0x00000043, 0x00000004, SetPlayerPos, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000008, 0x00000001, 0x00000001, 0x00000006, 0x00000000, 0x00000057, 0x00000000, 0x00000008, 0x00000001, 0x00000019, 0x00000046, 0x00000001, 0xFE363C8C, 0x00000002, 0x00000000, 0x00000001, 0x00000000

dlabel D_802417CC_89EF6C
.word 0x0000004D, 0x00000001, 0x0000001B, 0x00000043, 0x00000003, UseExitHeading, 0x0000003C, 0x00000000, 0x00000044, 0x00000001, ExitWalk, 0x00000043, 0x00000003, GotoMap, 0x80242700, 0x00000001, 0x00000008, 0x00000001, 0x00000064, 0x00000002, 0x00000000, 0x00000001, 0x00000000

dlabel D_80241828_89EFC8
.word 0x0000004D, 0x00000001, 0x0000001B, 0x00000043, 0x00000002, DisablePlayerInput, 0x00000001, 0x00000043, 0x00000002, DisablePlayerInput, 0x00000001, 0x00000024, 0x00000002, 0xFE363C80, 0x00000001, 0x00000024, 0x00000002, 0xFE363C81, 0x0000000E, 0x00000024, 0x00000002, 0xFE363C82, 0x00000009, 0x00000024, 0x00000002, 0xFE363C83, 0x0000000B, 0x00000044, 0x00000001, ExitDoubleDoor, 0x00000008, 0x00000001, 0x00000011, 0x00000043, 0x00000003, GotoMap, 0x80242708, 0x00000000, 0x00000008, 0x00000001, 0x00000064, 0x00000002, 0x00000000, 0x00000001, 0x00000000

dlabel D_802418DC_89F07C
.word 0x00000043, 0x00000003, GotoMap, 0x80242710, 0x00000000, 0x00000008, 0x00000001, 0x00000064, 0x00000002, 0x00000000, 0x00000001, 0x00000000

dlabel D_8024190C_89F0AC
.word 0x0000004D, 0x00000001, 0x0000001B, 0x00000024, 0x00000002, 0xFE363C8A, 0x00000002, 0x00000024, 0x00000002, 0xFE363C8B, 0x00000010, 0x00000024, 0x00000002, 0xFE363C8C, D_802418DC_89F07C, 0x00000046, 0x00000001, D_802411E0_89E980, 0x00000002, 0x00000000, 0x00000001, 0x00000000

dlabel D_80241964_89F104
.word 0x00000043, 0x00000002, DisablePlayerInput, 0x00000001, 0x00000043, 0x00000004, ShowMessageAtScreenPos, 0x001D0177, 0x000000A0, 0x00000028, 0x00000043, 0x00000002, DisablePlayerInput, 0x00000000, 0x00000002, 0x00000000, 0x00000001, 0x00000000

dlabel D_802419AC_89F14C
.word 0x00000047, 0x00000005, D_802417CC_89EF6C, 0x00080000, 0x00000007, 0x00000001, 0x00000000, 0x0000000F, 0x00000002, 0xF5DE0180, 0x0000003D, 0x00000047, 0x00000005, D_80241828_89EFC8, 0x00000100, 0x0000000E, 0x00000001, 0x00000000, 0x00000012, 0x00000000, 0x00000047, 0x00000005, D_80241964_89F104, 0x00000100, 0x0000000E, 0x00000001, 0x00000000, 0x00000013, 0x00000000, 0x00000047, 0x00000005, D_8024190C_89F0AC, 0x00000080, 0x00000010, 0x00000001, 0x00000000, 0x00000002, 0x00000000, 0x00000001, 0x00000000

dlabel D_80241A4C_89F1EC
.word 0x00000043, 0x00000002, GetEntryID, 0xFE363C80, 0x00000014, 0x00000001, 0xFE363C80, 0x00000016, 0x00000001, 0x00000000, 0x00000024, 0x00000002, 0xFE363C80, D_802419AC_89F14C, 0x00000044, 0x00000001, EnterWalk, 0x00000016, 0x00000001, 0x00000001, 0x00000024, 0x00000002, 0xFE363C82, 0x00000009, 0x00000024, 0x00000002, 0xFE363C83, 0x0000000B, 0x00000046, 0x00000001, EnterDoubleDoor, 0x00000044, 0x00000001, D_802419AC_89F14C, 0x00000016, 0x00000001, 0x00000002, 0x00000024, 0x00000002, 0xFE363C8A, D_802419AC_89F14C, 0x00000044, 0x00000001, D_80240A9C_89E23C, 0x00000023, 0x00000000, 0x00000008, 0x00000001, 0x00000001, 0x00000002, 0x00000000, 0x00000001, 0x00000000

dlabel D_80241B20_89F2C0
.word 0x00000024, 0x00000002, 0xFE363C81, 0xF4ACD485, 0x00000027, 0x00000002, 0xFE363C81, 0x00000001, 0x00000024, 0x00000002, 0xFE363C82, 0xF4ACD480, 0x00000024, 0x00000002, 0xFE363C83, 0xF4ACD481, 0x00000024, 0x00000002, 0xFE363C84, 0xF4ACD485, 0x00000027, 0x00000002, 0xFE363C84, 0x00000002, 0x00000024, 0x00000002, 0xFE363C85, 0xF4ACD480, 0x00000024, 0x00000002, 0xFE363C86, 0xF4ACD481, 0x00000024, 0x00000002, 0xFE363C87, 0xF4ACD485, 0x00000027, 0x00000002, 0xFE363C87, 0x00000003, 0x00000024, 0x00000002, 0xFE363C88, 0xF4ACD480, 0x00000024, 0x00000002, 0xFE363C89, 0xF4ACD481, 0x00000024, 0x00000002, 0xFE363C8A, 0xF4ACD485, 0x00000027, 0x00000002, 0xFE363C8A, 0x00000004, 0x00000024, 0x00000002, 0xFE363C8B, 0xF4ACD480, 0x00000024, 0x00000002, 0xFE363C8C, 0xF4ACD481, 0x00000043, 0x00000003, EnableModel, 0xFE363C81, 0x00000001, 0x00000043, 0x00000003, EnableModel, 0xFE363C84, 0x00000001, 0x00000043, 0x00000003, EnableModel, 0xFE363C87, 0x00000001, 0x00000043, 0x00000003, EnableModel, 0xFE363C8A, 0x00000001, 0x00000005, 0x00000001, 0x00000005, 0x00000027, 0x00000002, 0xFE363C80, 0x00000001, 0x00000027, 0x00000002, 0xFE363C82, 0x00000001, 0x00000027, 0x00000002, 0xFE363C83, 0x00000001, 0x00000043, 0x00000005, TranslateModel, 0xFE363C81, 0xFE363C82, 0xFE363C80, 0xFE363C83, 0x00000043, 0x00000005, ScaleModel, 0xFE363C81, 0xF24A7C80, 0xF24A7C80, 0xF24A7C80, 0x00000027, 0x00000002, 0xFE363C85, 0xFFFFFFFF, 0x00000027, 0x00000002, 0xFE363C86, 0x00000001, 0x00000043, 0x00000005, TranslateModel, 0xFE363C84, 0xFE363C85, 0xFE363C80, 0xFE363C86, 0x00000043, 0x00000005, ScaleModel, 0xFE363C84, 0xF24A7C80, 0xF24A7C80, 0xF24A7C80, 0x00000027, 0x00000002, 0xFE363C88, 0x00000001, 0x00000027, 0x00000002, 0xFE363C89, 0xFFFFFFFF, 0x00000043, 0x00000005, TranslateModel, 0xFE363C87, 0xFE363C88, 0xFE363C80, 0xFE363C89, 0x00000043, 0x00000005, ScaleModel, 0xFE363C87, 0xF24A7C80, 0xF24A7C80, 0xF24A7C80, 0x00000027, 0x00000002, 0xFE363C8B, 0xFFFFFFFF, 0x00000027, 0x00000002, 0xFE363C8C, 0xFFFFFFFF, 0x00000043, 0x00000005, TranslateModel, 0xFE363C8A, 0xFE363C8B, 0xFE363C80, 0xFE363C8C, 0x00000043, 0x00000005, ScaleModel, 0xFE363C8A, 0xF24A7C80, 0xF24A7C80, 0xF24A7C80, 0x00000008, 0x00000001, 0x00000001, 0x00000006, 0x00000000, 0x00000005, 0x00000001, 0x00000005, 0x00000027, 0x00000002, 0xFE363C80, 0xFFFFFFFF, 0x00000027, 0x00000002, 0xFE363C82, 0x00000001, 0x00000027, 0x00000002, 0xFE363C83, 0x00000001, 0x00000043, 0x00000005, TranslateModel, 0xFE363C81, 0xFE363C82, 0xFE363C80, 0xFE363C83, 0x00000043, 0x00000005, ScaleModel, 0xFE363C81, 0xF24A7C80, 0xF24A7C80, 0xF24A7C80, 0x00000027, 0x00000002, 0xFE363C85, 0xFFFFFFFF, 0x00000027, 0x00000002, 0xFE363C86, 0x00000001, 0x00000043, 0x00000005, TranslateModel, 0xFE363C84, 0xFE363C85, 0xFE363C80, 0xFE363C86, 0x00000043, 0x00000005, ScaleModel, 0xFE363C84, 0xF24A7C80, 0xF24A7C80, 0xF24A7C80, 0x00000027, 0x00000002, 0xFE363C88, 0x00000001, 0x00000027, 0x00000002, 0xFE363C89, 0xFFFFFFFF, 0x00000043, 0x00000005, TranslateModel, 0xFE363C87, 0xFE363C88, 0xFE363C80, 0xFE363C89, 0x00000043, 0x00000005, ScaleModel, 0xFE363C87, 0xF24A7C80, 0xF24A7C80, 0xF24A7C80, 0x00000027, 0x00000002, 0xFE363C8B, 0xFFFFFFFF, 0x00000027, 0x00000002, 0xFE363C8C, 0xFFFFFFFF, 0x00000043, 0x00000005, TranslateModel, 0xFE363C8A, 0xFE363C8B, 0xFE363C80, 0xFE363C8C, 0x00000043, 0x00000005, ScaleModel, 0xFE363C8A, 0xF24A7C80, 0xF24A7C80, 0xF24A7C80, 0x00000008, 0x00000001, 0x00000001, 0x00000006, 0x00000000, 0x00000043, 0x00000003, EnableModel, 0xFE363C81, 0x00000000, 0x00000043, 0x00000003, EnableModel, 0xFE363C84, 0x00000000, 0x00000043, 0x00000003, EnableModel, 0xFE363C87, 0x00000000, 0x00000043, 0x00000003, EnableModel, 0xFE363C8A, 0x00000000, 0x00000002, 0x00000000, 0x00000001, 0x00000000

dlabel D_80241FF0_89F790
.word 0x0000004D, 0x00000001, 0x00000000, 0x0000003C, 0x00000001, 0xFE363C8A, 0x00000024, 0x00000002, 0xFE363C85, 0xF4ACD485, 0x00000005, 0x00000001, 0x00000005, 0x00000043, 0x00000003, CloneModel, 0x00000013, 0xFE363C85, 0x00000043, 0x00000005, TranslateModel, 0xFE363C85, 0xF4ACD480, 0xF4ACD482, 0xF4ACD481, 0x00000027, 0x00000002, 0xFE363C85, 0x00000001, 0x00000006, 0x00000000, 0x00000003, 0x00000001, 0x00000000, 0x00000043, 0x00000003, RandInt, 0x000000C8, 0xFE363C80, 0x00000027, 0x00000002, 0xFE363C80, 0x00000032, 0x00000008, 0x00000001, 0xFE363C80, 0x00000043, 0x00000003, EnableModel, 0xF4ACD485, 0x00000001, 0x00000043, 0x00000005, MakeLerp, 0xF4ACD482, 0xF4ACD483, 0xF4ACD484, 0x00000001, 0x00000003, 0x00000001, 0x00000001, 0x00000043, 0x00000001, UpdateLerp, 0x00000043, 0x00000005, TranslateModel, 0xF4ACD485, 0xF4ACD480, 0xFE363C80, 0xF4ACD481, 0x00000008, 0x00000001, 0x00000001, 0x00000043, 0x00000002, tik_21_UnkPosFunc2, 0xF4ACD485, 0x0000000A, 0x00000002, 0xFE363C82, 0x00000001, 0x00000004, 0x00000001, 0x0000000A, 0x00000013, 0x00000000, 0x0000000A, 0x00000002, 0xFE363C81, 0x00000001, 0x00000004, 0x00000001, 0x00000001, 0x00000013, 0x00000000, 0x00000003, 0x00000001, 0x0000000A, 0x00000043, 0x00000003, EnableModel, 0xF4ACD485, 0x00000000, 0x00000043, 0x00000002, PlaySound, 0x000003F6, 0x00000046, 0x00000001, D_80241B20_89F2C0, 0x00000004, 0x00000001, 0x00000000, 0x00000002, 0x00000000, 0x00000001, 0x00000000

dlabel D_802421C8_89F968
.word 0x00000024, 0x00000002, 0xFE363C8F, 0x00002710, 0x00000030, 0x00000001, 0xFE363C80, 0x00000031, 0x00000001, 0xFE363C81, 0x00000005, 0x00000001, 0xFE363C81, 0x00000034, 0x00000004, 0xFE363C82, 0xFE363C83, 0xFE363C84, 0xFE363C85, 0x00000033, 0x00000003, 0xFE363C86, 0xFE363C87, 0xFE363C88, 0x00000031, 0x00000001, 0xFE363C89, 0x00000005, 0x00000001, 0xFE363C89, 0x0000003E, 0x00000002, 0x00000006, 0xFE363C8A, 0x0000003C, 0x00000001, 0xFE363C8A, 0x00000043, 0x00000003, RandInt, 0xFE363C84, 0xFE363C8B, 0x00000043, 0x00000003, RandInt, 0xFE363C85, 0xFE363C8C, 0x00000024, 0x00000002, 0xFE363C8D, 0xFE363C82, 0x00000024, 0x00000002, 0xFE363C8E, 0xFE363C83, 0x00000027, 0x00000002, 0xFE363C8D, 0xFE363C8B, 0x00000027, 0x00000002, 0xFE363C8E, 0xFE363C8C, 0x00000024, 0x00000002, 0xF4ACD480, 0xFE363C8D, 0x00000024, 0x00000002, 0xF4ACD481, 0xFE363C8E, 0x00000024, 0x00000002, 0xF4ACD482, 0xFE363C86, 0x00000024, 0x00000002, 0xF4ACD483, 0xFE363C87, 0x00000024, 0x00000002, 0xF4ACD484, 0xFE363C88, 0x00000024, 0x00000002, 0xF4ACD485, 0xFE363C8F, 0x00000044, 0x00000001, D_80241FF0_89F790, 0x00000027, 0x00000002, 0xFE363C8F, 0x00000005, 0x00000006, 0x00000000, 0x00000006, 0x00000000, 0x00000043, 0x00000003, EnableModel, 0x00000013, 0x00000000, 0x00000002, 0x00000000, 0x00000001, 0x00000000

dlabel D_80242374_89FB14
.word 0x00000002, 0xFFFFFEC0, 0xFFFFFFED, 0x0000003C, 0x0000002E, 0x000000C8, 0xFFFFFFEC, 0x0000003C, 0x00000002, 0x000000E9, 0xFFFFFF9C, 0x0000002D, 0x000000EB, 0x000000C8, 0xFFFFFFEC, 0x0000003C, 0x00000001

dlabel D_802423B8_89FB58
.word 0x00000024, 0x00000002, 0xFE363C80, D_80242374_89FB14, 0x00000044, 0x00000001, D_802421C8_89F968, 0x00000002, 0x00000000, 0x00000001, 0x00000000, 0x00000024, 0x00000002, 0xF5DE0329, 0x00000002, 0x00000043, 0x00000002, SetSpriteShading, 0x0000000F, 0x00000043, 0x00000006, SetCamPerspective, 0x00000000, 0x00000003, 0x00000019, 0x00000010, 0x00001000, 0x00000043, 0x00000005, SetCamBGColor, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000043, 0x00000003, SetCamEnabled, 0x00000000, 0x00000001, 0x00000043, 0x00000003, SetCamLeadPlayer, 0x00000000, 0x00000000, 0x00000046, 0x00000001, 0x802425D0, 0x00000044, 0x00000001, 0x802409D0, 0x00000043, 0x00000002, UseDoorSounds, 0x00000000, 0x00000044, 0x00000001, D_802423B8_89FB58, 0x00000043, 0x00000003, SetTexPanner, 0x00000011, 0x00000000, 0x00000056, 0x00000000, 0x00000024, 0x00000002, 0xFE363C80, 0x00000000, 0x00000024, 0x00000002, 0xFE363C81, 0x00000000, 0x00000024, 0x00000002, 0xFE363C82, 0xFFFFFF38, 0x00000024, 0x00000002, 0xFE363C83, 0xFFFFFF9C, 0x00000024, 0x00000002, 0xFE363C84, 0xFFFFFE0C, 0x00000024, 0x00000002, 0xFE363C85, 0x00000000, 0x00000024, 0x00000002, 0xFE363C86, 0x00000001, 0x00000024, 0x00000002, 0xFE363C87, 0x00000001, 0x00000024, 0x00000002, 0xFE363C88, 0x00000001, 0x00000024, 0x00000002, 0xFE363C89, 0x00000000, 0x00000024, 0x00000002, 0xFE363C8A, 0x00000000, 0x00000024, 0x00000002, 0xFE363C8B, 0x00000000, 0x00000024, 0x00000002, 0xFE363C8C, 0x00000000, 0x00000044, 0x00000001, D_80240A00_89E1A0, 0x00000057, 0x00000000, 0x00000008, 0x00000001, 0x00000001, 0x00000044, 0x00000001, D_80241A4C_89F1EC, 0x00000002, 0x00000000, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000
