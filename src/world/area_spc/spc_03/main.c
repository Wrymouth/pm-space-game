#include "spc_03.h"
#include "menu/menu.h"
#include "world/area_spc/common/texture_pan_bg.inc.c"

EntryList N(Entrances) = {
    [spc_03_ENTRY_0]    { -71.0,   34.0,  -170.0,   90.0 },
};

s32 N(map_init)(void) {
    sprintf(wMapBgName, "hos_bg");
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

EvtScript N(EVS_Main) = {
    EVT_SET(GB_WorldLocation, LOCATION_MARIOS_HOUSE) // pause_map.c
    EVT_CALL(SetSpriteShading, SHADING_NONE)
    EVT_SETUP_CAMERA_NO_LEAD()
    EVT_CALL(DisablePlayerInput, TRUE)
    EVT_CALL(DisablePlayerPhysics, TRUE)
    EVT_CALL(SetMusicTrack, 0, SONG_TUBBA_BLUBBA_THEME, 0, 8)
    EVT_CALL(SetMenuType, MENU_TYPE_CHARACTER_SELECT)
    EVT_EXEC(N(SetupTexturePan))
    EVT_RETURN
    EVT_END
};
