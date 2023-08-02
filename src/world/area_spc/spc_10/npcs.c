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
        EVT_CASE_EQ(2)
            EVT_SET(LVar4, -13)
            EVT_SET(MV_SwitchTimer, SwitchDuration)
            EVT_ADD(MV_SwitchTimer, 1)
        EVT_CASE_EQ(3)
            EVT_IF_GT(LVar3, 0)
                EVT_SET(LVar3, 80)
            EVT_ELSE
                EVT_SET(LVar3, -80)
            EVT_END_IF
        EVT_CASE_EQ(4)
            EVT_EXEC_WAIT(N(BowserFakeDefeat))
            EVT_CALL(SetNpcScale, NPC_SELF, EVT_FLOAT(3.7), EVT_FLOAT(3.7), EVT_FLOAT(3.7))
            EVT_CALL(SetNpcAnimation, NPC_SELF, ANIM_WorldBowser_Idle)
            EVT_CALL(SetNpcCollisionSize, NPC_SELF, 280, 300)
            EVT_CALL(SetNpcPos, NPC_SELF, 0, BowserDefeatYBottom, 16)
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
        EVT_IF_TRUE(MF_PhaseTransition)
            EVT_EXEC_WAIT(N(PhaseTransitions))
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
                    EVT_CALL(N(DoAttack), ENEMY_ATTACK_TYPE_BOWSER_HAMMER)
                    EVT_SET(MV_HammersTimer, 0)
                EVT_END_IF
                EVT_IF_EQ(MV_BallTimer, 50)
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
                        EVT_CALL(N(DoAttack), ENEMY_ATTACK_TYPE_SPREAD_FIRE)
                        EVT_SET(MV_FlamesTimer, 0)
                    EVT_END_IF
                    EVT_ADD(MV_FlamesTimer, 1)
                    EVT_IF_EQ(MV_ChaseTimer, 40)
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
                    EVT_EXEC(N(JumpAround))
                    EVT_SET(MF_StartedJumping, TRUE)
                EVT_END_IF
                // attacks
                EVT_IF_EQ(MV_FlamesTimer, 30)
                    EVT_CALL(N(DoAttack), ENEMY_ATTACK_TYPE_SPREAD_FIRE_WIDE)
                    EVT_SET(MV_FlamesTimer, 0)
                EVT_END_IF
                EVT_ADD(MV_FlamesTimer, 1)

            EVT_CASE_EQ(4) // giant
                // movement
                EVT_IF_EQ(MF_IsGiantJumping, FALSE)
                    EVT_IF_EQ(MV_JumpTimer, 70)
                        EVT_EXEC(N(DoGiantJump))
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
        EVT_IF_EQ(MV_BattlePhase, 4)
            EVT_SET(LVar0, ANIM_WorldBowser_Idle)
            EVT_SET(LVar1, ANIM_WorldBowser_RearUpLaugh)
        EVT_ELSE
            EVT_SET(LVar0, ANIM_WorldBowser_ClownCarIdle)
            EVT_SET(LVar1, ANIM_WorldBowser_ClownCarLaugh)
        EVT_END_IF
        EVT_CALL(N(SetDamageAnimation), LVar0, LVar1, LVar2)
        EVT_CALL(SetNpcAnimation, NPC_SELF, LVar2)
        // defeat
        EVT_IF_TRUE(GF_BowserDefeated)
            EVT_SET(MF_EnemyDefeated, TRUE)
            EVT_CALL(DoNpcDefeat)
            EVT_BREAK_LOOP
        EVT_END_IF
        EVT_LABEL(1)
        EVT_CALL(N(CheckPhase))
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

NpcGroupList N(DefaultNpcs) = {
    NPC_GROUP(N(NpcData_Bowser)),
    {}
};

