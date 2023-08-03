#include "spc_09.h"
#include "sprite/npc/ParadeKolorado.h"
#include "world/action/enemy_bullet.h"
#include "world/area_spc/common/enemy_behaviour.inc.c"

EvtScript N(NpcIdle_KoopaBrosShip) = {
    EVT_SET(LVar3, 20) // direction
    EVT_LOOP(0)
        //movement
        EVT_IF_EQ(MV_SplitTimer, 50)
            EVT_CALL(PlaySoundAtNpc, NPC_SELF, SOUND_20EB)
            EVT_CALL(N(DoAttack), ENEMY_ATTACK_TYPE_SPLIT)
            EVT_SET(MV_SplitTimer, 0)
        EVT_END_IF
        EVT_IF_EQ(MV_ShellTimer, 15)
            EVT_CALL(PlaySound, SOUND_2023)
            EVT_CALL(N(DoAttack), ENEMY_ATTACK_TYPE_KOOPA_SHELLS)
        EVT_END_IF
        EVT_ADD(MV_SplitTimer, 1)
        EVT_ADD(MV_ShellTimer, 1)
        // damage
        EVT_SET(LVar0, ANIM_ParadeKolorado_Idle)
        EVT_SET(LVar1, ANIM_ParadeKolorado_WifeIdle)
        EVT_CALL(N(SetDamageAnimation), LVar0, LVar1, LVar2)
        EVT_CALL(SetNpcAnimation, NPC_SELF, LVar2)
        // defeat
        EVT_IF_TRUE(GF_KoopaBrosDefeated)
            EVT_SET(MF_EnemyDefeated, TRUE)
            EVT_CALL(DoNpcDefeat)
            EVT_BREAK_LOOP
        EVT_END_IF
        EVT_WAIT(1)
    EVT_END_LOOP
    EVT_RETURN
    EVT_END
};

EvtScript N(NpcInit_KoopaBrosShip) = {
    EVT_CALL(EnableNpcShadow, NPC_SELF, FALSE)
    EVT_CALL(BindNpcIdle, NPC_SELF, EVT_PTR(N(NpcIdle_KoopaBrosShip)))
    EVT_RETURN
    EVT_END
};

NpcSettings N(NpcSettings_KoopaBrosShip) = {
    .height = 64,
    .radius = 28,
    .level = 99,
};

NpcData N(NpcData_KoopaBrosShip) = {
    .id = 2,
    .pos = { 90.0f, 0.0f, 16.0f },
    .init = &N(NpcInit_KoopaBrosShip),
    .yaw = 270,
    .settings = &N(NpcSettings_KoopaBrosShip),
    .flags = ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_ENABLE_HIT_SCRIPT | ENEMY_FLAG_NO_SHADOW_RAYCAST,
    .drops = NO_DROPS,
    .animations = {
            .idle   = ANIM_ParadeKolorado_Idle,
            .walk   = ANIM_ParadeKolorado_Idle,
            .run    = ANIM_ParadeKolorado_Idle,
            .chase  = ANIM_ParadeKolorado_Idle,
            .anim_4 = ANIM_ParadeKolorado_Idle,
            .anim_5 = ANIM_ParadeKolorado_Idle,
            .death  = ANIM_ParadeKolorado_Idle,
            .hit    = ANIM_ParadeKolorado_Idle,
            .anim_8 = ANIM_ParadeKolorado_Idle,
            .anim_9 = ANIM_ParadeKolorado_Idle,
            .anim_A = ANIM_ParadeKolorado_Idle,
            .anim_B = ANIM_ParadeKolorado_Idle,
            .anim_C = ANIM_ParadeKolorado_Idle,
            .anim_D = ANIM_ParadeKolorado_Idle,
            .anim_E = ANIM_ParadeKolorado_Idle,
            .anim_F = ANIM_ParadeKolorado_Idle,
        },
    .aiDetectFlags = AI_DETECT_SIGHT,
    .maxHP = 32,
    .invFrames = 30,
    .defeatFlag = GF_KoopaBrosDefeated,
};

NpcGroupList N(DefaultNpcs) = {
    NPC_GROUP(N(NpcData_KoopaBrosShip)),
    {}
};

