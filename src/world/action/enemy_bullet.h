#ifndef ENEMY_BULLET_H
#define ENEMY_BULLET_H

#include "npc.h"

typedef enum EnemyAttackType {
    ENEMY_ATTACK_TYPE_LEFT,
    ENEMY_ATTACK_TYPE_HAMMER,
    ENEMY_ATTACK_TYPE_LEFT_RIGHT,
    ENEMY_ATTACK_TYPE_KOOPA_SHELLS,
    ENEMY_ATTACK_TYPE_SPLIT,
} EnemyAttackType;

typedef enum EnemyBulletType {
    ENEMY_BULLET_TYPE_LEFT,
    ENEMY_BULLET_TYPE_RIGHT,
    ENEMY_BULLET_TYPE_HAMMER,
    ENEMY_BULLET_TYPE_KOOPA_UL,
    ENEMY_BULLET_TYPE_KOOPA_UR,
    ENEMY_BULLET_TYPE_KOOPA_DL,
    ENEMY_BULLET_TYPE_KOOPA_DR,
} EnemyBulletType;

void do_attack(Enemy* enemy, EnemyAttackType type);
s32 test_enemy_bullet_first_strike();

ApiStatus ClearAllEnemyBullets(Evt* script, s32 isInitialCall);

#endif // ENEMY_BULLET_H
