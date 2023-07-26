#ifndef DAMAGE_SYSTEM_H
#define DAMAGE_SYSTEM_H

extern s32 PLAYER_INV_FRAMES;

void player_take_damage(s32 amount);
void enemy_take_damage(Enemy* enemy, s32 amount);
void goto_gameover(void);
void game_over_cleanup(void);

#endif // DAMAGE_SYSTEM_H
