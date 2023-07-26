#ifndef BULLET_H
#define BULLET_H

#include "npc.h"

void use_bullet(void);
s32 test_bullet_first_strike(Npc* enemy);
void clear_bullets(void);
ApiStatus ClearAllPlayerBullets(Evt* script, s32 isInitialCall);

#endif // BULLET_H
