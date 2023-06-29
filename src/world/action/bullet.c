#include "common.h"
#include "variables.h"
#include "npc.h"
#include "world/partners.h"
#include "sprite/npc/Fire.h"
#include "sprite/player.h"
#include "world/action/bullet.h"

typedef struct BulletStatus{
    s32 activeBulletIndex;
    s32 activeBulletTime;
    s8 facingLeft;
} BulletStatus;

BulletStatus bulletStatus = {0,0};

const s32 BULLET_DECAY = 30;

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

    if (!bulletStatus.activeBulletIndex) {
        return FALSE;
    }

    bullet = get_npc_by_index(bulletStatus.activeBulletIndex);

    enemyX = enemy->pos.x;
    enemyY = enemy->pos.y;
    enemyZ = enemy->pos.z;

    bulletX = bullet->pos.x;
    bulletY = bullet->pos.y;
    bulletZ = bullet->pos.z;

    enemyCollHeight = enemy->collisionHeight;
    enemyCollRadius = enemy->collisionDiameter * 0.55;

    bulletCollHeight = bullet->collisionHeight;
    bulletCollHeight = bullet->collisionDiameter * 0.8;

    angleToEnemy = atan2(enemyX, enemyZ, bulletX, bulletZ);
    distToEnemy = dist2D(enemyX, enemyZ, bulletX, bulletZ);

    xTemp = bullet->pos.x;
    yTemp = bullet->pos.y;
    zTemp = bullet->pos.z;

    if (npc_test_move_taller_with_slipping(0, &xTemp, &yTemp, &zTemp, distToEnemy, angleToEnemy,
        bulletCollHeight, bulletCollRadius + enemyCollRadius)
    ) {
        return FALSE;
    }

    if (bulletY > enemyY + enemyCollHeight) {
        return FALSE;
    }

    if (enemyY > bulletY + bulletCollHeight) {
        return FALSE;
    }

    bulletX = enemyX - bulletX;
    bulletZ = enemyZ - bulletZ;
    distToEnemy = SQ(bulletX) + SQ(bulletZ);

    if (!(SQ(bulletCollRadius) + SQ(enemyCollRadius) <= distToEnemy)) {
        free_npc_by_index(bulletStatus.activeBulletIndex);
        bulletStatus.activeBulletIndex = NULL;
        return TRUE;
    }
    return FALSE;
}

void bullet_hit_entity(void) {
    Entity* entity;
    Npc* bulletNpc = get_npc_by_index(bulletStatus.activeBulletIndex);

    if (NpcHitQueryColliderID < 0 || !(NpcHitQueryColliderID & COLLISION_WITH_ENTITY_BIT)) {
        bulletNpc->moveSpeed = 0.0f;
        return;
    }

    bulletStatus.activeBulletTime = BULLET_DECAY; // Kill bullet on next render
    entity = get_entity_by_index(NpcHitQueryColliderID & ~COLLISION_WITH_ENTITY_BIT);
    entity->flags |= ENTITY_FLAG_PARTNER_COLLISION;
    
}

void update_collision(Npc* bulletNpc) {
    f32 posX, posY, posZ;
    // check the forward collision for bullet
    #define TEST_COLLISION_AT_ANGLE(testAngle) \
        ( \
        posX = bulletNpc->pos.x, \
        posY = bulletNpc->pos.y, \
        posZ = bulletNpc->pos.z, \
        npc_test_move_taller_with_slipping(COLLISION_CHANNEL_8000, \
            &posX, &posY, &posZ, bulletNpc->moveSpeed, testAngle,  \
            bulletNpc->collisionHeight, bulletNpc->collisionDiameter / 2) \
        )

    if (TEST_COLLISION_AT_ANGLE(bulletNpc->yaw - 20.0f)) {
        bullet_hit_entity();
    }

    if (TEST_COLLISION_AT_ANGLE(bulletNpc->yaw + 20.0f)) {
        bullet_hit_entity();
    }

    if (TEST_COLLISION_AT_ANGLE(bulletNpc->yaw)) {
        bullet_hit_entity();
    }

}

void on_bullet_render(Npc* bulletNpc) {
    if(bulletStatus.activeBulletTime < BULLET_DECAY) {
        bulletStatus.activeBulletTime++;
        update_collision(bulletNpc);
        npc_move_heading(bulletNpc, bulletNpc->moveSpeed, bulletNpc->yaw);
    }
    else {
        free_npc_by_index(bulletStatus.activeBulletIndex);
        bulletStatus.activeBulletIndex = NULL;

    }
}

void use_bullet(void){
    NpcBlueprint sp10;
    NpcBlueprint* bp = &sp10;
    Npc* bulletNpc;
    NpcSettings npcSettings;
    NpcData npcData;

    PlayerStatus* playerStatus = &gPlayerStatus;

    f32 playerPosX, playerPosY, playerPosZ, initialDistanceX, moveAngle;

    if (bulletStatus.activeBulletIndex) {
        return;
    }

    playerPosX = playerStatus->position.x;
    playerPosY = playerStatus->position.y;
    playerPosZ = playerStatus->position.z;

    npcSettings.defaultAnim = ANIM_Fire_Brighest_Burn;
    npcSettings.height = 12;
    npcSettings.radius = 32;

    npcData.id = 69;
    npcData.init = (void*) 0x00004003;
    npcData.settings = &npcSettings;
    //npcData.animations = {};

     // create the new NPC
    bp->flags = 0;
    bp->initialAnim = npcSettings.defaultAnim;
    bp->onRender = on_bullet_render;
    bp->onUpdate = NULL;

    bulletStatus.activeBulletTime = 0;
    bulletStatus.activeBulletIndex = create_basic_npc(bp);
    bulletStatus.facingLeft = partner_force_player_flip_done();

    bulletNpc = get_npc_by_index(bulletStatus.activeBulletIndex);
    disable_npc_shadow(bulletNpc);
    bulletNpc->scale.x = 0.7f;
    bulletNpc->scale.y = 0.7f;
    bulletNpc->scale.x = 0.7f;
    bulletNpc->npcID = npcData.id;
    bulletNpc->collisionDiameter = npcSettings.radius;
    bulletNpc->collisionHeight = npcSettings.height;
    bulletNpc->unk_96 = 0;
    bulletNpc->planarFlyDist = 0.0f;
    bulletNpc->flags = NPC_FLAG_IGNORE_PLAYER_COLLISION | NPC_FLAG_IGNORE_WORLD_COLLISION | NPC_FLAG_8;

    initialDistanceX = bulletStatus.facingLeft ? -25.0f : 25.0f;

    bulletNpc->moveToPos.x = playerStatus->position.x;
    bulletNpc->moveToPos.y = playerStatus->position.y;
    bulletNpc->moveToPos.z = playerStatus->position.z;
    bulletNpc->moveSpeed = 10.0f;
    bulletNpc->yaw = playerStatus->targetYaw;

    bulletNpc->pos.x = playerPosX + initialDistanceX;
    bulletNpc->pos.y = playerPosY + 10.0f;
    bulletNpc->pos.z = playerPosZ;

    bulletNpc->rotation.z = 0.0f;
    bulletNpc->rotationPivotOffsetY = 10.0f;

    sfx_play_sound_at_player(0x17, SOUND_SPACE_MODE_0);
}


