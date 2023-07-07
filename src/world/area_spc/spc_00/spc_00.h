/// @file spc_00.h
/// @brief Space Region - Test Area

#include "common.h"
#include "message_ids.h"
#include "map.h"

#include "../spc.h"
#include "mapfs/spc_00.h"

enum {
    MV_ShipPosX = MapVar(0),
    MV_ShipPosY = MapVar(1),
    MV_AttackTimer = MapVar(2),
};

enum {
    spc_00_ENTRY_0
};

#define NAMESPACE spc_00

extern EvtScript N(EVS_Main);
extern EvtScript N(GameLoop);
extern EvtScript N(NpcInit_Bowser);
extern EvtScript N(NpcIdle_Bowser);
extern NpcGroupList N(DefaultNpcs);
