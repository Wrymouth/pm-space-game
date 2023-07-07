#ifndef ENEMY_BULLET_H
#define ENEMY_BULLET_H

#include "npc.h"

typedef enum EnemyBulletType {
    ENEMY_BULLET_STRAIGHT,
    ENEMY_BULLET_HAMMER,
} EnemyBulletType;

void use_enemy_bullet(Enemy* enemy, EnemyBulletType type);
s32 test_enemy_bullet_first_strike();

#endif // ENEMY_BULLET_H
