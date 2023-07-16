#include "common.h"
#include "variables.h"
#include "damage_system.h"
#include "sprite/player.h"
#include "bullet.h"
#include "enemy_bullet.h"

const s32 PLAYER_INV_FRAMES = 30;

void goto_gameover(void) {
    char* map = "gv_01";

    s16 currentMapID = gGameStatusPtr->mapID;
    s16 currentAreaID = gGameStatusPtr->areaID;

    s16 destAreaID;
    s16 destMapID;
    
    get_map_IDs_by_name(map, &destAreaID, &destMapID);
    
    // Go there
    
    gGameStatusPtr->areaID = destAreaID;
    gGameStatusPtr->mapID = destMapID;
    gGameStatusPtr->entryID = currentMapID;
    set_map_transition_effect(0);
    set_game_mode(GAME_MODE_UNUSED);

    // Stop ambient sounds
    play_ambient_sounds(AMBIENT_SILENCE, 1);
    au_stop_songs();
}

void player_take_damage(s32 amount) {
    if (!(gPlayerStatus.flags & PS_FLAG_HAZARD_INVINCIBILITY) && !(evt_get_variable(NULL, MapFlag(0)))) {
        sfx_play_sound_at_player(0x158, SOUND_SPACE_MODE_0);
        gPlayerData.curHP -= amount;
        gPlayerStatus.flags |= PS_FLAG_HAZARD_INVINCIBILITY;
        gPlayerStatus.invFrames = 0;
        if (gPlayerData.curHP <= 0) {
            evt_set_variable(NULL, AreaFlag(0), TRUE);
        }
    }
}

void enemy_take_damage(Enemy* enemy, s32 amount) {
    s16 lostHP = 1024 / enemy->maxHP;
    if (!(enemy->flags & ENEMY_FLAG_INVINCIBLE) && !(evt_get_variable(NULL, AreaFlag(0)))) {
        sfx_play_sound_at_npc(0xE1, SOUND_SPACE_MODE_0, enemy->npcID);
        enemy->curHP -= amount;
        enemy->flags |= ENEMY_FLAG_INVINCIBLE;
        enemy->invFrames = 0;
        gPlayerData.specialBarsFilled -= (lostHP * amount);
    }
}

void game_over_cleanup() {
    clear_bullets();
    clear_enemy_bullets();
}

