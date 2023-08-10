#include "spc_06.h"
#include "sprite/npc/ParadeYoshi.h"
#include "world/action/enemy_bullet.h"
#include "world/common/enemy/HammerBros.h"
#include "world/area_spc/common/enemy_behaviour.inc.c"

#define HAMMER_BRO_SHIP_ANIMS \
{ \
    .idle   = ANIM_ParadeYoshi_StillGreen, \
    .walk   = ANIM_ParadeYoshi_StillGreen, \
    .run    = ANIM_ParadeYoshi_StillGreen, \
    .chase  = ANIM_ParadeYoshi_StillGreen, \
    .anim_4 = ANIM_ParadeYoshi_StillGreen, \
    .anim_5 = ANIM_ParadeYoshi_StillGreen, \
    .death  = ANIM_ParadeYoshi_StillGreen, \
    .hit    = ANIM_ParadeYoshi_StillGreen, \
    .anim_8 = ANIM_ParadeYoshi_StillGreen, \
    .anim_9 = ANIM_ParadeYoshi_StillGreen, \
    .anim_A = ANIM_ParadeYoshi_StillGreen, \
    .anim_B = ANIM_ParadeYoshi_StillGreen, \
    .anim_C = ANIM_ParadeYoshi_StillGreen, \
    .anim_D = ANIM_ParadeYoshi_StillGreen, \
    .anim_E = ANIM_ParadeYoshi_StillGreen, \
    .anim_F = ANIM_ParadeYoshi_StillGreen, \
}

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

EvtScript N(NpcWin_HammerBroShip) = {
    EVT_SET(LVar0, 3)
    EVT_SET(LVar1, ANIM_HammerBros_Anim0B)
    EVT_SET(LVar2, ANIM_HammerBros_Anim02)
    EVT_SET(LVar3, MSG_Space_Hammer_Defeat)
    EVT_SET(LVar4, 130)
    EVT_SETF(LVar5, EVT_FLOAT(0.4))
    EVT_EXEC_WAIT(N(ShowCharacterString))
    EVT_RETURN
    EVT_END
};

EvtScript N(NpcDefeat_HammerBroShip) = {
    EVT_THREAD
        EVT_SET(LVar0, 3)
        EVT_SET(LVar1, ANIM_HammerBros_Anim0B)
        EVT_SET(LVar2, ANIM_HammerBros_Anim02)
        EVT_SET(LVar3, MSG_Space_Hammer_Win)
        EVT_SET(LVar4, 130)
        EVT_SETF(LVar5, EVT_FLOAT(0.4))
        EVT_EXEC_WAIT(N(ShowCharacterString))
    EVT_END_THREAD
    EVT_CALL(GetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
    EVT_THREAD
        EVT_WAIT(20)
        EVT_CALL(SetNpcJumpscale, NPC_SELF, EVT_FLOAT(0.2))
        EVT_CALL(NpcJump0, NPC_SELF, LVar0, -500, LVar2, 30)
    EVT_END_THREAD
    EVT_LOOP(6)
        EVT_CALL(GetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
        EVT_SUB(LVar0, 20)
        EVT_SUB(LVar1, 20)
        EVT_CALL(RandInt, 50, LVar3)
        EVT_CALL(RandInt, 50, LVar4)
        EVT_ADD(LVar0, LVar3)
        EVT_ADD(LVar1, LVar4)
        EVT_CALL(PlaySound, 0x2076)
        EVT_CALL(PlayEffect, EFFECT_EXPLOSION, 1, LVar0, LVar1, LVar2)
        EVT_WAIT(7)
    EVT_END_LOOP
    EVT_RETURN
    EVT_END
};

EvtScript N(PhaseTransitions) = {
    EVT_IF_TRUE(MV_BattlePhase)
        EVT_CALL(SetNpcAnimation, NPC_SELF, ANIM_ParadeYoshi_StillBlue)
    EVT_END_IF
    EVT_ADD(MV_BattlePhase, 1)
    EVT_SWITCH(MV_BattlePhase)
        EVT_CASE_EQ(1)
            EVT_CALL(N(SetEnemyInvincible), TRUE)
            EVT_SET(LVar0, 3)
            EVT_SET(LVar1, ANIM_HammerBros_Anim0B)
            EVT_SET(LVar2, ANIM_HammerBros_Anim02)
            EVT_SET(LVar3, MSG_Space_Hammer_Phase1)
            EVT_SET(LVar4, 140)
            EVT_SETF(LVar5, EVT_FLOAT(0.4))
            EVT_EXEC_WAIT(N(ShowCharacterString))
            EVT_SET(MV_HammerInterval, 40)
            EVT_SET(LVar3, 20) // direction
            EVT_CALL(N(SetEnemyInvincible), FALSE)
        EVT_CASE_EQ(2)
            EVT_SET(LVar0, 3)
            EVT_SET(LVar1, ANIM_HammerBros_Anim0B)
            EVT_SET(LVar2, ANIM_HammerBros_Anim02)
            EVT_SET(LVar3, MSG_Space_Hammer_Phase2)
            EVT_SET(LVar4, 130)
            EVT_SETF(LVar5, EVT_FLOAT(0.4))
            EVT_EXEC_WAIT(N(ShowCharacterString))
            EVT_SET(LVar3, 20) // direction
        EVT_CASE_EQ(3)
            EVT_SET(LVar0, 3)
            EVT_SET(LVar1, ANIM_HammerBros_Anim0B)
            EVT_SET(LVar2, ANIM_HammerBros_Anim02)
            EVT_SET(LVar3, MSG_Space_Hammer_Phase3)
            EVT_SET(LVar4, 130)
            EVT_SETF(LVar5, EVT_FLOAT(0.4))
            EVT_EXEC_WAIT(N(ShowCharacterString))
            EVT_SET(MV_HammerInterval, 25)
            EVT_SET(MV_HammerTimer, 0)
            EVT_SET(LVar3, 20) // direction
    EVT_END_SWITCH
    EVT_RETURN
    EVT_END
};

EvtScript N(NpcIdle_HammerBroShip) = {
    EVT_SET(LVar3, 20) // direction
    EVT_LOOP(0)
        // player defeat
        EVT_IF_TRUE(AF_PlayerDead)
            EVT_EXEC_WAIT(N(NpcWin_HammerBroShip))
            EVT_BREAK_LOOP
        EVT_END_IF
        // phase check
        EVT_CALL(N(CheckPhase))
        EVT_IF_TRUE(MF_PhaseTransition)
            EVT_EXEC_WAIT(N(PhaseTransitions))
            EVT_GOTO(1)
        EVT_END_IF
        //movement
        EVT_CALL(GetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
        EVT_IF_LT(LVar0, MapXLeft)
            EVT_MUL(LVar3, -1)
        EVT_END_IF
        EVT_IF_GT(LVar0, MapXRight)
            EVT_MUL(LVar3, -1)
        EVT_END_IF
        EVT_ADD(LVar0, LVar3)
        EVT_CALL(SetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
        EVT_IF_EQ(MV_HammerTimer, MV_HammerInterval)
            EVT_CALL(PlaySound, SOUND_2004)
            EVT_CALL(N(DoAttack), ENEMY_ATTACK_TYPE_HAMMER)
            EVT_SET(MV_HammerTimer, 0)
        EVT_END_IF
        EVT_ADD(MV_HammerTimer, 1)
        EVT_IF_GT(MV_BattlePhase, 1)
            EVT_IF_EQ(MV_ShotTimer, 50)
                EVT_CALL(PlaySound, SOUND_20EB)
                EVT_CALL(N(DoAttack), ENEMY_ATTACK_TYPE_LEFT)
                EVT_SET(MV_ShotTimer, 0)
            EVT_END_IF
            EVT_ADD(MV_ShotTimer, 1)
        EVT_END_IF
        // damage
        EVT_SET(LVar0, ANIM_ParadeYoshi_StillGreen)
        EVT_SET(LVar1, ANIM_ParadeYoshi_StillBlue)
        EVT_CALL(N(SetDamageAnimation), LVar0, LVar1, LVar2)
        EVT_CALL(SetNpcAnimation, NPC_SELF, LVar2)
        EVT_LABEL(1)
        EVT_WAIT(1)
        // defeat
        EVT_IF_TRUE(GF_HammerBrosDefeated)
            EVT_SET(MF_EnemyDefeated, TRUE)
            EVT_EXEC_WAIT(N(NpcDefeat_HammerBroShip))
            EVT_CALL(DoNpcDefeat)
            EVT_CALL(PlaySound, SOUND_PERIL)
            EVT_BREAK_LOOP
        EVT_END_IF
    EVT_END_LOOP
    EVT_RETURN
    EVT_END
};

EvtScript N(NpcInit_HammerBroShip) = {
    EVT_CALL(EnableNpcShadow, NPC_SELF, FALSE)
    EVT_CALL(SetNpcScale, NPC_SELF, 2.0f, 2.0f, 1.0f)
    EVT_CALL(BindNpcIdle, NPC_SELF, EVT_PTR(N(NpcIdle_HammerBroShip)))
    EVT_CALL(BindNpcDefeat, NPC_SELF, EVT_PTR(N(NpcDefeat_HammerBroShip)))
    EVT_RETURN
    EVT_END
};

NpcSettings N(NpcSettings_HammerBroShip) = {
    .height = 64,
    .radius = 28,
    .level = 99,
};

NpcData N(NpcData_HammerBroShip) = {
    .id = 2,
    .pos = { 0.0f, -213.0f, 16.0f },
    .init = &N(NpcInit_HammerBroShip),
    .yaw = 270,
    .settings = &N(NpcSettings_HammerBroShip),
    .flags = ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_ENABLE_HIT_SCRIPT | ENEMY_FLAG_NO_SHADOW_RAYCAST,
    .drops = NO_DROPS,
    .animations = HAMMER_BRO_SHIP_ANIMS,
    .aiDetectFlags = AI_DETECT_SIGHT,
    .maxHP = 32,
    .invFrames = 30,
    .defeatFlag = GF_HammerBrosDefeated,
};

EvtScript N(NpcInit_Hammer) = {
    EVT_CALL(EnableNpcShadow, NPC_SELF, FALSE)
    EVT_RETURN
    EVT_END
};

NpcSettings N(NpcSettings_Hammer) = {
    .height = 64,
    .radius = 28,
    .level = 99,
};

NpcData N(NpcData_Hammer) = {
    .id = 3,
    .pos = { 0.0f, -1000.0f, 20.0f },
    .init = &N(NpcInit_Hammer),
    .yaw = 270,
    .settings = &N(NpcSettings_Hammer),
    .flags = ENEMY_FLAG_PASSIVE | ENEMY_FLAG_IGNORE_PLAYER_COLLISION | ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_ENABLE_HIT_SCRIPT | ENEMY_FLAG_NO_SHADOW_RAYCAST,
    .drops = NO_DROPS,
    .animations = HAMMER_BROS_ANIMS,
    .aiDetectFlags = AI_DETECT_SIGHT,
};

NpcGroupList N(DefaultNpcs) = {
    NPC_GROUP(N(NpcData_HammerBroShip)),
    NPC_GROUP(N(NpcData_Hammer)),
    {}
};

