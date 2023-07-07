#include "spc_02.h"
#include "menu/menu.h"

EntryList N(Entrances) = {
    [spc_01_Entry0]    { -246.0,   -144.0,  -105.0,   90.0 },
};

s32 N(map_init)(void) {
    sprintf(wMapTexName, "hos_tex");
    return FALSE;
}

MapSettings N(settings) = {
    .main = &N(EVS_Main),
    .entryList = &N(Entrances),
    .entryCount = ENTRY_COUNT(N(Entrances)),
    .background = &gBackgroundImage,
    .tattle = { MSG_MapTattle_kmr_20 },
};



EvtScript N(EVS_Main) = {
    EVT_SET(GB_WorldLocation, LOCATION_MARIOS_HOUSE) // pause_map.c
    EVT_CALL(SetSpriteShading, SHADING_NONE)
    EVT_SETUP_CAMERA_NO_LEAD()
    EVT_CALL(SetMusicTrack, 0, SONG_TOAD_TOWN, 0, 8)
    EVT_RETURN
    EVT_END
};
