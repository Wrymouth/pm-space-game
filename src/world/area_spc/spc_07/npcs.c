#include "spc_07.h"
#include "sprite/npc/HuffNPuff.h"
#include "world/common/enemy/HuffNPuff.h"
#include "world/action/enemy_bullet.h"

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

EvtScript N(NpcIdle_HuffNPuff) = {
    EVT_SET(LVar3, 20) // direction
    EVT_LOOP(0)
        //movement
        // damage
        EVT_SET(LVar0, ANIM_HuffNPuff_Anim00)
        EVT_SET(LVar1, ANIM_HuffNPuff_Anim01)
        EVT_CALL(N(SetDamageAnimation), LVar0, LVar1, LVar2)
        EVT_DEBUG_PRINT_VAR(LVar2)
        EVT_CALL(SetNpcAnimation, NPC_SELF, LVar2)
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
    .height = 64,
    .radius = 28,
    .level = 99,
};

NpcData N(NpcData_HuffNPuff) = {
    .id = 2,
    .pos = { 0.0f, -213.0f, 16.0f },
    .init = &N(NpcInit_HuffNPuff),
    .yaw = 270,
    .settings = &N(NpcSettings_HuffNPuff),
    .flags = ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_ENABLE_HIT_SCRIPT | ENEMY_FLAG_NO_SHADOW_RAYCAST,
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

