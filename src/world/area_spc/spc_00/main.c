#include "spc_00.h"

s32 N(map_init)(void) {
    sprintf(wMapTexName, "hos_tex");
    return FALSE;
}

EntryList N(Entrances) = {
    [spc_00_ENTRY_0]    { -246.0,   -144.0,  -105.0,   90.0 },
};

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
    EVT_EXEC(spc_00_GameLoop)
    EVT_RETURN
    EVT_END
};

API_CALLABLE(N(CheckButtonInput)) {
    Bytecode* args = script->ptrReadPos;
    s8 joystickX = gGameStatusPtr->stickX;
    s8 joystickY = gGameStatusPtr->stickY;
    evt_set_variable(script, *args++, joystickX);
    evt_set_variable(script, *args++, joystickY);
    return ApiStatus_DONE2;
};

EvtScript N(GameLoop) = {
    EVT_CALL(DisablePlayerInput, TRUE)
    EVT_CALL(DisablePlayerPhysics, TRUE)
    EVT_LOOP(0)
        EVT_CALL(spc_00_CheckButtonInput, LVar0, LVar1)
        EVT_DIV(LVar0, 8)
        EVT_DIV(LVar1, 8)
        EVT_ADD(MapVar(0), LVar0)
        EVT_ADD(MapVar(1), LVar1)
        EVT_CALL(TranslateGroup, Model_Spaceship, MapVar(0), MapVar(1), 0)
        EVT_CALL(GetPlayerPos, LVar0, LVar1, LVar2)
        EVT_SET(LVar0, MapVar(0))
        EVT_SUB(LVar0, 12) 
        EVT_SET(LVar1, MapVar(1))
        EVT_ADD(LVar1, 20) 
        EVT_CALL(SetPlayerPos, LVar0, LVar1, 12)
        EVT_WAIT(1)
    EVT_END_LOOP
    EVT_RETURN
    EVT_END
};


