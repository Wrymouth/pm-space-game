/// @file spc_10.h
/// @brief Space Region - Bowser Battle

#include "common.h"
#include "message_ids.h"
#include "map.h"

#include "../spc.h"
#include "mapfs/spc_10.h"

enum {
    spc_10_ENTRY_0
};

enum {
    MF_EnemyDefeated = MapFlag(0),
};

enum {
    MV_ShipPosX = MapVar(0),
    MV_ShipPosY = MapVar(1),
    MV_BattlePhase = MapVar(2),
};

#define NAMESPACE spc_10

extern NpcGroupList N(DefaultNpcs);
extern EvtScript N(EVS_Main);
