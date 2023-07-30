#include "spc_01.h"
#include "world/common/npc/StarSpirit.inc.c"

EvtScript N(NpcInit_EldStar) = {
    EVT_CALL(EnableNpcShadow, NPC_SELF, FALSE)
    EVT_RETURN
    EVT_END
};

NpcData N(NpcData_StarSpirit) = {
    .id = 2,
    .pos = { -11.0f, 0.0f, 557.0f },
    .init = &N(NpcInit_EldStar),
    .yaw = 90,
    .settings = &N(NpcSettings_StarSpirit),
    .flags = ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_ENABLE_HIT_SCRIPT | ENEMY_FLAG_NO_SHADOW_RAYCAST,
    .drops = NO_DROPS,
    .animations = ELDSTAR_ANIMS,
    .aiDetectFlags = AI_DETECT_SIGHT,
    .maxHP = 10,
};

NpcGroupList N(DefaultNpcs) = {
    NPC_GROUP(N(NpcData_StarSpirit)),
    {}
};
