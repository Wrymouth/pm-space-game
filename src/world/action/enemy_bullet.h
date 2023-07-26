#ifndef ENEMY_BULLET_H
#define ENEMY_BULLET_H

#include "npc.h"

typedef enum EnemyAttackType {
    ENEMY_ATTACK_TYPE_LEFT,
    ENEMY_ATTACK_TYPE_HAMMER,
    ENEMY_ATTACK_TYPE_LEFT_RIGHT,
    ENEMY_ATTACK_TYPE_KOOPA_SHELLS,
    ENEMY_ATTACK_TYPE_SPLIT,
    ENEMY_ATTACK_TYPE_WATER,
    ENEMY_ATTACK_TYPE_EGGS,
    ENEMY_ATTACK_TYPE_SPINIES,
} EnemyAttackType;

typedef enum EnemyBulletType {
    ENEMY_BULLET_TYPE_LEFT,
    ENEMY_BULLET_TYPE_RIGHT,
    ENEMY_BULLET_TYPE_HAMMER,
    ENEMY_BULLET_TYPE_KOOPA_UL,
    ENEMY_BULLET_TYPE_KOOPA_UR,
    ENEMY_BULLET_TYPE_KOOPA_DL,
    ENEMY_BULLET_TYPE_KOOPA_DR,
    ENEMY_BULLET_TYPE_WATER,
    ENEMY_BULLET_TYPE_EGG_UP,
    ENEMY_BULLET_TYPE_EGG_MID,
    ENEMY_BULLET_TYPE_EGG_DOWN,
} EnemyBulletType;

enum EnemyBulletFlags {
    ENEMY_BULLET_FLAG_FACING_LEFT = 0x0001,
    ENEMY_BULLET_FLAG_WATER_UP = 0x0002,
    ENEMY_BULLET_FLAG_WATER_DOWN = 0x0004,
};

void do_attack(Enemy* enemy, EnemyAttackType type);
s32 test_enemy_bullet_first_strike();
void clear_enemy_bullets(void);
ApiStatus ClearAllEnemyBullets(Evt* script, s32 isInitialCall);

#endif // ENEMY_BULLET_H
