/// @file spc_08.h
/// @brief Space Region - Whale Battle

#include "common.h"
#include "message_ids.h"
#include "map.h"

#include "../spc.h"
#include "mapfs/spc_08.h"

enum {
    spc_08_ENTRY_0
};

enum {
    MF_EnemyDefeated = MapFlag(0),
    MF_PhaseTransition = MapFlag(1),
};

enum {
    MV_ShipPosX = MapVar(0),
    MV_ShipPosY = MapVar(1),
    MV_BattlePhase = MapVar(2),
    MV_WaterTimer = MapVar(3),
    MV_WaterSubTimer = MapVar(4),
    MV_WaterCount = MapVar(5),
    MV_FuzzyTimer = MapVar(6),
    MV_WaterMax = MapVar(7),
};

#define NAMESPACE spc_08

extern NpcGroupList N(DefaultNpcs);
extern EvtScript N(EVS_Main);
