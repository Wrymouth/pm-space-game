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
    MF_EnemyHasDestination = MapFlag(1),
    MF_LightningCanDamagePlayer = MapFlag(2),
    MF_ThrowingSpinies = MapFlag(3),
};

enum {
    MV_ShipPosX = MapVar(0),
    MV_ShipPosY = MapVar(1),
    MV_BattlePhase = MapVar(2),
    MV_LightningTimer = MapVar(3),
    MV_AttackTimer = MapVar(4),
    MV_DestX = MapVar(5),
    MV_DestY = MapVar(6),
    MV_LightningX = MapVar(7),
    MV_SpinyTimer = MapVar(8),
    MV_SpinyCount = MapVar(9),
};

#define NAMESPACE spc_07

extern NpcGroupList N(DefaultNpcs);
extern EvtScript N(EVS_Main);
