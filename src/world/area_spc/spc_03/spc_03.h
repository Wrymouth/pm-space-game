/// @file spc_03.h
/// @brief Space Region - Character Select

#include "common.h"
#include "message_ids.h"
#include "map.h"

#include "../spc.h"
#include "mapfs/spc_03.h"

enum {
    spc_03_ENTRY_0
};

enum {
    MF_DialogueActive = MapFlag(0),
    MF_HUDInitialized = MapFlag(1),
};

enum {
    MV_SelectedRow = MapVar(0),
    MV_SelectedCol = MapVar(1),
    MV_ActiveNpcID = MapVar(2),
    MV_SpriteShowWait = MapVar(3),
};

#define NAMESPACE spc_03

extern NpcGroupList N(DefaultNpcs);
extern EvtScript N(EVS_Main);
