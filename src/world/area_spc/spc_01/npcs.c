#include "spc_01.h"
#include "world/common/npc/Bowser.h"
#include "world/common/npc/Bowser.inc.c"

NpcData N(NpcData_Bowser) = {
    .id = 2,
    .pos = { 200.0f, 100.0f, 16.0f },
    .yaw = 90,
    .settings = &N(NpcSettings_Bowser),
    .flags = ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_ENABLE_HIT_SCRIPT | ENEMY_FLAG_NO_SHADOW_RAYCAST,
    .drops = NO_DROPS,
    .animations = BOWSER_ANIMS,
    .aiDetectFlags = AI_DETECT_SIGHT,
    .maxHP = 10,
};

NpcGroupList N(DefaultNpcs) = {
    NPC_GROUP(N(NpcData_Bowser)),
    {}
};
