#include "common.h"
#include "variables.h"
#include "damage_system.h"

void player_take_damage(s32 amount) {
    if (!(gPlayerStatus.flags & PS_FLAG_HAZARD_INVINCIBILITY)) {
        gPlayerData.curHP -= amount;
        // gPlayerStatus.flags |= PS_FLAG_HAZARD_INVINCIBILITY;
    }
}

void enemy_take_damage(Enemy* enemy, s32 amount) {
    if (!(enemy->flags & ENEMY_FLAG_INVINCIBLE)) {
        enemy->curHP -= amount;
        enemy->flags |= ENEMY_FLAG_INVINCIBLE;
        enemy->invFrames = 0;
    }
}
