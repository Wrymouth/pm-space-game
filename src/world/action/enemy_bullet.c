#include "common.h"
#include "variables.h"
#include "npc.h"
#include "world/partners.h"
#include "sprite/npc/Fire.h"
#include "sprite/npc/HammerBros.h"
#include "sprite/player.h"
#include "world/action/enemy_bullet.h"

#define ENEMY_BULLET_EMPTY -1

typedef struct EnemyBulletStatus {
    s32 npcID;
    s32 activeBulletIndex;
    s32 activeBulletTime;
    s8  facingLeft;
} EnemyBulletStatus;

const s32 MAX_ENEMY_BULLETS  = 10;
const s32 ENEMY_BULLET_DECAY = 50;
s32       enemyBulletCount   = 0;

EnemyBulletStatus enemy_bullets[] = { { 0, ENEMY_BULLET_EMPTY }, { 0, ENEMY_BULLET_EMPTY }, { 0, ENEMY_BULLET_EMPTY },
                                      { 0, ENEMY_BULLET_EMPTY }, { 0, ENEMY_BULLET_EMPTY }, { 0, ENEMY_BULLET_EMPTY },
                                      { 0, ENEMY_BULLET_EMPTY }, { 0, ENEMY_BULLET_EMPTY }, { 0, ENEMY_BULLET_EMPTY },
                                      { 0, ENEMY_BULLET_EMPTY } };

s32 get_enemy_bullet_index_by_npc_id(s32 npcId) {
    s32 i;
    for (i = 0; i < ARRAY_COUNT(enemy_bullets); i++) {
        if (npcId == enemy_bullets[i].npcID) {
            return i;
        }
    }
    return -1;
}

void destroy_enemy_bullet(s32 bulletIndex) {
    free_npc_by_index(enemy_bullets[bulletIndex].activeBulletIndex);
    enemy_bullets[bulletIndex].npcID = 0;
    enemy_bullets[bulletIndex].activeBulletIndex = ENEMY_BULLET_EMPTY;
    enemyBulletCount--;
}

// This function seems overcomplicated but it's almost identical to kooper first strike test so I know it works
s32 test_enemy_bullet_first_strike() {
    PlayerStatus* playerStatus = &gPlayerStatus;
    f32           xTemp, yTemp, zTemp;
    f32           playerX, playerY, playerZ;
    f32           bulletX, bulletY, bulletZ;
    f32           playerCollHeight;
    f32           bulletCollHeight;
    f32           playerCollRadius;
    f32           bulletCollRadius;
    f32           angleToPlayer;
    f32           distToPlayer;

    Npc* bullet;
    s32  i;

    if (!enemyBulletCount) {
        return FALSE;
    }

    playerX          = playerStatus->position.x;
    playerY          = playerStatus->position.y;
    playerZ          = playerStatus->position.z;
    playerCollHeight = playerStatus->colliderHeight;
    playerCollRadius = playerStatus->colliderDiameter * 0.55;

    for (i = 0; i < ARRAY_COUNT(enemy_bullets); i++) {

        if (enemy_bullets[i].activeBulletIndex == ENEMY_BULLET_EMPTY) {
            continue;
        }

        bullet = get_npc_by_index(enemy_bullets[i].activeBulletIndex);

        bulletX = bullet->pos.x;
        bulletY = bullet->pos.y;
        bulletZ = bullet->pos.z;

        bulletCollHeight = bullet->collisionHeight;
        bulletCollRadius = bullet->collisionDiameter * 0.8;

        angleToPlayer = atan2(playerX, playerZ, bulletX, bulletZ);
        distToPlayer  = dist2D(playerX, playerZ, bulletX, bulletZ);

        xTemp = bullet->pos.x;
        yTemp = bullet->pos.y;
        zTemp = bullet->pos.z;

        if (npc_test_move_taller_with_slipping(
                0, &xTemp, &yTemp, &zTemp, distToPlayer, angleToPlayer, bulletCollHeight,
                bulletCollRadius + playerCollRadius
            ))
        {
            continue;
        }

        if (bulletY > playerY + playerCollHeight) {
            continue;
        }

        if (playerY > bulletY + bulletCollHeight) {
            continue;
        }

        bulletX      = playerX - bulletX;
        bulletZ      = playerZ - bulletZ;
        distToPlayer = SQ(bulletX) + SQ(bulletZ);

        if (!(SQ(bulletCollRadius) + SQ(playerCollRadius) <= distToPlayer)) {
            destroy_enemy_bullet(i);
            return TRUE;
        }
    }
    return FALSE;
}

void enemy_bullet_hit_entity(Npc* bulletNpc, s32 bulletIndex) {
    Entity* entity;

    if (NpcHitQueryColliderID < 0 || !(NpcHitQueryColliderID & COLLISION_WITH_ENTITY_BIT)) {
        bulletNpc->moveSpeed = 0.0f;
        return;
    }

    enemy_bullets[bulletIndex].activeBulletTime = ENEMY_BULLET_DECAY; // Kill bullet on next render
    entity         = get_entity_by_index(NpcHitQueryColliderID & ~COLLISION_WITH_ENTITY_BIT);
    entity->flags |= ENTITY_FLAG_PARTNER_COLLISION;
}

void update_enemy_bullet_collision(Npc* bulletNpc, s32 bulletIndex) {
    f32 posX, posY, posZ;
// check the forward collision for bullet
#define TEST_COLLISION_AT_ANGLE(testAngle)                                                                             \
    (posX = bulletNpc->pos.x, posY = bulletNpc->pos.y, posZ = bulletNpc->pos.z,                                        \
     npc_test_move_taller_with_slipping(                                                                               \
         COLLISION_CHANNEL_8000, &posX, &posY, &posZ, bulletNpc->moveSpeed, testAngle, bulletNpc->collisionHeight,     \
         bulletNpc->collisionDiameter / 2                                                                              \
     ))

    if (TEST_COLLISION_AT_ANGLE(bulletNpc->yaw - 20.0f)) {
        enemy_bullet_hit_entity(bulletNpc, bulletIndex);
    }

    if (TEST_COLLISION_AT_ANGLE(bulletNpc->yaw + 20.0f)) {
        enemy_bullet_hit_entity(bulletNpc, bulletIndex);
    }

    if (TEST_COLLISION_AT_ANGLE(bulletNpc->yaw)) {
        enemy_bullet_hit_entity(bulletNpc, bulletIndex);
    }
}

void bullet_render_straight(Npc* bulletNpc) {
    s32 bulletIndex = get_enemy_bullet_index_by_npc_id(bulletNpc->npcID);
    if (bulletIndex == -1) {
        return;
    }
    if (enemy_bullets[bulletIndex].activeBulletTime > ENEMY_BULLET_DECAY || bulletNpc->pos.x > 340) {
        destroy_enemy_bullet(bulletIndex);
    }
    enemy_bullets[bulletIndex].activeBulletTime++;
    update_enemy_bullet_collision(bulletNpc, bulletIndex);
    npc_move_heading(bulletNpc, bulletNpc->moveSpeed, bulletNpc->yaw);
}

void hammer_jump(Npc* bulletNpc) {
    // new render function below
    s32 bulletIndex = get_enemy_bullet_index_by_npc_id(bulletNpc->npcID);


    npc_move_heading(bulletNpc, bulletNpc->moveSpeed, bulletNpc->yaw);

    bulletNpc->pos.y += bulletNpc->jumpVelocity;
    bulletNpc->jumpVelocity -= bulletNpc->jumpScale;

    bulletNpc->duration--;

    if (bulletNpc->duration < 0) {
        enemy_bullets[bulletIndex].activeBulletTime = ENEMY_BULLET_DECAY;
    }
}

void bullet_render_hammer(Npc* bulletNpc) {
    s32 bulletIndex = get_enemy_bullet_index_by_npc_id(bulletNpc->npcID);
    if (enemy_bullets[bulletIndex].activeBulletTime >= ENEMY_BULLET_DECAY) {
        destroy_enemy_bullet(bulletIndex);
    }
    enemy_bullets[bulletIndex].activeBulletTime++;
    update_enemy_bullet_collision(bulletNpc, bulletIndex);
    hammer_jump(bulletNpc);
    if (enemy_bullets[bulletIndex].facingLeft) {
        bulletNpc->rotation.z += 20.0f;
    } else {
        bulletNpc->rotation.z -= 20.0f;
    }
    bulletNpc->rotation.x = 0;
}

void enemy_bullet_init(Npc* bulletNpc, Npc* enemy, EnemyBulletType type) {
    s32 bulletIndex = get_enemy_bullet_index_by_npc_id(bulletNpc->npcID);
    
    s32 enemyPosX = enemy->pos.x;
    s32 enemyPosY = enemy->pos.y;
    s32 enemyPosZ = enemy->pos.z;

    s32 initialDistanceX = enemy_bullets[bulletIndex].facingLeft ? -25.0f : 25.0f;
    s32 hammerOffset;
    s32 facingLeft;

    bulletNpc->pos.x = enemyPosX + initialDistanceX;
    bulletNpc->pos.y = enemyPosY + 30.0f;
    bulletNpc->pos.z = enemyPosZ;
    bulletNpc->rotation.x           = 0.0f;
    bulletNpc->rotation.z           = 0.0f;
    
    switch (type) {
        case ENEMY_BULLET_TYPE_HAMMER:
            facingLeft = enemy_bullets[bulletIndex].facingLeft = (gPlayerStatus.position.x < bulletNpc->pos.x);
            hammerOffset = facingLeft ? -20.0f : 20.0f;
            bulletNpc->moveToPos.x = gPlayerStatus.position.x + hammerOffset;
            bulletNpc->moveToPos.y = -240;
            bulletNpc->moveToPos.z = gPlayerStatus.position.z;
            bulletNpc->yaw = enemy_bullets[bulletIndex].facingLeft ? 270 : 90;
            bulletNpc->jumpScale = 1.1f;
            bulletNpc->duration   = ENEMY_BULLET_DECAY;
            bulletNpc->scale.x           = 2.5f;
            bulletNpc->scale.y           = 2.5f;
            bulletNpc->scale.z           = 2.5f;
            bulletNpc->flags |= NPC_FLAG_JUMPING;
            bulletNpc->goalY = bulletNpc->moveToPos.y - bulletNpc->pos.y;
            bulletNpc->distToGoal = dist2D(bulletNpc->pos.x, bulletNpc->pos.z, bulletNpc->moveToPos.x, bulletNpc->moveToPos.z);
            bulletNpc->moveSpeed = bulletNpc->distToGoal / bulletNpc->duration;
            bulletNpc->jumpVelocity = (bulletNpc->jumpScale * bulletNpc->duration * 0.5f) + (bulletNpc->goalY / bulletNpc->duration);
            break;
        case ENEMY_BULLET_TYPE_LEFT:
            bulletNpc->moveToPos.x = enemyPosX;
            bulletNpc->moveToPos.y = enemyPosY;
            bulletNpc->moveToPos.z = enemyPosZ;
            bulletNpc->yaw         = enemy->yaw;
            bulletNpc->scale.x           = 0.7f;
            bulletNpc->scale.y           = 0.7f;
            bulletNpc->scale.z           = 0.7f;
            bulletNpc->moveSpeed   = 30.0f;
            break;
    }
}

void use_enemy_bullet(Enemy* enemy, EnemyBulletType type) {
    NpcBlueprint  sp10;
    NpcBlueprint* bp = &sp10;
    Npc*          bulletNpc;
    Npc*          enemyNpc;
    NpcSettings   npcSettings;
    NpcData       npcData;

    f32 enemyPosX, enemyPosY, enemyPosZ, initialDistanceX, moveAngle;

    s32 i;

    if (enemyBulletCount >= MAX_ENEMY_BULLETS) {
        return;
    }

    // find empty bullet index
    for (i = 0; i < ARRAY_COUNT(enemy_bullets); i++) {
        if (enemy_bullets[i].activeBulletIndex == ENEMY_BULLET_EMPTY) {
            break;
        }
    }

    enemyBulletCount++;

    enemyNpc = get_npc_safe(enemy->npcID);

    enemyPosX = enemyNpc->pos.x;
    enemyPosY = enemyNpc->pos.y;
    enemyPosZ = enemyNpc->pos.z;

    npcData.id       = 80 + i;
    npcData.settings = &npcSettings;

    switch (type) {
        case ENEMY_BULLET_TYPE_HAMMER:
            npcSettings.defaultAnim = ANIM_HammerBros_Anim0D;
            npcSettings.height      = 12;
            npcSettings.radius      = 32;
            bp->onRender    = bullet_render_hammer;
            break;
        case ENEMY_BULLET_TYPE_LEFT:
            npcSettings.defaultAnim = ANIM_Fire_Brighest_Burn;
            npcSettings.height      = 16;
            npcSettings.radius      = 32;
            bp->onRender    = bullet_render_straight;
            break;
        case ENEMY_BULLET_TYPE_RIGHT:
            npcSettings.defaultAnim = ANIM_Fire_Brighest_Burn;
            npcSettings.height      = 16;
            npcSettings.radius      = 32;
            bp->onRender    = bullet_render_straight;
            break;
    }

    // create the new NPC
    bp->flags       = 0;
    bp->initialAnim = npcSettings.defaultAnim;
    bp->onUpdate    = NULL;

    enemy_bullets[i].activeBulletTime  = 0;
    enemy_bullets[i].activeBulletIndex = create_basic_npc(bp);

    if (!(enemyNpc->yaw >= 90.0f) || !(enemyNpc->yaw < 270)) {
        enemy_bullets[i].facingLeft        = TRUE;
    } else {
        enemy_bullets[i].facingLeft        = FALSE;
    }
    enemy_bullets[i].npcID             = npcData.id;

    bulletNpc = get_npc_by_index(enemy_bullets[i].activeBulletIndex);
    disable_npc_shadow(bulletNpc);

    bulletNpc->npcID             = npcData.id;
    bulletNpc->collisionDiameter = npcSettings.radius;
    bulletNpc->collisionHeight   = npcSettings.height;
    bulletNpc->unk_96            = 0;
    bulletNpc->planarFlyDist     = 0.0f;
    bulletNpc->flags             = NPC_FLAG_IGNORE_PLAYER_COLLISION | NPC_FLAG_IGNORE_WORLD_COLLISION | NPC_FLAG_8;

    enemy_bullet_init(bulletNpc, enemyNpc, type);


}



void create_enemy_bullet() {
    
}

void clear_enemy_bullets() {
    s32 i;
    for (i = 0; i < ARRAY_COUNT(enemy_bullets); i++)
    {
        enemy_bullets[i].npcID = 0;
        enemy_bullets[i].activeBulletIndex = ENEMY_BULLET_EMPTY;
    }

    enemyBulletCount = 0;
}

