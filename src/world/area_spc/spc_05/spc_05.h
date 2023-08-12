/// @file spc_05.h
/// @brief Space Region - Jr Troopa Battle

#include "common.h"
#include "message_ids.h"
#include "map.h"

#include "../spc.h"
#include "mapfs/spc_05.h"

enum {
    spc_05_ENTRY_0
};

enum {
    MF_EnemyDefeated = MapFlag(0),
    MF_PhaseTransition = MapFlag(1),
};

enum {
    MV_ShipPosX = MapVar(0),
    MV_ShipPosY = MapVar(1),
    MV_BattlePhase = MapVar(2),
    MV_EggsTimer = MapVar(3),
    MV_SummonTimer = MapVar(4),
    MV_SummonMax = MapVar(5),
};

#define NAMESPACE spc_05

extern NpcGroupList N(DefaultNpcs);
extern EvtScript N(EVS_Main);
