/// @file spc_00.h
/// @brief Space Region - Mario's House

#include "common.h"
#include "message_ids.h"
#include "map.h"

#include "../spc.h"
#include "mapfs/spc_00.h"

/*
#include "sprite/npc/Luigi.h"
#include "sprite/npc/Toad.h"
#include "sprite/npc/WorldParakarry.h"
#include "sprite/npc/ShyGuy.h"

enum {
    NPC_Scene_Parakarry         = 0,
    NPC_Scene_Luigi             = 1,

    NPC_Luigi_0                 = 0,
    NPC_Luigi_1                 = 1,
    NPC_ShyGuy                  = 2,
};

enum {
    MV_RecordsDataPtr       = MapVar(10),
};

enum {
    MF_LuigiWaiting         = MapFlag(10),
    MF_ReadyForPlayerEntry  = MapFlag(11),
    MF_Unk_0C               = MapFlag(12),
    MF_LuigiInBasement      = MapFlag(13),
};
*/

enum {
    spc_00_ENTRY_0
};

#define NAMESPACE spc_00

extern EvtScript N(EVS_Main);
extern EvtScript N(GameLoop);
