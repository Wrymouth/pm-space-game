#include "spc_00.h"
#include "world/common/npc/Bowser.h"
#include "world/common/npc/Bowser.inc.c"

NpcData N(NpcData_Bowser) = {
    .id = 2,
    .pos = { 200.0f, 50.0f, 16.0f },
    .init = &N(NpcInit_Bowser),
    .yaw = 90,
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

API_CALLABLE(N(SetDamageBlink)) {
    Bytecode* args = script->ptrReadPos;
    AnimID animID = *args++;
    s32 outVar = *args++;
    Enemy* enemy = script->owner1.enemy;
    s32 blink;

    if (enemy->invFrames >= enemy->invTimer) {
        enemy->flags &= ~ENEMY_FLAG_INVINCIBLE;
        enemy->invFrames = 0;
    }
    if (enemy->flags & ENEMY_FLAG_INVINCIBLE) {
        enemy->invFrames++;
        if (enemy->invFrames % 10 == 0) {
            blink = (animID & 0x00000700) ? TRUE : FALSE; // is blink palette currently active?
            evt_set_variable(script, outVar, !blink);
        }
    }
}


EvtScript N(NpcInit_Bowser) = {
    EVT_CALL(BindNpcHit, NPC_SELF, EVT_PTR(N(NpcIdle_Bowser)))
    EVT_RETURN
    EVT_END
};

EvtScript N(NpcIdle_Bowser) = {
    // set inv flag
    EVT_LOOP(0)
        EVT_CALL(GetNpcAnimation, NPC_SELF, LVar0)
        EVT_CALL(N(SetDamageBlink), LVar1, LVar0)
        EVT_IF_EQ(LVar0, TRUE) 
            EVT_SUB(LVar1, 0x00000700)
        EVT_ELSE
            EVT_ADD(LVar1, 0x00000700)
        EVT_END_IF
        EVT_CALL(SetNpcAnimation, NPC_SELF, LVar1)
        EVT_WAIT(1)
    EVT_END_LOOP
    // unset inv flag
    EVT_RETURN
    EVT_END
};


