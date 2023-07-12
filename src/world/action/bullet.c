#include "common.h"
#include "variables.h"
#include "npc.h"
#include "world/partners.h"
#include "sprite/npc/Leaf.h"
#include "sprite/player.h"
#include "world/action/bullet.h"
#define BULLET_EMPTY -1

const s32 MAX_BULLETS  = 3;
const s32 BULLET_DECAY = 50;

typedef struct BulletStatus {
    s32 npcID;
    s32 activeBulletIndex;
    s32 activeBulletTime;
    s8  facingLeft;
} BulletStatus;

BulletStatus bullets[] = { { 0, BULLET_EMPTY }, { 0, BULLET_EMPTY }, { 0, BULLET_EMPTY } };


s32       bulletCount  = 0;
f32 bulletSpeed = 30.0f;

s32 get_bullet_index_by_npc_id(s32 npcId) {
    s32 i;
    for (i = 0; i < ARRAY_COUNT(bullets); i++) {
        if (npcId == bullets[i].npcID) {
            return i;
        }
    }
    return -1;
}

// This function seems overcomplicated but it's almost identical to kooper first strike test so I know it works
s32 test_bullet_first_strike(Npc* enemy) {
    f32 xTemp, yTemp, zTemp;
    f32 enemyX, enemyY, enemyZ;
    f32 bulletX, bulletY, bulletZ;
    f32 enemyCollHeight;
    f32 bulletCollHeight;
    f32 enemyCollRadius;
    f32 bulletCollRadius;
    f32 angleToEnemy;
    f32 distToEnemy;

    Npc* bullet;
    s32  i;

    if (!bulletCount) {
        return FALSE;
    }

    enemyX          = enemy->pos.x;
    enemyY          = enemy->pos.y;
    enemyZ          = enemy->pos.z;
    enemyCollHeight = enemy->collisionHeight;
    enemyCollRadius = enemy->collisionDiameter * 0.55;

    for (i = 0; i < ARRAY_COUNT(bullets); i++) {

        if (bullets[i].activeBulletIndex == BULLET_EMPTY) {
            continue;
        }

        bullet = get_npc_by_index(bullets[i].activeBulletIndex);

        bulletX = bullet->pos.x;
        bulletY = bullet->pos.y;
        bulletZ = bullet->pos.z;

        bulletCollHeight = bullet->collisionHeight;
        bulletCollHeight = bullet->collisionDiameter * 0.8;

        angleToEnemy = atan2(enemyX, enemyZ, bulletX, bulletZ);
        distToEnemy  = dist2D(enemyX, enemyZ, bulletX, bulletZ);

        xTemp = bullet->pos.x;
        yTemp = bullet->pos.y;
        zTemp = bullet->pos.z;

        if (npc_test_move_taller_with_slipping(
                0, &xTemp, &yTemp, &zTemp, distToEnemy, angleToEnemy, bulletCollHeight,
                bulletCollRadius + enemyCollRadius
            ))
        {
            continue;
        }

        if (bulletY > enemyY + enemyCollHeight) {
            continue;
        }

        if (enemyY > bulletY + bulletCollHeight) {
            continue;
        }

        bulletX     = enemyX - bulletX;
        bulletZ     = enemyZ - bulletZ;
        distToEnemy = SQ(bulletX) + SQ(bulletZ);

        if (!(SQ(bulletCollRadius) + SQ(enemyCollRadius) <= distToEnemy)) {
            free_npc_by_index(bullets[i].activeBulletIndex);
            bullets[i].activeBulletIndex = BULLET_EMPTY;
            bulletCount--;
            return TRUE;
        }
    }
    return FALSE;
}

void bullet_hit_entity(Npc* bulletNpc, s32 bulletIndex) {
    Entity* entity;

    if (NpcHitQueryColliderID < 0 || !(NpcHitQueryColliderID & COLLISION_WITH_ENTITY_BIT)) {
        bulletNpc->moveSpeed = 0.0f;
        return;
    }

    bullets[bulletIndex].activeBulletTime  = BULLET_DECAY; // Kill bullet on next render
    entity                                 = get_entity_by_index(NpcHitQueryColliderID & ~COLLISION_WITH_ENTITY_BIT);
    entity->flags                         |= ENTITY_FLAG_PARTNER_COLLISION;
}

void update_collision(Npc* bulletNpc, s32 bulletIndex) {
    f32 posX, posY, posZ;
// check the forward collision for bullet
#define TEST_COLLISION_AT_ANGLE(testAngle)                                                                             \
    (posX = bulletNpc->pos.x, posY = bulletNpc->pos.y, posZ = bulletNpc->pos.z,                                        \
     npc_test_move_taller_with_slipping(                                                                               \
         COLLISION_CHANNEL_8000, &posX, &posY, &posZ, bulletNpc->moveSpeed, testAngle, bulletNpc->collisionHeight,     \
         bulletNpc->collisionDiameter / 2                                                                              \
     ))

    if (TEST_COLLISION_AT_ANGLE(bulletNpc->yaw - 20.0f)) {
        bullet_hit_entity(bulletNpc, bulletIndex);
    }

    if (TEST_COLLISION_AT_ANGLE(bulletNpc->yaw + 20.0f)) {
        bullet_hit_entity(bulletNpc, bulletIndex);
    }

    if (TEST_COLLISION_AT_ANGLE(bulletNpc->yaw)) {
        bullet_hit_entity(bulletNpc, bulletIndex);
    }
}

void on_bullet_render(Npc* bulletNpc) {
    s32 bulletIndex = get_bullet_index_by_npc_id(bulletNpc->npcID);
    if (bulletIndex == -1) {
        return;
    }
    if (bullets[bulletIndex].activeBulletTime > BULLET_DECAY || bulletNpc->pos.x > 340) {
        free_npc_by_index(bullets[bulletIndex].activeBulletIndex);
        bullets[bulletIndex].activeBulletIndex = BULLET_EMPTY;
        bulletCount--;
    }
    bullets[bulletIndex].activeBulletTime++;
    update_collision(bulletNpc, bulletIndex);
    npc_move_heading(bulletNpc, bulletNpc->moveSpeed, bulletNpc->yaw);
}

void use_bullet(void) {
    NpcBlueprint  sp10;
    NpcBlueprint* bp = &sp10;
    Npc*          bulletNpc;
    NpcSettings   npcSettings;
    NpcData       npcData;

    PlayerStatus* playerStatus = &gPlayerStatus;

    f32 playerPosX, playerPosY, playerPosZ, initialDistanceX, moveAngle;

    s32 i;

    if (bulletCount >= MAX_BULLETS) {
        return;
    }

    // find empty bullet index
    for (i = 0; i < ARRAY_COUNT(bullets); i++) {
        if (bullets[i].activeBulletIndex == -1) {
            break;
        }
    }

    bulletCount++;

    playerPosX = playerStatus->position.x;
    playerPosY = playerStatus->position.y;
    playerPosZ = playerStatus->position.z;

    npcSettings.defaultAnim = ANIM_Leaf_Pivot;
    npcSettings.height      = 12;
    npcSettings.radius      = 32;

    npcData.id       = 69 + i;
    npcData.init     = (void*)0x00004003;
    npcData.settings = &npcSettings;
    npcData.flags = ENEMY_FLAG_PASSIVE;

    // create the new NPC
    bp->flags       = NPC_FLAG_IGNORE_WORLD_COLLISION;
    bp->initialAnim = npcSettings.defaultAnim;
    bp->onRender    = on_bullet_render;
    bp->onUpdate    = NULL;

    bullets[i].activeBulletTime  = 0;
    bullets[i].activeBulletIndex = create_basic_npc(bp);
    bullets[i].facingLeft        = FALSE;
    bullets[i].npcID             = npcData.id;

    bulletNpc = get_npc_by_index(bullets[i].activeBulletIndex);
    disable_npc_shadow(bulletNpc);
    bulletNpc->scale.x           = 0.7f;
    bulletNpc->scale.y           = 0.7f;
    bulletNpc->scale.x           = 0.7f;
    bulletNpc->npcID             = npcData.id;
    bulletNpc->collisionDiameter = npcSettings.radius;
    bulletNpc->collisionHeight   = npcSettings.height;
    bulletNpc->unk_96            = 0;
    bulletNpc->planarFlyDist     = 0.0f;
    bulletNpc->flags             = NPC_FLAG_IGNORE_PLAYER_COLLISION | NPC_FLAG_IGNORE_WORLD_COLLISION | NPC_FLAG_8;

    initialDistanceX = bullets[i].facingLeft ? -25.0f : 25.0f;

    bulletNpc->moveToPos.x = playerStatus->position.x;
    bulletNpc->moveToPos.y = playerStatus->position.y;
    bulletNpc->moveToPos.z = playerStatus->position.z;
    bulletNpc->moveSpeed   = bulletSpeed;
    bulletNpc->yaw         = 90.0f;

    bulletNpc->pos.x = playerPosX + initialDistanceX;
    bulletNpc->pos.y = playerPosY + 10.0f;
    bulletNpc->pos.z = playerPosZ;

    bulletNpc->rotation.z           = 0.0f;
    bulletNpc->rotationPivotOffsetY = 10.0f;

    sfx_play_sound_at_player(0x17, SOUND_SPACE_MODE_0);
}

void clear_bullets() {
    s32 i;
    for (i = 0; i < ARRAY_COUNT(bullets); i++)
    {
        bullets[i].npcID = 0;
        bullets[i].activeBulletIndex = BULLET_EMPTY;
    }
    bulletCount = 0;
}
