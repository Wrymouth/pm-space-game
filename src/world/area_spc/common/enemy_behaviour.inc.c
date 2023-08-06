#include "world/action/enemy_bullet.h"
#include "effects.h"

API_CALLABLE(N(DoAttack)) {
    Bytecode* args = script->ptrReadPos;

    do_attack(script->owner1.enemy, *args++);
    return ApiStatus_DONE2;
}

API_CALLABLE(N(SetDamageAnimation)) {
    Bytecode* args           = script->ptrReadPos;
    AnimID    standardAnimID = evt_get_variable(script, *args++);
    AnimID    hurtAnimID     = evt_get_variable(script, *args++);
    AnimID    currentAnimID;
    AnimID    outVar   = *args++;
    Enemy*    enemy    = script->owner1.enemy;
    Npc*      enemyNpc = get_npc_safe(enemy->npcID);

    currentAnimID = enemyNpc->curAnim;

    if (enemy->invFrames >= enemy->invTimer) {
        enemy->flags     &= ~ENEMY_FLAG_INVINCIBLE;
        enemy->invFrames  = 0;
    }
    if (enemy->flags & ENEMY_FLAG_INVINCIBLE) {
        enemy->invFrames++;
        evt_set_variable(script, outVar, hurtAnimID);
    } else if (hurtAnimID == currentAnimID) {
        evt_set_variable(script, outVar, standardAnimID);
    } else {
        evt_set_variable(script, outVar, currentAnimID);
    }
    return ApiStatus_DONE2;
}

API_CALLABLE(N(Fast_NpcFacePlayer)) {
    Bytecode* args = script->ptrReadPos;
    s32 npcID = evt_get_variable(script, *args++);
    Npc* npc = resolve_npc(script, npcID);

    if (gPlayerStatus.pos.x < npc->pos.x) {
        npc->yaw       = 270;
    } else {
        npc->yaw       = 90;
    }

    return ApiStatus_DONE2;
}

/**
 * @arg LVar0 NpcID
 * @arg LVar1 TalkAnim
 * @arg LVar2 IdleAnim
 * @arg LVar3 String to display
 * @arg LVar4 Duration
*/ 
EvtScript N(ShowCharacterString) = {
    EVT_CALL(PlayEffect, EFFECT_TATTLE_WINDOW, 0, 300, 45, 0, EVT_FLOAT(0.4), LVar4)
    EVT_WAIT(10)
    EVT_CALL(SetNpcScale, LVar0, EVT_FLOAT(2.0), EVT_FLOAT(2.0), EVT_FLOAT(2.0))
    EVT_CALL(SetNpcPos, LVar0, 225, 100, 16)
    EVT_CALL(SpeakToPlayer, LVar0, LVar1, LVar2, 5, LVar3)
    EVT_CALL(SetNpcPos, LVar0, 0, -1000, 0)
    EVT_CALL(SetNpcScale, LVar0, EVT_FLOAT(1.0), EVT_FLOAT(1.0), EVT_FLOAT(1.0))
    EVT_RETURN
    EVT_END
};
