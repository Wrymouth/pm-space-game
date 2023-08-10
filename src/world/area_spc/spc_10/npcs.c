#include "spc_10.h"
#include "world/common/npc/Bowser.h"
#include "world/action/enemy_bullet.h"
#include "world/area_spc/common/enemy_behaviour.inc.c"

API_CALLABLE(N(CheckPhase)) {
    Enemy* enemy = script->owner1.enemy;
    s32 newPhase;

    if (enemy->curHP <= 12) {
        newPhase = 4; 
    } else if (enemy->curHP <= 24) {
        newPhase = 3; 
    } else if (enemy->curHP <= 36) {
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

EvtScript N(NpcWin_Bowser) = {
    EVT_SET(LVar0, 3)
    EVT_SET(LVar1, ANIM_WorldBowser_Talk)
    EVT_SET(LVar2, ANIM_WorldBowser_ArmsCrossed)
    EVT_SET(LVar3, MSG_Space_Bowser_Defeat)
    EVT_SET(LVar4, 130)
    EVT_SETF(LVar5, EVT_FLOAT(0.5))
    EVT_EXEC_WAIT(N(ShowCharacterString))
    EVT_RETURN
    EVT_END
};

EvtScript N(NpcDefeat_Bowser) = {
    EVT_THREAD
        EVT_SET(LVar0, 3)
        EVT_SET(LVar1, ANIM_WorldBowser_Talk)
        EVT_SET(LVar2, ANIM_WorldBowser_ArmsCrossed)
        EVT_SET(LVar3, MSG_Space_Bowser_Win)
        EVT_SET(LVar4, 130)
        EVT_SETF(LVar5, EVT_FLOAT(0.5))
        EVT_EXEC_WAIT(N(ShowCharacterString))
    EVT_END_THREAD
    EVT_LOOP(0)
        EVT_SET(LVar0, MapXLeft)
        EVT_SET(LVar1, MapYBottom)
        EVT_CALL(RandInt, 600, LVar3)
        EVT_CALL(RandInt, 350, LVar4)
        EVT_ADD(LVar0, LVar3)
        EVT_ADD(LVar1, LVar4)
        EVT_CALL(PlaySound, 0x2076)
        EVT_CALL(PlayEffect, EFFECT_EXPLOSION, 1, LVar0, LVar1, 16)
        EVT_WAIT(6)
    EVT_END_LOOP
    EVT_RETURN
    EVT_END
};

EvtScript N(BowserFakeDefeat) = {
    EVT_SET(LVarA, 10)
    EVT_LOOP(0)
        EVT_CALL(GetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
        EVT_IF_LT(LVar1, BowserDefeatYBottom)
            EVT_CALL(SetNpcPos, NPC_SELF, LVar0, BowserDefeatYBottom, LVar2)  
            EVT_BREAK_LOOP
        EVT_END_IF
        EVT_SUB(LVar1, LVarA)
        EVT_CALL(SetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
        EVT_DEBUG_PRINT_VAR(LVar1)
        EVT_WAIT(1)
    EVT_END_LOOP
    EVT_RETURN
    EVT_END
};

EvtScript N(PhaseTransitions) = {
    EVT_ADD(MV_BattlePhase, 1)
    EVT_SWITCH(MV_BattlePhase)
        EVT_CASE_EQ(1)
            EVT_CALL(N(SetEnemyInvincible), TRUE)
            EVT_SET(LVarA, LVar3)
            EVT_SET(LVar0, 3)
            EVT_SET(LVar1, ANIM_WorldBowser_Talk)
            EVT_SET(LVar2, ANIM_WorldBowser_ArmsCrossed)
            EVT_SET(LVar3, MSG_Space_Bowser_Phase1)
            EVT_SET(LVar4, 300)
            EVT_SETF(LVar5, EVT_FLOAT(0.5))
            EVT_EXEC_WAIT(N(ShowCharacterString))
            EVT_SET(LVar3, LVarA) // directionX
            EVT_CALL(SetMusicTrack, 0, SONG_SMB1_BOWSER, 0, 8)
            EVT_CALL(N(SetEnemyInvincible), FALSE)
        EVT_CASE_EQ(2)
            EVT_SET(LVarA, LVar3)
            EVT_SET(LVar0, 3)
            EVT_SET(LVar1, ANIM_WorldBowser_Talk)
            EVT_SET(LVar2, ANIM_WorldBowser_ArmsCrossed)
            EVT_SET(LVar3, MSG_Space_Bowser_Phase2)
            EVT_SET(LVar4, 130)
            EVT_SETF(LVar5, EVT_FLOAT(0.5))
            EVT_EXEC_WAIT(N(ShowCharacterString))
            EVT_CALL(SetMusicTrack, 0, SONG_MONSTAR_THEME, 0, 8)
            EVT_SET(LVar3, LVarA)
            EVT_SET(LVar4, -13)
            EVT_SET(MV_SwitchTimer, SwitchDuration)
            EVT_ADD(MV_SwitchTimer, 1)
        EVT_CASE_EQ(3)
            EVT_SET(LVarA, LVar3)
            EVT_SET(LVar0, 3)
            EVT_SET(LVar1, ANIM_WorldBowser_Talk)
            EVT_SET(LVar2, ANIM_WorldBowser_ArmsCrossed)
            EVT_SET(LVar3, MSG_Space_Bowser_Phase3)
            EVT_SET(LVar4, 130)
            EVT_SETF(LVar5, EVT_FLOAT(0.5))
            EVT_EXEC_WAIT(N(ShowCharacterString))
            EVT_IF_GT(LVarA, 0)
                EVT_SET(LVar3, 80)
            EVT_ELSE
                EVT_SET(LVar3, -80)
            EVT_END_IF
        EVT_CASE_EQ(4)
            EVT_KILL_THREAD(LVar7)
            EVT_EXEC_WAIT(N(BowserFakeDefeat))
            EVT_CALL(SetNpcScale, NPC_SELF, EVT_FLOAT(3.7), EVT_FLOAT(3.7), EVT_FLOAT(3.7))
            EVT_CALL(SetNpcAnimation, NPC_SELF, ANIM_WorldBowser_Idle)
            EVT_CALL(SetNpcCollisionSize, NPC_SELF, 300, 300)
            EVT_CALL(SetNpcPos, NPC_SELF, 0, BowserDefeatYBottom, 16)

            EVT_SET(LVar0, 3)
            EVT_SET(LVar1, ANIM_WorldBowser_Talk)
            EVT_SET(LVar2, ANIM_WorldBowser_ArmsCrossed)
            EVT_SET(LVar3, MSG_Space_Bowser_Phase4)
            EVT_SET(LVar4, 200)
            EVT_SETF(LVar5, EVT_FLOAT(0.5))
            EVT_EXEC_WAIT(N(ShowCharacterString))

            EVT_CALL(SetMusicTrack, 0, SONG_SHROOB_FINAL2, 0, 8)
    EVT_END_SWITCH
    EVT_RETURN
    EVT_END
};

EvtScript N(JumpAround) = {
    EVT_LOOP(0)
        EVT_IF_EQ(MV_BattlePhase, 4)
            EVT_BREAK_LOOP
        EVT_END_IF
        EVT_CALL(GetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
        EVT_SET(LVar1, BowserYBottom)
        EVT_IF_LT(LVar0, BowserXLeft)
            EVT_SET(LVar3, 120)
        EVT_END_IF
        EVT_IF_GT(LVar0, BowserXRight)
            EVT_SET(LVar3, -120)
        EVT_END_IF
        EVT_ADD(LVar0, LVar3)
        EVT_CALL(SetNpcJumpscale, NPC_SELF, EVT_FLOAT(2.3))
        EVT_CALL(NpcJump0, NPC_SELF, LVar0, LVar1, LVar2, 30)
        EVT_CALL(PlaySound, SOUND_15C)
        EVT_WAIT(1)
    EVT_END_LOOP
    EVT_RETURN
    EVT_END
};

EvtScript N(DoGiantJump) = {
    EVT_SET(MF_IsGiantJumping, TRUE)
    EVT_CALL(RandInt, 500, LVarC)
    EVT_SUB(LVarC, 250)
    EVT_CALL(SetNpcPos, NPC_SELF, LVarC, BowserDefeatYBottom, LVar2)
    EVT_CALL(SetNpcJumpscale, NPC_SELF, EVT_FLOAT(0.3))
    EVT_CALL(NpcJump0, NPC_SELF, LVarC, BowserDefeatYBottom, LVar2, 40)
    EVT_CALL(SetNpcJumpscale, NPC_SELF, EVT_FLOAT(2.6))
    EVT_CALL(PlaySound, SOUND_3E7)
    EVT_CALL(NpcJump0, NPC_SELF, LVarC, BowserDefeatYBottom, LVar2, 40)
    EVT_SET(MF_IsGiantJumping, FALSE)
    EVT_RETURN
    EVT_END
};

EvtScript N(NpcIdle_Bowser) = {
    EVT_SET(LVar3, 10) // directionX
    EVT_SET(LVar4, 13) // directionY
    EVT_LOOP(0)
        EVT_CALL(N(Fast_NpcFacePlayer), NPC_SELF)
        // player defeat
        EVT_IF_TRUE(AF_PlayerDead)
            EVT_EXEC_WAIT(N(NpcWin_Bowser))
            EVT_BREAK_LOOP
        EVT_END_IF
        // phase check
        EVT_CALL(N(CheckPhase))
        EVT_IF_TRUE(MF_PhaseTransition)
            EVT_EXEC_WAIT(N(PhaseTransitions))
            EVT_DEBUG_PRINT_VAR(LVarE)
            EVT_GOTO(1)
        EVT_END_IF
        //movement
        EVT_CALL(GetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
        EVT_SWITCH(MV_BattlePhase)
            EVT_CASE_EQ(0) // before battle
            
            EVT_CASE_EQ(1) // move left to right at top of screen
                // movement
                EVT_SWITCH(LVar0)
                    EVT_CASE_OR_EQ(BowserXLeft)
                    EVT_CASE_OR_EQ(BowserXRight)
                        EVT_MUL(LVar3, -1)
                    EVT_END_CASE_GROUP
                EVT_END_SWITCH
                EVT_ADD(LVar0, LVar3)
                EVT_CALL(SetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
                // attacks
                EVT_IF_EQ(MV_HammersTimer, 12)
                    EVT_CALL(PlaySound, SOUND_2004)
                    EVT_CALL(N(DoAttack), ENEMY_ATTACK_TYPE_BOWSER_HAMMER)
                    EVT_SET(MV_HammersTimer, 0)
                EVT_END_IF
                EVT_IF_EQ(MV_BallTimer, 50)
                    EVT_CALL(PlaySound, SOUND_20DE)
                    EVT_CALL(N(DoAttack), ENEMY_ATTACK_TYPE_LARGE_BALL)
                    EVT_SET(MV_BallTimer, 0)
                EVT_END_IF
                EVT_ADD(MV_BallTimer, 1)
                EVT_ADD(MV_HammersTimer, 1)
            EVT_CASE_EQ(2) // move up and down, switching sides at intervals
                // movement
                EVT_IF_LT(LVar1, BowserYBottom)
                    EVT_MUL(LVar4, -1)
                EVT_END_IF
                EVT_IF_GT(LVar1, BowserYTop)
                    EVT_MUL(LVar4, -1)
                EVT_END_IF
                EVT_IF_GT(MV_SwitchTimer, SwitchDuration)
                    EVT_ADD(LVar0, LVar3)
                    EVT_IF_LT(LVar0, BowserXLeft)
                        EVT_SET(LVar3, 10)
                        EVT_SET(MV_SwitchTimer, 0)
                    EVT_END_IF
                    EVT_IF_GT(LVar0, BowserXRight)
                        EVT_SET(LVar3, -10)
                        EVT_SET(MV_SwitchTimer, 0)
                    EVT_END_IF
                EVT_ELSE
                    EVT_ADD(MV_SwitchTimer, 1)
                    // attacks
                    EVT_IF_EQ(MV_FlamesTimer, 25)
                        EVT_CALL(PlaySound, SOUND_3C4)
                        EVT_CALL(N(DoAttack), ENEMY_ATTACK_TYPE_SPREAD_FIRE)
                        EVT_SET(MV_FlamesTimer, 0)
                    EVT_END_IF
                    EVT_ADD(MV_FlamesTimer, 1)
                    EVT_IF_EQ(MV_ChaseTimer, 40)
                        EVT_CALL(PlaySound, SOUND_20D2)
                        EVT_CALL(N(DoAttack), ENEMY_ATTACK_TYPE_CHASE_FIREBALL)
                        EVT_SET(MV_ChaseTimer, 0)
                    EVT_END_IF
                    EVT_ADD(MV_ChaseTimer, 1)
                EVT_END_IF
 
                EVT_ADD(LVar1, LVar4)
                EVT_CALL(SetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)

            EVT_CASE_EQ(3) // bounce around screen
                // movement
                EVT_IF_EQ(MF_StartedJumping, FALSE)
                    EVT_EXEC_GET_TID(N(JumpAround), LVar7)
                    EVT_SET(MF_StartedJumping, TRUE)
                EVT_END_IF
                // attacks
                EVT_IF_EQ(MV_FlamesTimer, 30)
                    EVT_CALL(PlaySound, SOUND_3C4)
                    EVT_CALL(N(DoAttack), ENEMY_ATTACK_TYPE_SPREAD_FIRE_WIDE)
                    EVT_SET(MV_FlamesTimer, 0)
                EVT_END_IF
                EVT_ADD(MV_FlamesTimer, 1)

            EVT_CASE_EQ(4) // giant
                // movement
                EVT_IF_EQ(MF_IsGiantJumping, FALSE)
                    EVT_IF_EQ(MV_JumpTimer, 70)
                        EVT_EXEC_GET_TID(N(DoGiantJump), LVarB)
                        EVT_SET(MV_JumpTimer, 0)
                    EVT_END_IF
                    EVT_ADD(MV_JumpTimer, 1)
                EVT_END_IF
                // attack
                EVT_IF_EQ(MV_RainTimer, 17)
                    EVT_CALL(N(DoAttack), ENEMY_ATTACK_TYPE_FIRE_RAIN)
                    EVT_SET(MV_RainTimer, 0)
                EVT_END_IF
                EVT_ADD(MV_RainTimer, 1)
        EVT_END_SWITCH
        // damage
        EVT_IF_GT(MV_BattlePhase, 3)
            EVT_SET(LVar0, ANIM_WorldBowser_Idle)
            EVT_SET(LVar1, ANIM_WorldBowser_Shock)
        EVT_ELSE
            EVT_SET(LVar0, ANIM_WorldBowser_ClownCarIdle)
            EVT_SET(LVar1, ANIM_WorldBowser_ClownCarLaugh)
        EVT_END_IF
        EVT_CALL(N(SetDamageAnimation), LVar0, LVar1, LVar2)
        EVT_CALL(SetNpcAnimation, NPC_SELF, LVar2)
        // defeat
        EVT_IF_TRUE(GF_BowserDefeated)
            EVT_KILL_THREAD(LVarB)
            EVT_SET(MF_EnemyDefeated, TRUE)
            EVT_EXEC_WAIT(N(NpcDefeat_Bowser))
            EVT_BREAK_LOOP
        EVT_END_IF
        EVT_LABEL(1)
        EVT_WAIT(1)
    EVT_END_LOOP
    EVT_RETURN
    EVT_END
};

EvtScript N(NpcInit_Bowser) = {
    EVT_CALL(EnableNpcShadow, NPC_SELF, FALSE)
    EVT_CALL(SetNpcScale, NPC_SELF, EVT_FLOAT(1.5), EVT_FLOAT(1.5), EVT_FLOAT(1.5))
    EVT_CALL(BindNpcIdle, NPC_SELF, EVT_PTR(N(NpcIdle_Bowser)))
    EVT_RETURN
    EVT_END
};

NpcSettings N(NpcSettings_Bowser) = {
    .height = 128,
    .radius = 48,
    .level = 99,
};

NpcData N(NpcData_Bowser) = {
    .id = 2,
    .pos = { 0.0f, 20.0f, 16.0f },
    .init = &N(NpcInit_Bowser),
    .yaw = 270,
    .settings = &N(NpcSettings_Bowser),
    .flags = ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_ENABLE_HIT_SCRIPT | ENEMY_FLAG_NO_SHADOW_RAYCAST,
    .drops = NO_DROPS,
    .animations = BOWSER_ANIMS,
    .aiDetectFlags = AI_DETECT_SIGHT,
    .maxHP = 48,
    .invFrames = 30,
    .defeatFlag = GF_BowserDefeated,
};

EvtScript N(NpcInit_BowserTalk) = {
    EVT_CALL(EnableNpcShadow, NPC_SELF, FALSE)
    EVT_RETURN
    EVT_END
};

NpcSettings N(NpcSettings_BowserTalk) = {
    .height = 64,
    .radius = 28,
    .level = 99,
};

NpcData N(NpcData_BowserTalk) = {
    .id = 3,
    .pos = { 0.0f, -1000.0f, 20.0f },
    .init = &N(NpcInit_BowserTalk),
    .yaw = 270,
    .settings = &N(NpcSettings_BowserTalk),
    .flags = ENEMY_FLAG_PASSIVE | ENEMY_FLAG_IGNORE_PLAYER_COLLISION | ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_ENABLE_HIT_SCRIPT | ENEMY_FLAG_NO_SHADOW_RAYCAST,
    .drops = NO_DROPS,
    .animations = { 
        .idle   = ANIM_WorldBowser_ArmsCrossed, 
        .walk   = ANIM_WorldBowser_ArmsCrossed, 
        .run    = ANIM_WorldBowser_ArmsCrossed, 
        .chase  = ANIM_WorldBowser_ArmsCrossed, 
        .anim_4 = ANIM_WorldBowser_ArmsCrossed, 
        .anim_5 = ANIM_WorldBowser_ArmsCrossed, 
        .death  = ANIM_WorldBowser_ArmsCrossed, 
        .hit    = ANIM_WorldBowser_ArmsCrossed, 
        .anim_8 = ANIM_WorldBowser_ArmsCrossed, 
        .anim_9 = ANIM_WorldBowser_ArmsCrossed, 
        .anim_A = ANIM_WorldBowser_ArmsCrossed, 
        .anim_B = ANIM_WorldBowser_ArmsCrossed, 
        .anim_C = ANIM_WorldBowser_ArmsCrossed, 
        .anim_D = ANIM_WorldBowser_ArmsCrossed, 
        .anim_E = ANIM_WorldBowser_ArmsCrossed, 
        .anim_F = ANIM_WorldBowser_ArmsCrossed, 
    },
    .aiDetectFlags = AI_DETECT_SIGHT,
};

NpcGroupList N(DefaultNpcs) = {
    NPC_GROUP(N(NpcData_Bowser)),
    NPC_GROUP(N(NpcData_BowserTalk)),
    {}
};

