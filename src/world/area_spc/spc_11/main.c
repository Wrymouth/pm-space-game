#include "spc_11.h"
#include "menu/menu.h"

EntryList N(Entrances) = {
    [spc_11_ENTRY_0]    { -71.0,   34.0,  -170.0,   90.0 },
};

s32 N(map_init)(void) {
    sprintf(wMapTexName, "hos_tex");
    return FALSE;
}

MapSettings N(settings) = {
    .main = &N(EVS_Main),
    .entryList = &N(Entrances),
    .entryCount = ENTRY_COUNT(N(Entrances)),
    .background = 0,
    .tattle = { MSG_MapTattle_kmr_20 },
};

EvtScript N(BadEnding) = {
    EVT_CALL(ShowMessageAtScreenPos, MSG_Space_BadEnding, 160, 40)
    EVT_CALL(GotoMap, EVT_PTR("spc_01"), 0)
    EVT_RETURN
    EVT_END
};

EvtScript N(EVS_Main) = {
    EVT_SET(GB_WorldLocation, LOCATION_MARIOS_HOUSE) // pause_map.c
    EVT_CALL(SetSpriteShading, SHADING_NONE)
    EVT_SETUP_CAMERA_NO_LEAD()
    EVT_CALL(SetMusicTrack, 0, SONG_KAMMY_KOOPA_THEME, 0, 8)
    EVT_CALL(DisablePlayerInput, TRUE)
    EVT_CALL(DisablePlayerPhysics, TRUE)
    EVT_EXEC(N(BadEnding))
    EVT_RETURN
    EVT_END
};
