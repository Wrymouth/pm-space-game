#include "spc_09.h"
#include "sprite/npc/ParadeKolorado.h"
#include "world/common/npc/KoopaBros.h"
#include "world/action/enemy_bullet.h"
#include "world/area_spc/common/enemy_behaviour.inc.c"

API_CALLABLE(N(CheckPhase)) {
    Enemy* enemy = script->owner1.enemy;
    s32 newPhase;

    if (enemy->curHP <= 16) {
        newPhase = 3; 
    } else if (enemy->curHP <= 24) {
        newPhase = 2; 
    } else {
        newPhase = 1;
    }

    if (newPhase != evt_get_variable(script,MV_BattlePhase)) {
        evt_set_variable(script, MF_PhaseTransition, TRUE);
    } else {
        evt_set_variable(script, MF_PhaseTransition, FALSE);
    }
    return ApiStatus_DONE2;
}

Vec3f N(KoopaBrosShipPath)[] = {
    {  83.0,  -65.0, 16.0 },
    {  68.0, -100.0, 16.0 },
    {  33.0, -115.0, 16.0 },
    {  -2.0, -100.0, 16.0 },
    { -17.0,  -65.0, 16.0 },
    {  -2.0,  -30.0, 16.0 },
    {  33.0,  -15.0, 16.0 },
    {  68.0,  -30.0, 16.0 },
    {  83.0,  -65.0, 16.0 },
};

EvtScript N(NpcWin_KoopaBrosShip) = {
    EVT_SET(LVar0, 3)
    EVT_SET(LVar1, ANIM_KoopaBros_Black_Talk)
    EVT_SET(LVar2, ANIM_KoopaBros_Black_Idle)
    EVT_SET(LVar3, MSG_Space_Koopa_Defeat)
    EVT_SET(LVar4, 130)
    EVT_SETF(LVar5, EVT_FLOAT(0.4))
    EVT_EXEC_WAIT(N(ShowCharacterString))
    EVT_RETURN
    EVT_END
};

EvtScript N(NpcDefeat_KoopaBrosShip) = {
    EVT_THREAD
        EVT_SET(LVar0, 3)
        EVT_SET(LVar1, ANIM_KoopaBros_Black_Talk)
        EVT_SET(LVar2, ANIM_KoopaBros_Black_Idle)
        EVT_SET(LVar3, MSG_Space_Koopa_Win)
        EVT_SET(LVar4, 130)
        EVT_SETF(LVar5, EVT_FLOAT(0.4))
        EVT_EXEC_WAIT(N(ShowCharacterString))
    EVT_END_THREAD
    // bounce around the screen like the tinier shells
    EVT_LOOP(6)
        EVT_CALL(GetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
        EVT_SUB(LVar0, 20)
        EVT_SUB(LVar1, 5)
        EVT_CALL(RandInt, 50, LVar3)
        EVT_CALL(RandInt, 50, LVar4)
        EVT_ADD(LVar0, LVar3)
        EVT_ADD(LVar1, LVar4)
        EVT_CALL(PlaySound, 0x2076)
        EVT_CALL(PlayEffect, EFFECT_EXPLOSION, 1, LVar0, LVar1, LVar2)
        EVT_WAIT(7)
    EVT_END_LOOP
    EVT_SET(LVar3, 20)
    EVT_SET(LVar4, 20)
    EVT_LOOP(100)
        EVT_CALL(GetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
        EVT_ADD(LVar0, LVar3)
        EVT_ADD(LVar1, LVar4)
        EVT_IF_GT(LVar0, MapXRight)
            EVT_MUL(LVar3, -1)
        EVT_END_IF
        EVT_IF_LT(LVar0, MapXLeft)
            EVT_MUL(LVar3, -1)
        EVT_END_IF
        EVT_IF_GT(LVar1, MapYTop)
            EVT_MUL(LVar4, -1)
        EVT_END_IF
        EVT_IF_LT(LVar1, MapYBottom)
            EVT_MUL(LVar4, -1)
        EVT_END_IF
        EVT_CALL(SetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
        EVT_WAIT(1)
    EVT_END_LOOP
    EVT_RETURN
    EVT_END
};

EvtScript N(PhaseTransitions) = {
    EVT_IF_TRUE(MV_BattlePhase)
        EVT_CALL(SetNpcAnimation, NPC_SELF, ANIM_ParadeKolorado_WifeIdle)
    EVT_END_IF
    EVT_ADD(MV_BattlePhase, 1)
    EVT_SWITCH(MV_BattlePhase)
        EVT_CASE_EQ(1)
            EVT_CALL(N(SetEnemyInvincible), TRUE)
            EVT_SET(LVar0, 3)
            EVT_SET(LVar1, ANIM_KoopaBros_Black_Talk)
            EVT_SET(LVar2, ANIM_KoopaBros_Black_Idle)
            EVT_SET(LVar3, MSG_Space_Koopa_Phase1)
            EVT_SET(LVar4, 220)
            EVT_SETF(LVar5, EVT_FLOAT(0.4))
            EVT_EXEC_WAIT(N(ShowCharacterString))
            EVT_SET(LVar3, 20) // direction
            EVT_CALL(N(SetEnemyInvincible), FALSE)
        EVT_CASE_EQ(2)
            EVT_SET(LVar0, 3)
            EVT_SET(LVar1, ANIM_KoopaBros_Black_Talk)
            EVT_SET(LVar2, ANIM_KoopaBros_Black_Idle)
            EVT_SET(LVar3, MSG_Space_Koopa_Phase2)
            EVT_SET(LVar4, 130)
            EVT_SETF(LVar5, EVT_FLOAT(0.4))
            EVT_EXEC_WAIT(N(ShowCharacterString))
            EVT_SET(LVar3, 20) // direction
        EVT_CASE_EQ(3)
            EVT_SET(LVar0, 3)
            EVT_SET(LVar1, ANIM_KoopaBros_Black_Talk)
            EVT_SET(LVar2, ANIM_KoopaBros_Black_Idle)
            EVT_SET(LVar3, MSG_Space_Koopa_Phase3)
            EVT_SET(LVar4, 130)
            EVT_SETF(LVar5, EVT_FLOAT(0.4))
            EVT_EXEC_WAIT(N(ShowCharacterString))
            EVT_SET(LVar3, 20) // direction
    EVT_END_SWITCH
    EVT_RETURN
    EVT_END
};

EvtScript N(MoveAlongPath) = {
    EVT_LOOP(0)
        EVT_CALL(LoadPath, 90, N(KoopaBrosShipPath), 9, EASING_LINEAR)
        EVT_LOOP(0)
            EVT_CALL(GetNextPathPos)
            EVT_CALL(SetNpcPos, NPC_SELF, LVar1, LVar2, LVar3)
            EVT_IF_EQ(LVar0, 0)
                EVT_BREAK_LOOP
            EVT_END_IF
            EVT_WAIT(1)
        EVT_END_LOOP
    EVT_END_LOOP
    EVT_RETURN
    EVT_END
};

EvtScript N(NpcIdle_KoopaBrosShip) = {
    EVT_SET(LVar3, 20) // direction
    EVT_EXEC_GET_TID(N(MoveAlongPath), LVarA)
    EVT_LOOP(0)
        // player defeat
        EVT_IF_TRUE(AF_PlayerDead)
            EVT_EXEC_WAIT(N(NpcWin_KoopaBrosShip))
            EVT_BREAK_LOOP
        EVT_END_IF
        // phase check
        EVT_CALL(N(CheckPhase))
        EVT_IF_TRUE(MF_PhaseTransition)
            EVT_EXEC_WAIT(N(PhaseTransitions))
            EVT_GOTO(1)
        EVT_END_IF
        //movement
        EVT_IF_GT(MV_BattlePhase, 1)
            EVT_IF_EQ(MV_SplitTimer, 50)
                EVT_CALL(PlaySound, SOUND_20EB)
                EVT_CALL(N(DoAttack), ENEMY_ATTACK_TYPE_SPLIT)
                EVT_SET(MV_SplitTimer, 0)
            EVT_END_IF
            EVT_ADD(MV_SplitTimer, 1)
        EVT_END_IF
        EVT_IF_EQ(MV_ShellTimerA, 15)
            EVT_CALL(PlaySound, SOUND_2023)
            EVT_CALL(N(DoAttack), ENEMY_ATTACK_TYPE_KOOPA_SHELLS_A)
        EVT_END_IF
        EVT_IF_EQ(MV_ShellTimerB, 15)
            EVT_CALL(PlaySound, SOUND_2023)
            EVT_CALL(N(DoAttack), ENEMY_ATTACK_TYPE_KOOPA_SHELLS_B)
        EVT_END_IF
        EVT_ADD(MV_ShellTimerA, 1)
        EVT_IF_EQ(MV_BattlePhase, 3)
            EVT_ADD(MV_ShellTimerB, 1)
        EVT_END_IF
        // damage
        EVT_SET(LVar0, ANIM_ParadeKolorado_Idle)
        EVT_SET(LVar1, ANIM_ParadeKolorado_WifeIdle)
        EVT_CALL(N(SetDamageAnimation), LVar0, LVar1, LVar2)
        EVT_CALL(SetNpcAnimation, NPC_SELF, LVar2)
        // defeat
        EVT_IF_TRUE(GF_KoopaBrosDefeated)
            EVT_KILL_THREAD(LVarA)
            EVT_SET(MF_EnemyDefeated, TRUE)
            EVT_EXEC_WAIT(N(NpcDefeat_KoopaBrosShip))
            EVT_CALL(DoNpcDefeat)
            EVT_BREAK_LOOP
        EVT_END_IF
        EVT_LABEL(1)
        EVT_WAIT(1)
    EVT_END_LOOP
    EVT_RETURN
    EVT_END
};

EvtScript N(NpcInit_KoopaBrosShip) = {
    EVT_CALL(EnableNpcShadow, NPC_SELF, FALSE)
    EVT_CALL(SetNpcScale, NPC_SELF, EVT_FLOAT(2.0), EVT_FLOAT(2.0), EVT_FLOAT(2.0))
    EVT_CALL(BindNpcIdle, NPC_SELF, EVT_PTR(N(NpcIdle_KoopaBrosShip)))
    EVT_RETURN
    EVT_END
};

NpcSettings N(NpcSettings_KoopaBrosShip) = {
    .height = 80,
    .radius = 64,
    .level = 99,
};

NpcData N(NpcData_KoopaBrosShip) = {
    .id = 2,
    .pos = { 90.0f, 0.0f, 16.0f },
    .init = &N(NpcInit_KoopaBrosShip),
    .yaw = 270,
    .settings = &N(NpcSettings_KoopaBrosShip),
    .flags = ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_ENABLE_HIT_SCRIPT | ENEMY_FLAG_NO_SHADOW_RAYCAST,
    .drops = NO_DROPS,
    .animations = {
            .idle   = ANIM_ParadeKolorado_Idle,
            .walk   = ANIM_ParadeKolorado_Idle,
            .run    = ANIM_ParadeKolorado_Idle,
            .chase  = ANIM_ParadeKolorado_Idle,
            .anim_4 = ANIM_ParadeKolorado_Idle,
            .anim_5 = ANIM_ParadeKolorado_Idle,
            .death  = ANIM_ParadeKolorado_Idle,
            .hit    = ANIM_ParadeKolorado_Idle,
            .anim_8 = ANIM_ParadeKolorado_Idle,
            .anim_9 = ANIM_ParadeKolorado_Idle,
            .anim_A = ANIM_ParadeKolorado_Idle,
            .anim_B = ANIM_ParadeKolorado_Idle,
            .anim_C = ANIM_ParadeKolorado_Idle,
            .anim_D = ANIM_ParadeKolorado_Idle,
            .anim_E = ANIM_ParadeKolorado_Idle,
            .anim_F = ANIM_ParadeKolorado_Idle,
        },
    .aiDetectFlags = AI_DETECT_SIGHT,
    .maxHP = 32,
    .invFrames = 30,
    .defeatFlag = GF_KoopaBrosDefeated,
};

EvtScript N(NpcInit_KoopaBros) = {
    EVT_CALL(EnableNpcShadow, NPC_SELF, FALSE)
    EVT_RETURN
    EVT_END
};

NpcSettings N(NpcSettings_KoopaBros) = {
    .height = 64,
    .radius = 28,
    .level = 99,
};

NpcData N(NpcData_KoopaBros) = {
    .id = 3,
    .pos = { 0.0f, -1000.0f, 20.0f },
    .init = &N(NpcInit_KoopaBros),
    .yaw = 270,
    .settings = &N(NpcSettings_KoopaBros),
    .flags = ENEMY_FLAG_PASSIVE | ENEMY_FLAG_IGNORE_PLAYER_COLLISION | ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_ENABLE_HIT_SCRIPT | ENEMY_FLAG_NO_SHADOW_RAYCAST,
    .drops = NO_DROPS,
    .animations = BLACK_KOOPA_BROS_ANIMS,
    .aiDetectFlags = AI_DETECT_SIGHT,
};


NpcGroupList N(DefaultNpcs) = {
    NPC_GROUP(N(NpcData_KoopaBrosShip)),
    NPC_GROUP(N(NpcData_KoopaBros)),
    {}
};
