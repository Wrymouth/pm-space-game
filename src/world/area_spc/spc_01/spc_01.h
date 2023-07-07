/// @file spc_01.h
/// @brief Space Region - Title Screen

#include "common.h"
#include "message_ids.h"
#include "map.h"

#include "../spc.h"
#include "mapfs/spc_01.h"

enum {
    spc_01_ENTRY_0
};

#define NAMESPACE spc_01

extern EvtScript N(ShowTitle);
extern NpcGroupList N(DefaultNpcs);
extern EvtScript N(EVS_Main);
extern EvtScript N(SetupTexturePan);
