#ifndef ENEMY_BULLET_H
#define ENEMY_BULLET_H

#include "npc.h"

typedef enum EnemyAttackType {
    ENEMY_ATTACK_TYPE_LEFT,
    ENEMY_ATTACK_TYPE_HAMMER,
    ENEMY_ATTACK_TYPE_LEFT_RIGHT,
} EnemyAttackType;

typedef enum EnemyBulletType {
    ENEMY_BULLET_TYPE_LEFT,
    ENEMY_BULLET_TYPE_RIGHT,
    ENEMY_BULLET_TYPE_HAMMER,
} EnemyBulletType;

void use_enemy_bullet(Enemy* enemy, EnemyBulletType type);
s32 test_enemy_bullet_first_strike();

#endif // ENEMY_BULLET_H
