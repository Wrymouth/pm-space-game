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
    SwitchDuration = 90,
};

enum {
    MF_EnemyDefeated = MapFlag(0),
    MF_PhaseTransition = MapFlag(1),
    MF_StartedJumping = MapFlag(2),
    MF_IsGiantJumping = MapFlag(3),
};

enum {
    MV_ShipPosX = MapVar(0),
    MV_ShipPosY = MapVar(1),
    MV_BattlePhase = MapVar(2),
    MV_SwitchTimer = MapVar(3),
    MV_HammersTimer = MapVar(4),
    MV_BallTimer = MapVar(5),
    MV_FlamesTimer = MapVar(6),
    MV_ChaseTimer = MapVar(7),
    MV_JumpTimer = MapVar(8),
    MV_RainTimer = MapVar(9),
};

#define NAMESPACE spc_10

extern NpcGroupList N(DefaultNpcs);
extern EvtScript N(EVS_Main);
