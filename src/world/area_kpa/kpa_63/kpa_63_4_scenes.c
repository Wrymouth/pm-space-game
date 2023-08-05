#include "kpa_63.h"
#include "world/common/npc/StarSpirit.h"

EvtScript N(NpcInit_EldStar) = {
    EVT_RETURN
    EVT_END
};

NpcSettings N(NpcSettings_EldStar) = {
    .height = 64,
    .radius = 28,
    .level = 99,
};

NpcData N(NpcData_EldStar) = {
    .id = 2,
    .pos = { 0.0f, -213.0f, 16.0f },
    .init = &N(NpcInit_EldStar),
    .yaw = 270,
    .settings = &N(NpcSettings_EldStar),
    .flags = ENEMY_FLAG_PASSIVE | ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_ENABLE_HIT_SCRIPT,
    .drops = NO_DROPS,
    .animations = ELDSTAR_ANIMS,
    .aiDetectFlags = AI_DETECT_SIGHT,
};

NpcGroupList N(DefaultNpcs) = {
    NPC_GROUP(N(NpcData_EldStar)),
    {}
};

API_CALLABLE(N(SetPassengerPos)) {
    Bytecode* args = script->ptrReadPos;
    Npc* partner;
    f32 x, y, z;
    f32 yBase;
    f32 angle;
    f32 radius;

    if (isInitialCall) {
        script->functionTemp[0] = evt_get_variable(script, *args++);
        yBase = evt_get_variable(script, *args++);
        angle = evt_get_variable(script, *args++);
        switch (script->functionTemp[0]) {
            case 0:
                radius = 0.0f;
                break;
            case 1:
                radius = 5.0f;
                break;
        }
    }

    x = (sin_deg(angle) * radius) + -120.0f;
    y = yBase - 11.0f;
    z = (cos_deg(angle) * radius) + 225.0f;

    switch (script->functionTemp[0]) {
        case 0:
            gPlayerStatus.pos.x = x;
            gPlayerStatus.pos.y = y;
            gPlayerStatus.pos.z = z;
            break;
        case 1:
            partner = get_npc_safe(NPC_PARTNER);
            if (partner != NULL) {
                partner->pos.x = x;
                partner->pos.y = y;
                partner->pos.z = z;
                partner->colliderPos.x = partner->pos.x;
                partner->colliderPos.y = partner->pos.y;
                partner->colliderPos.z = partner->pos.z;
                partner->flags |= NPC_FLAG_DIRTY_SHADOW;
            }
            break;
        default:
            return ApiStatus_DONE2;
    }
    return ApiStatus_DONE2;
}

EvtScript N(EVS_UpdatePassengers) = {
    EVT_IF_EQ(MV_PlayerOnBoard, TRUE)
        EVT_CALL(N(SetPassengerPos), 0, LVar3, MV_Starship_Yaw)
    EVT_END_IF
    EVT_IF_EQ(MV_PartnerOnBoard, TRUE)
        EVT_CALL(N(SetPassengerPos), 1, LVar3, MV_Starship_Yaw)
    EVT_END_IF
    EVT_RETURN
    EVT_END
};

EvtScript N(EVS_Starship_Update) = {
    EVT_LABEL(0)
    EVT_CALL(MakeLerp, 0, 100, 30, EASING_COS_IN_OUT)
    EVT_LOOP(0)
        EVT_CALL(UpdateLerp)
        EVT_SETF(LVar3, LVar0)
        EVT_MULF(LVar3, EVT_FLOAT(0.1))
        EVT_ADDF(LVar3, MV_Starship_PosY)
        EVT_CALL(TranslateGroup, MODEL_g55, 0, LVar3, 0)
        EVT_CALL(RotateGroup, MODEL_g55, MV_Starship_Yaw, 0, 1, 0)
        EVT_EXEC(N(EVS_UpdatePassengers))
        EVT_WAIT(1)
        EVT_IF_EQ(LVar1, 0)
            EVT_BREAK_LOOP
        EVT_END_IF
    EVT_END_LOOP
    EVT_CALL(MakeLerp, 100, 0, 30, EASING_COS_IN_OUT)
    EVT_LOOP(0)
        EVT_CALL(UpdateLerp)
        EVT_SETF(LVar3, LVar0)
        EVT_MULF(LVar3, EVT_FLOAT(0.1))
        EVT_ADDF(LVar3, MV_Starship_PosY)
        EVT_CALL(TranslateGroup, MODEL_g55, 0, LVar3, 0)
        EVT_CALL(RotateGroup, MODEL_g55, MV_Starship_Yaw, 0, 1, 0)
        EVT_EXEC(N(EVS_UpdatePassengers))
        EVT_WAIT(1)
        EVT_IF_EQ(LVar1, 0)
            EVT_BREAK_LOOP
        EVT_END_IF
    EVT_END_LOOP
    EVT_GOTO(0)
    EVT_RETURN
    EVT_END
};

EvtScript N(EVS_Starship_Depart) = {
    EVT_CALL(DisablePlayerInput, TRUE)
    EVT_CALL(HidePlayerShadow, TRUE)
    EVT_CALL(GetModelCenter, MODEL_o308)
    EVT_ADD(LVar0, -5)
    EVT_CALL(SetPlayerJumpscale, 0)
    EVT_CALL(PlayerJump2, LVar0, LVar1, LVar2, 2)
    EVT_CALL(SetPlayerActionState, ACTION_STATE_LAND)
    EVT_SET(MV_PlayerOnBoard, TRUE)
    EVT_CALL(InterpPlayerYaw, 225, 0)
    EVT_CALL(DisablePlayerPhysics, TRUE)
    EVT_CALL(DisablePartnerAI, 0)
    EVT_ADD(LVar2, 10)
    EVT_WAIT(10)
    EVT_CALL(ShowMessageAtScreenPos, MSG_Menus_0188, 160, 40)
    EVT_CALL(ShowChoice, MSG_Choice_000C)
    EVT_CALL(CloseMessage)
    EVT_IF_EQ(LVar0, 1)
        EVT_CALL(DisablePlayerPhysics, FALSE)
        EVT_CALL(SetPlayerJumpscale, 2)
        EVT_SET(MV_PlayerOnBoard, FALSE)
        EVT_CALL(PlayerJump, -50, 0, 225, 13)
        EVT_CALL(HidePlayerShadow, FALSE)
        EVT_CALL(SetPlayerActionState, ACTION_STATE_LAND)
        EVT_IF_EQ(LVar0, TRUE)
            EVT_WAIT(10)
        EVT_END_IF
        EVT_CALL(DisablePlayerInput, FALSE)
        EVT_RETURN
    EVT_END_IF
    EVT_CALL(SetMusicTrack, 0, SONG_STARSHIP_THEME, 1, 8)
    EVT_THREAD
        EVT_CALL(PlaySoundAtPlayer, SOUND_17E, SOUND_SPACE_MODE_0)
        EVT_SET(LVar2, MV_Starship_PosY)
        EVT_CALL(MakeLerp, 0, -110, 60, EASING_QUADRATIC_OUT)
        EVT_LOOP(0)
            EVT_CALL(UpdateLerp)
            EVT_ADD(LVar0, LVar2)
            EVT_SET(MV_Starship_PosY, LVar0)
            EVT_WAIT(1)
            EVT_IF_EQ(LVar1, 0)
                EVT_BREAK_LOOP
            EVT_END_IF
        EVT_END_LOOP
    EVT_END_THREAD
    EVT_WAIT(50)
    EVT_CALL(GotoMap, EVT_PTR("kpa_60"), kpa_60_ENTRY_5)
    EVT_WAIT(100)
    EVT_RETURN
    EVT_END
};

EvtScript N(EVS_EndGame) = {
    EVT_CALL(SetNpcPos, 2, 9, 58, 225)
    EVT_CALL(SpeakToPlayer, 2, ANIM_WorldEldstar_Talk, ANIM_WorldEldstar_Idle, 0, MSG_Space_EldstarTalk1)
    EVT_CALL(SpeakToPlayer, 2, ANIM_WorldEldstar_Talk, ANIM_WorldEldstar_Idle, 0, MSG_Space_EldstarTalk2)
    EVT_RETURN
    EVT_END
};

EvtScript N(EVS_Starship_Arrive) = {
    EVT_SET(MV_Starship_PosY, -100)
    EVT_SET(MV_Starship_Yaw, 180)
    EVT_SET(MV_PlayerOnBoard, TRUE)
    EVT_CALL(InterpPlayerYaw, 90, 0)
    EVT_CALL(DisablePlayerInput, TRUE)
    EVT_CALL(DisablePlayerPhysics, TRUE)
    EVT_CALL(SetPlayerActionState, ACTION_STATE_LAND)
    EVT_CALL(UseSettingsFrom, CAM_DEFAULT, -120, 0, 230)
    EVT_CALL(SetPanTarget, CAM_DEFAULT, -120, 0, 230)
    EVT_CALL(SetCamSpeed, CAM_DEFAULT, EVT_FLOAT(90.0))
    EVT_CALL(PanToTarget, CAM_DEFAULT, 0, 1)
    EVT_CALL(HidePlayerShadow, FALSE)
    EVT_EXEC_GET_TID(N(EVS_UpdatePassengers), LVar9)
    EVT_CALL(PlaySoundAtPlayer, SOUND_17F, SOUND_SPACE_MODE_0)
    EVT_THREAD
        EVT_CALL(MakeLerp, -100, 0, 60, EASING_QUADRATIC_OUT)
        EVT_LOOP(0)
            EVT_CALL(UpdateLerp)
            EVT_SET(MV_Starship_PosY, LVar0)
            EVT_WAIT(1)
            EVT_IF_EQ(LVar1, 0)
                EVT_BREAK_LOOP
            EVT_END_IF
        EVT_END_LOOP
    EVT_END_THREAD
    EVT_WAIT(20)
    EVT_THREAD
        EVT_CALL(InterpPlayerYaw, 270, 40)
    EVT_END_THREAD
    EVT_CALL(MakeLerp, 180, 0, 60, EASING_QUADRATIC_OUT)
    EVT_LOOP(0)
        EVT_CALL(UpdateLerp)
        EVT_SET(MV_Starship_Yaw, LVar0)
        EVT_WAIT(1)
        EVT_IF_EQ(LVar1, 0)
            EVT_BREAK_LOOP
        EVT_END_IF
    EVT_END_LOOP
    EVT_KILL_THREAD(LVar9)
    EVT_CALL(SetPlayerJumpscale, 2)
    EVT_SET(MV_PlayerOnBoard, FALSE)
    EVT_CALL(PlayerJump, -50, 0, 225, 13)
    EVT_CALL(HidePlayerShadow, FALSE)
    EVT_CALL(DisablePlayerPhysics, FALSE)
    EVT_IF_EQ(LVar0, TRUE)
        EVT_WAIT(10)
    EVT_END_IF
    EVT_CALL(PanToTarget, CAM_DEFAULT, 0, 0)
    EVT_WAIT(10)
    EVT_IF_LT(GB_StoryProgress, STORY_CH8_REACHED_BOWSERS_CASTLE)
        EVT_SET(GB_StoryProgress, STORY_CH8_REACHED_BOWSERS_CASTLE)
    EVT_END_IF
    EVT_CALL(SetMusicTrack, 0, SONG_BOWSERS_CASTLE, 0, 8)
    EVT_THREAD
        EVT_WAIT(30)
        EVT_CALL(SetMusicTrackVolumes, TRACK_VOLS_KPA_OUTSIDE)
    EVT_END_THREAD
    EVT_EXEC_WAIT(N(EVS_EndGame))
    EVT_CALL(DisablePlayerInput, FALSE)
    EVT_RETURN
    EVT_END
};

EvtScript N(EVS_SetupStarship) = {
    EVT_EXEC(N(EVS_Starship_Update))
    EVT_CALL(ParentColliderToModel, COLLIDER_o400, MODEL_o308)
    EVT_LOOP(0)
        EVT_CALL(UpdateColliderTransform, COLLIDER_o400)
        EVT_WAIT(1)
    EVT_END_LOOP
    EVT_RETURN
    EVT_END
};
