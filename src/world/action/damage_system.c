#include "common.h"
#include "variables.h"
#include "damage_system.h"
#include "sprite/player.h"
#include "bullet.h"
#include "enemy_bullet.h"

const s32 PLAYER_INV_FRAMES = 30;

void goto_gameover(void) {
    char* map = "gv_01";
    // Get area/map id pair
    s16 areaID;
    s16 mapID;

    get_map_IDs_by_name(map, &areaID, &mapID);
    
    // Go there
    gGameStatusPtr->areaID = areaID;
    gGameStatusPtr->mapID = mapID;
    gGameStatusPtr->entryID = 0;
    set_map_transition_effect(0);
    set_game_mode(GAME_MODE_UNUSED);

    // Stop ambient sounds
    play_ambient_sounds(AMBIENT_SILENCE, 1);
    au_stop_songs();
}

void player_take_damage(s32 amount) {
    if (!(gPlayerStatus.flags & PS_FLAG_HAZARD_INVINCIBILITY)) {
        sfx_play_sound(0x158);
        gPlayerData.curHP -= amount;
        gPlayerStatus.flags |= PS_FLAG_HAZARD_INVINCIBILITY;
        gPlayerStatus.invFrames = 0;
        if (gPlayerData.curHP <= 0) {
            goto_gameover();
        }
    }
}

void enemy_take_damage(Enemy* enemy, s32 amount) {
    if (!(enemy->flags & ENEMY_FLAG_INVINCIBLE)) {
        enemy->curHP -= amount;
        enemy->flags |= ENEMY_FLAG_INVINCIBLE;
        enemy->invFrames = 0;
    }
}

void game_over_cleanup() {
    clear_enemy_bullets();
    clear_bullets();
}

