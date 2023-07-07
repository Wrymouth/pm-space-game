#include "spc_05.h"
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

API_CALLABLE(N(HammerShipUseBullet)) {
    use_enemy_bullet(script->owner1.enemy, ENEMY_BULLET_HAMMER);
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

EvtScript N(NpcIdle_HammerBroShip) = {
    EVT_LOOP(0)
        //movement
        EVT_IF_EQ(MV_AttackTimer, 15)
            EVT_CALL(N(HammerShipUseBullet))
            EVT_SET(MV_AttackTimer, 0)
        EVT_END_IF
        // damage
        EVT_SET(LVar0, ANIM_ParadeYoshi_StillGreen)
        EVT_SET(LVar1, ANIM_ParadeYoshi_IdleGreen)
        EVT_CALL(N(SetDamageAnimation), LVar0, LVar1, LVar2)
        EVT_DEBUG_PRINT_VAR(LVar2)
        EVT_CALL(SetNpcAnimation, NPC_SELF, LVar2)
        EVT_ADD(MV_AttackTimer, 1)
        EVT_WAIT(1)
    EVT_END_LOOP
    EVT_RETURN
    EVT_END
};

EvtScript N(NpcInit_HammerBroShip) = {
    EVT_CALL(BindNpcIdle, NPC_SELF, EVT_PTR(N(NpcIdle_HammerBroShip)))
    EVT_RETURN
    EVT_END
};

NpcSettings N(NpcSettings_HammerBroShip) = {
    .height = 50,
    .radius = 18,
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
    .maxHP = 10,
    .invFrames = 60,
};

NpcGroupList N(DefaultNpcs) = {
    NPC_GROUP(N(NpcData_HammerBroShip)),
    {}
};

