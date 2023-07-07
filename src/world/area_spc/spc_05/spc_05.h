/// @file spc_05.h
/// @brief Space Region - Hammer Bros Battle

#include "common.h"
#include "message_ids.h"
#include "map.h"

#include "../spc.h"
#include "mapfs/spc_01.h"


enum {
    spc_05_ENTRY_0
};

enum {
    MV_ShipPosX = MapVar(0),
    MV_ShipPosY = MapVar(1),
    MV_AttackTimer = MapVar(2),
};

#define NAMESPACE spc_05

extern NpcGroupList N(DefaultNpcs);
extern EvtScript N(EVS_Main);
