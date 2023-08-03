#include "spc_05.h"
#include "sprite/npc/JrTroopa.h"
#include "world/common/npc/JrTroopa.h"
#include "world/common/npc/StarSpirit.h"
#include "world/action/enemy_bullet.h"
#include "world/area_spc/common/enemy_behaviour.inc.c"

API_CALLABLE(N(CheckPhase)) {
    Enemy* enemy = script->owner1.enemy;

    if (enemy->curHP <= 8) {
        evt_set_variable(script, MV_BattlePhase, 2);
    }
    return ApiStatus_DONE2;
}

EvtScript N(NpcIdle_JrTroopa) = {
    EVT_SET(LVar3, 12) // direction
    EVT_LOOP(0)
        // movement
        EVT_CALL(GetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
        EVT_IF_TRUE(MV_BattlePhase)
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
                EVT_CALL(PlaySound, 0x0F)
                EVT_CALL(N(DoAttack), ENEMY_ATTACK_TYPE_EGGS)
                EVT_SET(MV_EggsTimer, 0)
            EVT_END_IF
            EVT_ADD(MV_EggsTimer, 1)
            EVT_IF_EQ(MV_BattlePhase, 2)
                EVT_IF_EQ(MV_SummonTimer, 60)
                    EVT_CALL(PlaySound, SOUND_SPELL_CAST1)
                    EVT_CALL(N(DoAttack), ENEMY_ATTACK_TYPE_SUMMON)
                    EVT_SET(MV_SummonTimer, 0)
                EVT_END_IF
                EVT_ADD(MV_SummonTimer, 1)
            EVT_END_IF
            // damage
            EVT_SET(LVar0, ANIM_JrTroopa_Idle)
            EVT_SET(LVar1, ANIM_JrTroopa_Ashen_BurnHurt)
            EVT_CALL(N(SetDamageAnimation), LVar0, LVar1, LVar2)
            EVT_CALL(SetNpcAnimation, NPC_SELF, LVar2)
        EVT_END_IF
        EVT_CALL(N(CheckPhase))
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
    EVT_CALL(EnableNpcShadow, NPC_SELF, FALSE)
    EVT_CALL(BindNpcIdle, NPC_SELF, EVT_PTR(N(NpcIdle_JrTroopa)))
    EVT_CALL(SetNpcScale, NPC_SELF, EVT_FLOAT(2.0), EVT_FLOAT(2.0), EVT_FLOAT(2.0))
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
    .pos = { 0.0f, -1000.0f, 0.0f },
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
    .pos = { 0.0f, -213.0f, 16.0f },
    .init = &N(NpcInit_EldStar),
    .yaw = 270,
    .settings = &N(NpcSettings_EldStar),
    .flags = ENEMY_FLAG_PASSIVE | ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_IGNORE_PLAYER_COLLISION | ENEMY_FLAG_ENABLE_HIT_SCRIPT | ENEMY_FLAG_NO_SHADOW_RAYCAST,
    .drops = NO_DROPS,
    .animations = ELDSTAR_ANIMS,
    .aiDetectFlags = AI_DETECT_SIGHT,
};

NpcGroupList N(DefaultNpcs) = {
    NPC_GROUP(N(NpcData_EldStar)),
    NPC_GROUP(N(NpcData_JrTroopa)),
    {}
};

