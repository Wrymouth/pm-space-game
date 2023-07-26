#include "spc_08.h"
#include "sprite/npc/ParadeYoshi.h"
#include "world/action/enemy_bullet.h"

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

API_CALLABLE(N(DoWaterAttack)) {
    do_attack(script->owner1.enemy, ENEMY_ATTACK_TYPE_WATER);
    return ApiStatus_DONE2;
}

API_CALLABLE(N(GetHeading)) {
    s32 enemyPosX  = evt_get_variable(script, LVar0);
    s32 enemyPosY  = evt_get_variable(script, LVar1); 
    s32 targetPosX = evt_get_variable(script, LVar4); 
    s32 targetPosY = evt_get_variable(script, LVar5);
    s32 moveSpeed  = evt_get_variable(script, LVar3);
    s32 directionX;
    s32 directionY;

    f32 distX;
    f32 distY;

    s32 diffX = targetPosX - enemyPosX;
    s32 diffY = targetPosY - enemyPosY;
    f32 dist = dist2D(targetPosX, targetPosY, enemyPosX, enemyPosY);
    
    if (dist > 0) {
        distX = (diffX / dist) * moveSpeed;
        distY = (diffY / dist) * moveSpeed;
    }

    evt_set_variable(script, LVar0, (enemyPosX+distX));
    evt_set_variable(script, LVar1, (enemyPosY+distY));

    return ApiStatus_DONE2;
}

API_CALLABLE(N(SetDamageAnimation)) {
    Bytecode* args = script->ptrReadPos;
    AnimID standardAnimID = evt_get_variable(script, *args++);
    AnimID hurtAnimID = evt_get_variable(script, *args++);
    AnimID outVar = *args++;
    Enemy* enemy = script->owner1.enemy;
    s32 blink;

    if (enemy->invFrames >= enemy->invTimer) {
        enemy->flags &= ~ENEMY_FLAG_INVINCIBLE;
        enemy->invFrames = 0;
    }
    if (enemy->flags & ENEMY_FLAG_INVINCIBLE) {
        enemy->invFrames++;
        evt_set_variable(script, outVar, hurtAnimID);
    } else {
        evt_set_variable(script, outVar, standardAnimID);
    }
    return ApiStatus_DONE2;
}

EvtScript N(NpcIdle_Whale) = {
    EVT_SET(LVar3, 4) // moveSpeed
    EVT_LOOP(0)
        // movement
        EVT_CALL(GetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
        EVT_CALL(GetPlayerPos, LVar4, LVar5, LVar6)
        EVT_CALL(N(GetHeading))
        EVT_CALL(SetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
        EVT_ADD(LVar0, 25)
        EVT_ADD(LVar1, 46)
        EVT_CALL(TranslateGroup, Model_Whale, LVar0, LVar1, LVar2)
        // attack
        EVT_IF_EQ(MV_WaterTimer, 90)
            EVT_IF_EQ(MV_WaterSubTimer, 8)
                EVT_CALL(N(DoWaterAttack))
                EVT_SET(MV_WaterSubTimer, 0)
                EVT_ADD(MV_WaterCount, 1)
            EVT_END_IF
            EVT_IF_EQ(MV_WaterCount, 3)
                EVT_SET(MV_WaterTimer, 0)
                EVT_SET(MV_WaterCount, 0)
            EVT_ELSE
                EVT_ADD(MV_WaterSubTimer, 1)
            EVT_END_IF
        EVT_ELSE
            EVT_ADD(MV_WaterTimer, 1)
        EVT_END_IF
        // damage
        EVT_SET(LVar0, ANIM_ParadeYoshi_StillGreen)
        EVT_SET(LVar1, ANIM_ParadeYoshi_StillBlue)
        EVT_CALL(N(SetDamageAnimation), LVar0, LVar1, LVar2)
        EVT_CALL(SetNpcAnimation, NPC_SELF, LVar2)
        EVT_WAIT(1)
        // defeat
        EVT_IF_TRUE(GF_WhaleDefeated)
            EVT_SET(MF_EnemyDefeated, TRUE)
            EVT_CALL(DoNpcDefeat)
            EVT_BREAK_LOOP
        EVT_END_IF
    EVT_END_LOOP
    EVT_RETURN
    EVT_END
};

EvtScript N(NpcDefeat_Whale) = {
    EVT_CALL(TranslateGroup, Model_Whale, 0, 0, -20)
    EVT_RETURN
    EVT_END
};

EvtScript N(NpcInit_Whale) = {
    EVT_CALL(BindNpcIdle, NPC_SELF, EVT_PTR(N(NpcIdle_Whale)))
    EVT_CALL(BindNpcDefeat, NPC_SELF, EVT_PTR(N(NpcDefeat_Whale)))
    EVT_RETURN
    EVT_END
};

NpcSettings N(NpcSettings_Whale) = {
    .height = 80,
    .radius = 220,
    .level = 99,
};

NpcData N(NpcData_Whale) = {
    .id = 2,
    .pos = { 0.0f, -213.0f, 16.0f },
    .init = &N(NpcInit_Whale),
    .yaw = 270,
    .settings = &N(NpcSettings_Whale),
    .flags = ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_ENABLE_HIT_SCRIPT | ENEMY_FLAG_NO_SHADOW_RAYCAST,
    .drops = NO_DROPS,
    .animations = HAMMER_BRO_SHIP_ANIMS,
    .aiDetectFlags = AI_DETECT_SIGHT,
    .maxHP = 16,
    .invFrames = 30,
    .defeatFlag = GF_WhaleDefeated,
};

NpcGroupList N(DefaultNpcs) = {
    NPC_GROUP(N(NpcData_Whale)),
    {}
};

