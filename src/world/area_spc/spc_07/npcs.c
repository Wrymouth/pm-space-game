#include "spc_07.h"
#include "sprite/npc/HuffNPuff.h"
#include "world/common/enemy/HuffNPuff.h"
#include "world/action/enemy_bullet.h"
#include "effects.h"
#include "world/action/damage_system.h"

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

API_CALLABLE(N(EnemyMoveToDestination)) {
    s32 enemyPosX = evt_get_variable(script, LVar0);
    s32 enemyPosY = evt_get_variable(script, LVar1);
    s32 moveSpeed = evt_get_variable(script, LVar3);

    s32 destX = evt_get_variable(script, MV_DestX);
    s32 destY = evt_get_variable(script, MV_DestY);
    
    f32 distX;
    f32 distY;

    s32 diffX = destX - enemyPosX;
    s32 diffY = destY - enemyPosY;
    f32 dist = dist2D(destX, destY, enemyPosX, enemyPosY);
    
    if (dist > 0) {
        distX = (diffX / dist) * moveSpeed;
        distY = (diffY / dist) * moveSpeed;
    }

    if (dist < moveSpeed) {
        enemyPosX = destX;
        enemyPosY = destY;
        distX = 0;
        distY = 0;
    }
    
    evt_set_variable(script, LVar0, (enemyPosX+distX));
    evt_set_variable(script, LVar1, (enemyPosY+distY));

    return ApiStatus_DONE2;
}

API_CALLABLE(N(SetEnemyDestination)) {
    s32 destX = rand_int(550) - 275;
    s32 destY = rand_int(300) - 150;

    evt_set_variable(script, MV_DestX, destX);
    evt_set_variable(script, MV_DestY, destY);
    evt_set_variable(script, MF_EnemyHasDestination, TRUE);
    return ApiStatus_DONE2;
}

API_CALLABLE(N(LightningHurtPlayer)) {
    s32 playerX = evt_get_variable(script, LVar0);
    s32 lightningX = evt_get_variable(script, MV_LightningX);

    if (playerX > lightningX - 30 && playerX < lightningX + 30) {
        player_take_damage(1);
    }
    return ApiStatus_DONE2;
}

EvtScript N(NpcIdle_HuffNPuff) = {
    EVT_SET(LVar3, 6) // moveSpeed
    EVT_LOOP(0)
        //movement
        EVT_CALL(GetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
        EVT_CALL(N(EnemyMoveToDestination))
        EVT_CALL(SetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
        EVT_IF_EQ(LVar0, MV_DestX)
            EVT_IF_EQ(LVar1, MV_DestY)
                EVT_CALL(N(SetEnemyDestination))
            EVT_END_IF
        EVT_END_IF
        // attacks
        EVT_CALL(GetPlayerPos, LVar0, LVar1, LVar2)
        EVT_IF_EQ(MV_LightningTimer, 120)
            // save player position and start lightning anim
            EVT_CALL(PlaySoundAtNpc, NPC_SELF, SOUND_209, 0)
            EVT_CALL(SetNpcAnimation, NPC_SELF, ANIM_HuffNPuff_Anim01)
            EVT_SET(MV_LightningX, LVar0)
        EVT_END_IF
        EVT_IF_EQ(MV_LightningTimer, 130)
            // do lightning strike
            EVT_CALL(PlaySound, SOUND_20A)
            EVT_CALL(PlayEffect, EFFECT_LIGHTNING_BOLT, 0, MV_LightningX, 170, LVar2, MV_LightningX, -250, LVar2, EVT_FLOAT(2.5), 15)
            EVT_SET(MF_LightningCanDamagePlayer, TRUE)
        EVT_END_IF
        EVT_IF_EQ(MV_LightningTimer, 135)
            EVT_SET(MF_LightningCanDamagePlayer, FALSE)
            EVT_SET(MV_LightningTimer, 0)
        EVT_END_IF
        EVT_IF_TRUE(MF_LightningCanDamagePlayer)
            EVT_CALL(N(LightningHurtPlayer))
        EVT_END_IF
        EVT_ADD(MV_LightningTimer, 1)
        // damage
        EVT_SET(LVar0, ANIM_HuffNPuff_Anim00)
        EVT_SET(LVar1, ANIM_HuffNPuff_Anim01)
        EVT_CALL(N(SetDamageAnimation), LVar0, LVar1, LVar2)
        EVT_CALL(SetNpcAnimation, NPC_SELF, LVar2)
        // defeat
        EVT_IF_TRUE(GF_HuffNPuffDefeated)
            EVT_SET(MF_EnemyDefeated, TRUE)
            EVT_CALL(DoNpcDefeat)
            EVT_BREAK_LOOP
        EVT_END_IF
        EVT_WAIT(1)
    EVT_END_LOOP
    EVT_RETURN
    EVT_END
};

EvtScript N(NpcInit_HuffNPuff) = {
    EVT_CALL(BindNpcIdle, NPC_SELF, EVT_PTR(N(NpcIdle_HuffNPuff)))
    EVT_RETURN
    EVT_END
};

NpcSettings N(NpcSettings_HuffNPuff) = {
    .height = 70,
    .radius = 70,
    .level = 99,
};

NpcData N(NpcData_HuffNPuff) = {
    .id = 2,
    .pos = { 0.0f, -213.0f, 16.0f },
    .init = &N(NpcInit_HuffNPuff),
    .yaw = 270,
    .settings = &N(NpcSettings_HuffNPuff),
    .flags = ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_ENABLE_HIT_SCRIPT,
    .drops = NO_DROPS,
    .animations = HUFF_N_PUFF_ANIMS,
    .aiDetectFlags = AI_DETECT_SIGHT,
    .maxHP = 32,
    .invFrames = 30,
    .defeatFlag = GF_HuffNPuffDefeated,
};

NpcGroupList N(DefaultNpcs) = {
    NPC_GROUP(N(NpcData_HuffNPuff)),
    {}
};

