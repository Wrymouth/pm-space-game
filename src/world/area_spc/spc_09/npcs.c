#include "spc_09.h"
#include "sprite/npc/ParadeKolorado.h"
#include "world/action/enemy_bullet.h"

API_CALLABLE(N(HammerShipUseHammer)) {
    use_enemy_bullet(script->owner1.enemy, ENEMY_BULLET_TYPE_HAMMER);
    return ApiStatus_DONE2;
}

API_CALLABLE(N(HammerShipUseBullet)) {
    use_enemy_bullet(script->owner1.enemy, ENEMY_BULLET_TYPE_LEFT);
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

EvtScript N(NpcIdle_KoopaBrosShip) = {
    EVT_SET(LVar3, 20) // direction
    EVT_LOOP(0)
        //movement
        
        // damage
        EVT_SET(LVar0, ANIM_ParadeKolorado_Idle)
        EVT_SET(LVar1, ANIM_ParadeKolorado_WifeIdle)
        EVT_CALL(N(SetDamageAnimation), LVar0, LVar1, LVar2)
        EVT_DEBUG_PRINT_VAR(LVar2)
        EVT_CALL(SetNpcAnimation, NPC_SELF, LVar2)
        EVT_WAIT(1)
    EVT_END_LOOP
    EVT_RETURN
    EVT_END
};

EvtScript N(NpcInit_KoopaBrosShip) = {
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
    .pos = { 0.0f, -213.0f, 16.0f },
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

