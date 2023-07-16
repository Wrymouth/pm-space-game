/// @file spc_07.h
/// @brief Space Region - Huff N Puff Battle

#include "common.h"
#include "message_ids.h"
#include "map.h"

#include "../spc.h"
#include "mapfs/spc_07.h"

enum {
    spc_07_ENTRY_0
};

enum {
    MF_EnemyDefeated = MapFlag(0),
};

enum {
    MV_ShipPosX = MapVar(0),
    MV_ShipPosY = MapVar(1),
    MV_HammerTimer = MapVar(2),
    MV_ShotTimer = MapVar(3),
};

#define NAMESPACE spc_07

extern NpcGroupList N(DefaultNpcs);
extern EvtScript N(EVS_Main);
