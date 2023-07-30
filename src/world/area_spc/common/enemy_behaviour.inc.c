#include "world/action/enemy_bullet.h"

API_CALLABLE(N(DoAttack)) {
    Bytecode* args = script->ptrReadPos;

    do_attack(script->owner1.enemy, *args++);
    return ApiStatus_DONE2;
}

API_CALLABLE(N(SetDamageAnimation)) {
    Bytecode* args = script->ptrReadPos;
    AnimID standardAnimID = evt_get_variable(script, *args++);
    AnimID hurtAnimID = evt_get_variable(script, *args++);
    AnimID currentAnimID;
    AnimID outVar = *args++;
    Enemy* enemy = script->owner1.enemy;
    Npc* enemyNpc = get_npc_safe(enemy->npcID);

    currentAnimID = enemyNpc->currentAnim;

    if (enemy->invFrames >= enemy->invTimer) {
        enemy->flags &= ~ENEMY_FLAG_INVINCIBLE;
        enemy->invFrames = 0;
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
