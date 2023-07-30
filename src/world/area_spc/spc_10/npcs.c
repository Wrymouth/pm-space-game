#include "spc_10.h"
#include "world/common/npc/Bowser.h"
#include "world/action/enemy_bullet.h"
#include "world/area_spc/common/enemy_behaviour.inc.c"

EvtScript N(NpcIdle_Bowser) = {
    EVT_SET(LVar3, 20) // direction
    EVT_LOOP(0)
        //movement
        EVT_SWITCH(MV_BattlePhase)
            EVT_CASE_EQ(0) // before battle
            EVT_CASE_EQ(1) // move left to right at top of screen
            EVT_CASE_EQ(2) // move up and down, switching sides at intervals
            EVT_CASE_EQ(3) // bounce around screen
        EVT_END_SWITCH
        // damage
        EVT_SET(LVar0, ANIM_WorldBowser_ClownCarIdle)
        EVT_SET(LVar1, ANIM_WorldBowser_ClownCarLaugh)
        EVT_CALL(N(SetDamageAnimation), LVar0, LVar1, LVar2)
        EVT_CALL(SetNpcAnimation, NPC_SELF, LVar2)
        // defeat
        EVT_IF_TRUE(GF_BowserDefeated)
            EVT_SET(MF_EnemyDefeated, TRUE)
            EVT_CALL(DoNpcDefeat)
            EVT_BREAK_LOOP
        EVT_END_IF
        EVT_WAIT(1)
    EVT_END_LOOP
    EVT_RETURN
    EVT_END
};

EvtScript N(NpcInit_Bowser) = {
    EVT_CALL(EnableNpcShadow, NPC_SELF, FALSE)
    EVT_CALL(BindNpcIdle, NPC_SELF, EVT_PTR(N(NpcIdle_Bowser)))
    EVT_RETURN
    EVT_END
};

NpcSettings N(NpcSettings_Bowser) = {
    .height = 64,
    .radius = 28,
    .level = 99,
};

NpcData N(NpcData_Bowser) = {
    .id = 2,
    .pos = { 0.0f, -213.0f, 16.0f },
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

