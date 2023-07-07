#include "spc_01.h"
#include "menu/menu.h"
#include "world/common/atomic/TexturePan.inc.c"

EntryList N(Entrances) = {
    [spc_01_Entry0]    { -67.0,   34.0,  557.0,   90.0 },
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
    EVT_EXEC(N(ShowTitle))
    EVT_CALL(SetMusicTrack, 0, SONG_PARADE_NIGHT, 0, 8)
    EVT_CALL(DisablePlayerInput, TRUE)
    EVT_CALL(MakeNpcs, FALSE, EVT_PTR(N(DefaultNpcs)))
    EVT_CALL(EnableTitleMenu)
    EVT_EXEC(N(SetupTexturePan))
    EVT_RETURN
    EVT_END
};

EvtScript N(SetupTexturePan) = {
    EVT_CALL(SetTexPanner, Model_ScrollingBG, TEX_PANNER_0)
    EVT_THREAD
        TEX_PAN_PARAMS_ID(TEX_PANNER_0)
        TEX_PAN_PARAMS_STEP(2000,2000,2000,0)
        TEX_PAN_PARAMS_FREQ(    0,    0,    0,    0)
        TEX_PAN_PARAMS_INIT(    0,    0,    0,    0)
        EVT_EXEC(N(EVS_UpdateTexturePan))
    EVT_END_THREAD
    EVT_RETURN
    EVT_END  
};

