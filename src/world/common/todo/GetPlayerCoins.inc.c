#include "common.h"
#include "npc.h"

ApiStatus N(GetPlayerCoins)(Evt* script, s32 isInitialCall) {
    script->varTable[0] = gPlayerData.coins;
    return ApiStatus_DONE2;
}