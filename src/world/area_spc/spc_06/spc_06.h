/// @file spc_06.h
/// @brief Space Region - Hammer Bros Battle

#include "common.h"
#include "message_ids.h"
#include "map.h"

#include "../spc.h"
#include "mapfs/spc_06.h"

enum {
    spc_06_ENTRY_0
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

#define NAMESPACE spc_06

extern NpcGroupList N(DefaultNpcs);
extern EvtScript N(EVS_Main);
