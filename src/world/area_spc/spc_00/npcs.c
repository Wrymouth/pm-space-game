#include "spc_00.h"
#include "world/common/npc/Bowser.h"
#include "world/common/npc/Bowser.inc.c"
#include "world/action/enemy_bullet.h"

NpcData N(NpcData_Bowser) = {
    .id = 2,
    .pos = { 200.0f, -50.0f, 16.0f },
    .init = &N(NpcInit_Bowser),
    .yaw = 270,
    .settings = &N(NpcSettings_Bowser),
    .flags = ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_ENABLE_HIT_SCRIPT | ENEMY_FLAG_NO_SHADOW_RAYCAST,
    .drops = NO_DROPS,
    .animations = BOWSER_ANIMS,
    .aiDetectFlags = AI_DETECT_SIGHT,
    .maxHP = 10,
    .invFrames = 60,
};

NpcGroupList N(DefaultNpcs) = {
    NPC_GROUP(N(NpcData_Bowser)),
    {}
};

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

API_CALLABLE(N(BowserUseBullet)) {
    use_enemy_bullet(script->owner1.enemy, ENEMY_BULLET_TYPE_LEFT);
    return ApiStatus_DONE2;
}


EvtScript N(NpcInit_Bowser) = {
    EVT_CALL(BindNpcIdle, NPC_SELF, EVT_PTR(N(NpcIdle_Bowser)))
    EVT_RETURN
    EVT_END
};

EvtScript N(NpcIdle_Bowser) = {
    EVT_LOOP(0)
        //movement
        EVT_IF_EQ(MV_AttackTimer, 15)
            EVT_CALL(N(BowserUseBullet))
            EVT_SET(MV_AttackTimer, 0)
        EVT_END_IF
        // damage
        EVT_SET(LVar0, ANIM_WorldBowser_Idle)
        EVT_SET(LVar1, ANIM_WorldBowser_Shock)
        EVT_CALL(N(SetDamageAnimation), LVar0, LVar1, LVar2)
        EVT_DEBUG_PRINT_VAR(LVar2)
        EVT_CALL(SetNpcAnimation, NPC_SELF, LVar2)
        EVT_ADD(MV_AttackTimer, 1)
        EVT_WAIT(1)
    EVT_END_LOOP
    EVT_RETURN
    EVT_END
};


