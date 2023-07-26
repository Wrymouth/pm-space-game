#include "spc_05.h"
#include "sprite/npc/JrTroopa.h"
#include "world/common/npc/JrTroopa.h"
#include "world/common/npc/StarSpirit.h"
#include "world/action/enemy_bullet.h"

API_CALLABLE(N(DoEggAttack)) {
    do_attack(script->owner1.enemy, ENEMY_ATTACK_TYPE_EGGS);
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

EvtScript N(NpcIdle_JrTroopa) = {
    EVT_SET(LVar3, 12) // direction
    EVT_LOOP(0)
        // movement
        EVT_CALL(GetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
        EVT_IF_LT(LVar1, MapYBottom)
            EVT_MUL(LVar3, -1)
        EVT_END_IF
        EVT_IF_GT(LVar1, MapYTop)
            EVT_MUL(LVar3, -1)
        EVT_END_IF
        EVT_ADD(LVar1, LVar3)
        EVT_CALL(SetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
        // attack
        EVT_IF_EQ(MV_EggsTimer, 40)
            EVT_CALL(N(DoEggAttack))
            EVT_SET(MV_EggsTimer, 0)
        EVT_END_IF
        EVT_ADD(MV_EggsTimer, 1)
        // damage
        EVT_SET(LVar0, ANIM_JrTroopa_Idle)
        EVT_SET(LVar1, ANIM_JrTroopa_Ashen_BurnHurt)
        EVT_CALL(N(SetDamageAnimation), LVar0, LVar1, LVar2)
        EVT_CALL(SetNpcAnimation, NPC_SELF, LVar2)
        EVT_WAIT(1)
        // defeat
        EVT_IF_TRUE(GF_JrTroopaDefeated)
            EVT_SET(MF_EnemyDefeated, TRUE)
            EVT_CALL(DoNpcDefeat)
            EVT_BREAK_LOOP
        EVT_END_IF
    EVT_END_LOOP
    EVT_RETURN
    EVT_END
};

EvtScript N(NpcInit_JrTroopa) = {
    EVT_CALL(BindNpcIdle, NPC_SELF, EVT_PTR(N(NpcIdle_JrTroopa)))
    EVT_RETURN
    EVT_END
};

NpcSettings N(NpcSettings_JrTroopa) = {
    .height = 64,
    .radius = 28,
    .level = 99,
};

NpcData N(NpcData_JrTroopa) = {
    .id = 3,
    .pos = { 240.0f, 0.0f, 16.0f },
    .init = &N(NpcInit_JrTroopa),
    .yaw = 270,
    .settings = &N(NpcSettings_JrTroopa),
    .flags = ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_ENABLE_HIT_SCRIPT | ENEMY_FLAG_NO_SHADOW_RAYCAST,
    .drops = NO_DROPS,
    .animations = JR_TROOPA_ANIMS,
    .aiDetectFlags = AI_DETECT_SIGHT,
    .maxHP = 16,
    .invFrames = 30,
    .defeatFlag = GF_JrTroopaDefeated,
};

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
    .flags = ENEMY_FLAG_PASSIVE | ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_ENABLE_HIT_SCRIPT | ENEMY_FLAG_NO_SHADOW_RAYCAST,
    .drops = NO_DROPS,
    .animations = ELDSTAR_ANIMS,
    .aiDetectFlags = AI_DETECT_SIGHT,
};

NpcGroupList N(DefaultNpcs) = {
    NPC_GROUP(N(NpcData_EldStar)),
    NPC_GROUP(N(NpcData_JrTroopa)),
    {}
};

