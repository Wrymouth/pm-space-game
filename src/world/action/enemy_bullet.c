#include "common.h"
#include "variables.h"
#include "npc.h"
#include "world/partners.h"
#include "sprite/npc/Fire.h"
#include "sprite/npc/HammerBros.h"
#include "sprite/npc/KoopaBros.h"
#include "sprite/npc/ParadeTwink.h"
#include "sprite/npc/ParadeShyGuy.h"
#include "sprite/npc/ParadeWizard.h"
#include "sprite/player.h"
#include "world/action/enemy_bullet.h"

#define ENEMY_BULLET_EMPTY -1

#define CHECK_FLAG(var, flag) (var & flag)
#define SET_FLAG(var, flag) (var |= flag)
#define UNSET_FLAG(var, flag) (var &= ~flag)

typedef struct EnemyBulletStatus {
    s32 npcID;
    s32 activeBulletIndex;
    s32 activeBulletTime;
    s32 flags;
    EnemyBulletType bulletType;
} EnemyBulletStatus;

const s32 MAX_ENEMY_BULLETS  = 10;
const s32 ENEMY_BULLET_DECAY = 50;
s32       enemyBulletCount   = 0;

EnemyBulletStatus enemy_bullets[] = { { 0, ENEMY_BULLET_EMPTY }, { 0, ENEMY_BULLET_EMPTY }, { 0, ENEMY_BULLET_EMPTY },
                                      { 0, ENEMY_BULLET_EMPTY }, { 0, ENEMY_BULLET_EMPTY }, { 0, ENEMY_BULLET_EMPTY },
                                      { 0, ENEMY_BULLET_EMPTY }, { 0, ENEMY_BULLET_EMPTY }, { 0, ENEMY_BULLET_EMPTY },
                                      { 0, ENEMY_BULLET_EMPTY } };

void npc_move_angle_speed(Npc* npc, s32 moveSpeed, f32 angle) {
    s32 distX = cos_deg(angle) * moveSpeed;
    s32 distY = sin_deg(angle) * moveSpeed;

    npc->pos.x += distX;
    npc->pos.y += distY;
}

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
    if (enemy_bullets[bulletIndex].activeBulletIndex != ENEMY_BULLET_EMPTY) {
        free_npc_by_index(enemy_bullets[bulletIndex].activeBulletIndex);
    }
    enemy_bullets[bulletIndex].npcID = 0;
    enemy_bullets[bulletIndex].activeBulletIndex = ENEMY_BULLET_EMPTY;
    enemy_bullets[bulletIndex].flags = 0;
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
            switch (enemy_bullets[i].bulletType) {
                case ENEMY_BULLET_TYPE_KOOPA_UL:
                case ENEMY_BULLET_TYPE_KOOPA_UR:
                case ENEMY_BULLET_TYPE_KOOPA_DL:
                case ENEMY_BULLET_TYPE_KOOPA_DR:
                    return TRUE;
            }
            destroy_enemy_bullet(i);
            return TRUE;
        }
    }
    return FALSE;
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
    npc_move_heading(bulletNpc, bulletNpc->moveSpeed, bulletNpc->yaw);
}

void hammer_jump(Npc* bulletNpc) {
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
    hammer_jump(bulletNpc);
    if (enemy_bullets[bulletIndex].flags & ENEMY_BULLET_FLAG_FACING_LEFT) {
        bulletNpc->rotation.z += 20.0f;
    } else {
        bulletNpc->rotation.z -= 20.0f;
    }
    bulletNpc->rotation.x = 0;
}

void bullet_render_koopa(Npc* bulletNpc) {
    s32 bulletIndex = get_enemy_bullet_index_by_npc_id(bulletNpc->npcID);
    if (bulletNpc->pos.x > 300 || bulletNpc->pos.x < -300) {
        bulletNpc->moveToPos.x *= -1;
    }
    if (bulletNpc->pos.y > 150 || bulletNpc->pos.y < -200) {
        bulletNpc->moveToPos.y *= -1;
    }
    bulletNpc->pos.x += bulletNpc->moveToPos.x;
    bulletNpc->pos.y += bulletNpc->moveToPos.y;
}

void bullet_render_water(Npc* bulletNpc) {
    s32 bulletIndex = get_enemy_bullet_index_by_npc_id(bulletNpc->npcID);
    EnemyBulletStatus* bulletStatus = &enemy_bullets[bulletIndex];
    if (CHECK_FLAG(bulletStatus->flags, ENEMY_BULLET_FLAG_WATER_UP)) {
        bulletNpc->pos.y += bulletNpc->moveSpeed;
        if (bulletStatus->activeBulletTime > 30) {
            bulletNpc->moveSpeed = 7;
            bulletNpc->pos.x = rand_int(500) - 250;
            bulletNpc->moveToPos.x = bulletNpc->pos.x; // used for base position in the down phase
            bulletNpc->pos.y = 180;
            bulletNpc->rotation.z = 0;
            UNSET_FLAG(bulletStatus->flags, ENEMY_BULLET_FLAG_WATER_UP);
            SET_FLAG(bulletStatus->flags, ENEMY_BULLET_FLAG_WATER_DOWN);
        }
    } else if (CHECK_FLAG(bulletStatus->flags, ENEMY_BULLET_FLAG_WATER_DOWN)) {
        bulletNpc->pos.y -= bulletNpc->moveSpeed;
        bulletNpc->pos.x = 100 * sin_deg(bulletNpc->pos.y * 2) + bulletNpc->moveToPos.x;
        if (bulletStatus->activeBulletTime > 180) {
            destroy_enemy_bullet(bulletIndex);
        }
    } else {
        SET_FLAG(enemy_bullets[bulletIndex].flags, ENEMY_BULLET_FLAG_WATER_UP);
    }
    bulletStatus->activeBulletTime++;
}

void bullet_render_egg(Npc* bulletNpc) {
    s32 bulletIndex = get_enemy_bullet_index_by_npc_id(bulletNpc->npcID);
    if (enemy_bullets[bulletIndex].activeBulletTime >= ENEMY_BULLET_DECAY) {
        destroy_enemy_bullet(bulletIndex);
    }
    bulletNpc->rotation.z += 20.0f;
    enemy_bullets[bulletIndex].activeBulletTime++;
    npc_move_angle_speed(bulletNpc, bulletNpc->moveSpeed, bulletNpc->moveAngle);
}

void enemy_bullet_init(Npc* bulletNpc, Npc* enemy, EnemyBulletType type) {
    s32 bulletIndex = get_enemy_bullet_index_by_npc_id(bulletNpc->npcID);
    
    s32 enemyPosX = enemy->pos.x;
    s32 enemyPosY = enemy->pos.y;
    s32 enemyPosZ = enemy->pos.z;

    s32 initialDistanceX = (enemy_bullets[bulletIndex].flags & ENEMY_BULLET_FLAG_FACING_LEFT) ? -25.0f : 25.0f;
    s32 hammerOffset;
    s32 facingLeft = FALSE;

    bulletNpc->pos.x = enemyPosX + initialDistanceX;
    bulletNpc->pos.y = enemyPosY + 30.0f;
    bulletNpc->pos.z = enemyPosZ;
    bulletNpc->rotation.x           = 0.0f;
    bulletNpc->rotation.z           = 0.0f;
    
    switch (type) {
        case ENEMY_BULLET_TYPE_HAMMER:
            if (gPlayerStatus.position.x < bulletNpc->pos.x) {
                enemy_bullets[bulletIndex].flags |= ENEMY_BULLET_FLAG_FACING_LEFT;
                facingLeft = TRUE;
            }
            hammerOffset = facingLeft ? -20.0f : 20.0f;
            bulletNpc->moveToPos.x = gPlayerStatus.position.x + hammerOffset;
            bulletNpc->moveToPos.y = -240;
            bulletNpc->moveToPos.z = gPlayerStatus.position.z;
            bulletNpc->yaw = facingLeft ? 270 : 90;
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
        case ENEMY_BULLET_TYPE_RIGHT:
            bulletNpc->moveToPos.x = enemyPosX;
            bulletNpc->moveToPos.y = enemyPosY;
            bulletNpc->moveToPos.z = enemyPosZ;
            bulletNpc->yaw         = clamp_angle(enemy->yaw - 180);
            bulletNpc->scale.x           = 0.7f;
            bulletNpc->scale.y           = 0.7f;
            bulletNpc->scale.z           = 0.7f;
            bulletNpc->moveSpeed   = 30.0f;
            break;
        case ENEMY_BULLET_TYPE_KOOPA_UL:
            // used for axis speed
            bulletNpc->moveToPos.x = -10.0f;
            bulletNpc->moveToPos.y = 10.0f;
            break;
        case ENEMY_BULLET_TYPE_KOOPA_UR:
            bulletNpc->moveToPos.x = 10.0f;
            bulletNpc->moveToPos.y = 10.0f;
            break;
        case ENEMY_BULLET_TYPE_KOOPA_DL:
            bulletNpc->moveToPos.x = -10.0f;
            bulletNpc->moveToPos.y = -10.0f;
            break;
        case ENEMY_BULLET_TYPE_KOOPA_DR:
            bulletNpc->moveToPos.x = 10.0f;
            bulletNpc->moveToPos.y = -10.0f;
            break;
        case ENEMY_BULLET_TYPE_EGG_DOWN:
            bulletNpc->moveSpeed = 13;
            bulletNpc->moveAngle = 210;
            bulletNpc->scale.x   = 2.0f;
            bulletNpc->scale.y   = 2.0f;
            bulletNpc->scale.z   = 2.0f;
            break;
        case ENEMY_BULLET_TYPE_EGG_MID:
            bulletNpc->moveSpeed = 13;
            bulletNpc->moveAngle = 180;
            bulletNpc->scale.x   = 2.0f;
            bulletNpc->scale.y   = 2.0f;
            bulletNpc->scale.z   = 2.0f;
            break;
        case ENEMY_BULLET_TYPE_EGG_UP:
            bulletNpc->moveSpeed = 13;
            bulletNpc->moveAngle = 150;
            bulletNpc->scale.x   = 2.0f;
            bulletNpc->scale.y   = 2.0f;
            bulletNpc->scale.z   = 2.0f;
            break;
        case ENEMY_BULLET_TYPE_WATER:
            bulletNpc->moveSpeed = 13; 
            bulletNpc->rotation.z = 180;
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
            enemy_bullets[i].flags |= ENEMY_BULLET_FLAG_FACING_LEFT;
            npcSettings.defaultAnim = ANIM_ParadeShyGuy_Groove_GrooveStill;
            npcSettings.height      = 16;
            npcSettings.radius      = 32;
            bp->onRender    = bullet_render_straight;
            break;
        case ENEMY_BULLET_TYPE_RIGHT:
            enemy_bullets[i].flags &= ~ENEMY_BULLET_FLAG_FACING_LEFT;
            npcSettings.defaultAnim = ANIM_ParadeShyGuy_Groove_GrooveStill;
            npcSettings.height      = 16;
            npcSettings.radius      = 32;
            bp->onRender    = bullet_render_straight;
            break;
        case ENEMY_BULLET_TYPE_KOOPA_UL:
            npcSettings.defaultAnim = ANIM_KoopaBros_Green_ShellSpin;
            npcSettings.height      = 16;
            npcSettings.radius      = 16;
            bp->onRender    = bullet_render_koopa;
            break;
        case ENEMY_BULLET_TYPE_KOOPA_UR:
            npcSettings.defaultAnim = ANIM_KoopaBros_Yellow_ShellSpin;
            npcSettings.height      = 16;
            npcSettings.radius      = 16;
            bp->onRender    = bullet_render_koopa;
            break;
        case ENEMY_BULLET_TYPE_KOOPA_DL:
            npcSettings.defaultAnim = ANIM_KoopaBros_Black_ShellSpin;
            npcSettings.height      = 16;
            npcSettings.radius      = 16;
            bp->onRender    = bullet_render_koopa;
            break;
        case ENEMY_BULLET_TYPE_KOOPA_DR:
            npcSettings.defaultAnim = ANIM_KoopaBros_Red_ShellSpin;
            npcSettings.height      = 16;
            npcSettings.radius      = 16;
            bp->onRender    = bullet_render_koopa;
            break;
        case ENEMY_BULLET_TYPE_WATER:
            npcSettings.defaultAnim = ANIM_ParadeWizard_Merle_MerleGather;
            npcSettings.height      = 16;
            npcSettings.radius      = 16;
            bp->onRender    = bullet_render_water;
            break;
        case ENEMY_BULLET_TYPE_EGG_DOWN:
        case ENEMY_BULLET_TYPE_EGG_UP:
        case ENEMY_BULLET_TYPE_EGG_MID:
            npcSettings.defaultAnim = ANIM_ParadeTwink_Still;
            npcSettings.height      = 32;
            npcSettings.radius      = 32;
            bp->onRender    = bullet_render_egg;
            break;
    }

    // create the new NPC
    bp->flags       = 0;
    bp->initialAnim = npcSettings.defaultAnim;
    bp->onUpdate    = NULL;

    enemy_bullets[i].activeBulletTime  = 0;
    enemy_bullets[i].activeBulletIndex = create_basic_npc(bp);
    enemy_bullets[i].bulletType = type;

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

void do_attack(Enemy* enemy, EnemyAttackType type) {
    switch (type) {
        case ENEMY_ATTACK_TYPE_HAMMER:
            use_enemy_bullet(enemy, ENEMY_BULLET_TYPE_HAMMER);
            break;
        case ENEMY_ATTACK_TYPE_LEFT:
            use_enemy_bullet(enemy, ENEMY_BULLET_TYPE_LEFT);
            break;
        case ENEMY_ATTACK_TYPE_KOOPA_SHELLS:
            use_enemy_bullet(enemy, ENEMY_BULLET_TYPE_KOOPA_UL);
            use_enemy_bullet(enemy, ENEMY_BULLET_TYPE_KOOPA_UR);
            use_enemy_bullet(enemy, ENEMY_BULLET_TYPE_KOOPA_DL);
            use_enemy_bullet(enemy, ENEMY_BULLET_TYPE_KOOPA_DR);
            break;
        case ENEMY_ATTACK_TYPE_SPLIT:
            use_enemy_bullet(enemy, ENEMY_BULLET_TYPE_LEFT);
            use_enemy_bullet(enemy, ENEMY_BULLET_TYPE_RIGHT);
            break;
        case ENEMY_ATTACK_TYPE_WATER:
            use_enemy_bullet(enemy, ENEMY_BULLET_TYPE_WATER);
            break;
        case ENEMY_ATTACK_TYPE_EGGS:
            use_enemy_bullet(enemy, ENEMY_BULLET_TYPE_EGG_DOWN);
            use_enemy_bullet(enemy, ENEMY_BULLET_TYPE_EGG_UP);
            use_enemy_bullet(enemy, ENEMY_BULLET_TYPE_EGG_MID);
            break;
        default:
            break;
    }
    
}

void clear_enemy_bullets(void) {
    s32 i;
    for (i = 0; i < ARRAY_COUNT(enemy_bullets); i++) {
        destroy_enemy_bullet(i);
    }
    sfx_play_sound(SOUND_PERIL);
    enemyBulletCount = 0;
}

API_CALLABLE(ClearAllEnemyBullets) {
    clear_enemy_bullets();
    return ApiStatus_DONE2;
}
