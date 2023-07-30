#include "spc_03.h"

#include "world/common/npc/StarSpirit.h"
#include "world/common/enemy/HammerBros.h"
#include "world/common/npc/ShiverToad.h"
#include "world/common/npc/KoopaBros.h"
#include "world/common/npc/Mouser.h"
#include "sprite/npc/ParadeHorn.h"


EvtScript N(NpcInit_EldStar) = {
    EVT_CALL(EnableNpcShadow, NPC_SELF, FALSE)
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
    .pos = { 0.0f, -1000.0f, 16.0f },
    .init = &N(NpcInit_EldStar),
    .yaw = 270,
    .settings = &N(NpcSettings_EldStar),
    .flags = ENEMY_FLAG_PASSIVE | ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_ENABLE_HIT_SCRIPT | ENEMY_FLAG_NO_SHADOW_RAYCAST,
    .drops = NO_DROPS,
    .animations = ELDSTAR_ANIMS,
    .aiDetectFlags = AI_DETECT_SIGHT,
};

EvtScript N(NpcInit_Hammer) = {
    EVT_CALL(EnableNpcShadow, NPC_SELF, FALSE)
    EVT_RETURN
    EVT_END
};

NpcSettings N(NpcSettings_Hammer) = {
    .height = 64,
    .radius = 28,
    .level = 99,
};

NpcData N(NpcData_Hammer) = {
    .id = 3,
    .pos = { 0.0f, -1000.0f, 16.0f },
    .init = &N(NpcInit_Hammer),
    .yaw = 270,
    .settings = &N(NpcSettings_Hammer),
    .flags = ENEMY_FLAG_PASSIVE | ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_ENABLE_HIT_SCRIPT | ENEMY_FLAG_NO_SHADOW_RAYCAST,
    .drops = NO_DROPS,
    .animations = HAMMER_BROS_ANIMS,
    .aiDetectFlags = AI_DETECT_SIGHT,
};

EvtScript N(NpcInit_Lakitu) = {
    EVT_CALL(EnableNpcShadow, NPC_SELF, FALSE)
    EVT_RETURN
    EVT_END
};

NpcSettings N(NpcSettings_Lakitu) = {
    .height = 64,
    .radius = 28,
    .level = 99,
};

NpcData N(NpcData_Lakitu) = {
    .id = 4,
    .pos = { 0.0f, -1000.0f, 16.0f },
    .init = &N(NpcInit_Lakitu),
    .yaw = 270,
    .settings = &N(NpcSettings_Lakitu),
    .flags = ENEMY_FLAG_PASSIVE | ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_ENABLE_HIT_SCRIPT | ENEMY_FLAG_NO_SHADOW_RAYCAST,
    .drops = NO_DROPS,
    .animations = SHIVER_TOAD_RED_ANIMS,
    .aiDetectFlags = AI_DETECT_SIGHT,
};

EvtScript N(NpcInit_Whale) = {
    EVT_CALL(EnableNpcShadow, NPC_SELF, FALSE)
    EVT_RETURN
    EVT_END
};

NpcSettings N(NpcSettings_Whale) = {
    .height = 64,
    .radius = 28,
    .level = 99,
};

NpcData N(NpcData_Whale) = {
    .id = 5,
    .pos = { 0.0f, -1000.0f, 16.0f },
    .init = &N(NpcInit_Whale),
    .yaw = 270,
    .settings = &N(NpcSettings_Whale),
    .flags = ENEMY_FLAG_PASSIVE | ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_ENABLE_HIT_SCRIPT | ENEMY_FLAG_NO_SHADOW_RAYCAST,
    .drops = NO_DROPS,
    .animations = MOUSER_BLUE_ANIMS,
    .aiDetectFlags = AI_DETECT_SIGHT,
};

EvtScript N(NpcInit_Koopa) = {
    EVT_CALL(EnableNpcShadow, NPC_SELF, FALSE)
    EVT_RETURN
    EVT_END
};

NpcSettings N(NpcSettings_Koopa) = {
    .height = 64,
    .radius = 28,
    .level = 99,
};

NpcData N(NpcData_Koopa) = {
    .id = 6,
    .pos = { 0.0f, -1000.0f, 16.0f },
    .init = &N(NpcInit_Koopa),
    .yaw = 270,
    .settings = &N(NpcSettings_Koopa),
    .flags = ENEMY_FLAG_PASSIVE | ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_ENABLE_HIT_SCRIPT | ENEMY_FLAG_NO_SHADOW_RAYCAST,
    .drops = NO_DROPS,
    .animations = RED_KOOPA_BROS_ANIMS,
    .aiDetectFlags = AI_DETECT_SIGHT,
};

EvtScript N(NpcInit_Static) = {
    EVT_CALL(EnableNpcShadow, NPC_SELF, FALSE)
    EVT_RETURN
    EVT_END
};

NpcSettings N(NpcSettings_Static) = {
    .height = 64,
    .radius = 28,
    .level = 99,
};

NpcData N(NpcData_Static) = {
    .id = 7,
    .pos = { 0.0f, -1000.0f, 16.0f },
    .init = &N(NpcInit_Static),
    .yaw = 270,
    .settings = &N(NpcSettings_Static),
    .flags = ENEMY_FLAG_PASSIVE | ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_ENABLE_HIT_SCRIPT | ENEMY_FLAG_NO_SHADOW_RAYCAST,
    .drops = NO_DROPS,
    .animations = {
            .idle   = ANIM_ParadeHorn_Idle,
            .walk   = ANIM_ParadeHorn_Idle,
            .run    = ANIM_ParadeHorn_Idle,
            .chase  = ANIM_ParadeHorn_Idle,
            .anim_4 = ANIM_ParadeHorn_Idle,
            .anim_5 = ANIM_ParadeHorn_Idle,
            .death  = ANIM_ParadeHorn_Idle,
            .hit    = ANIM_ParadeHorn_Idle,
            .anim_8 = ANIM_ParadeHorn_Idle,
            .anim_9 = ANIM_ParadeHorn_Idle,
            .anim_A = ANIM_ParadeHorn_Idle,
            .anim_B = ANIM_ParadeHorn_Idle,
            .anim_C = ANIM_ParadeHorn_Idle,
            .anim_D = ANIM_ParadeHorn_Idle,
            .anim_E = ANIM_ParadeHorn_Idle,
            .anim_F = ANIM_ParadeHorn_Idle,
    },
    .aiDetectFlags = AI_DETECT_SIGHT,
};

NpcGroupList N(DefaultNpcs) = {
    NPC_GROUP(N(NpcData_EldStar)),
    NPC_GROUP(N(NpcData_Hammer)),
    NPC_GROUP(N(NpcData_Lakitu)),
    NPC_GROUP(N(NpcData_Whale)),
    NPC_GROUP(N(NpcData_Koopa)),
    NPC_GROUP(N(NpcData_Static)),
    {}
};
