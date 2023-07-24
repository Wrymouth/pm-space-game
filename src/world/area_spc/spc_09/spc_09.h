/// @file spc_09.h
/// @brief Space Region - Koopa Bros Battle

#include "common.h"
#include "message_ids.h"
#include "map.h"

#include "../spc.h"
#include "mapfs/spc_09.h"

enum {
    spc_09_ENTRY_0
};

enum {
    MF_EnemyDefeated = MapFlag(0),
};

enum {
    MV_ShipPosX = MapVar(0),
    MV_ShipPosY = MapVar(1),
    MV_SplitTimer = MapVar(2),
    MV_ShellTimer = MapVar(3),
};

#define NAMESPACE spc_09

extern NpcGroupList N(DefaultNpcs);
extern EvtScript N(EVS_Main);