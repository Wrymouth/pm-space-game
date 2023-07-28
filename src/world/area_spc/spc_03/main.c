#include "spc_03.h"
#include "menu/menu.h"
#include "world/area_spc/common/texture_pan_bg.inc.c"
#include "effects.h"
#include "world/action/damage_system.h"

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

API_CALLABLE(N(Menu_GetCharacterStringAndAnim)) {
    if (evt_get_variable(script, MF_HUDInitialized)) {
        if ((csSelectedRow != evt_get_variable(script, MV_SelectedRow)) || (csSelectedCol != evt_get_variable(script, MV_SelectedCol))) {
            evt_set_variable(script, LVar1, TRUE);
            evt_set_variable(script, MV_SelectedCol, csSelectedCol);
            evt_set_variable(script, MV_SelectedRow, csSelectedRow);
            if (csPanels[csSelectedRow][csSelectedCol].cleared == TRUE) {
                evt_set_variable(script, LVar2, MSG_Space_EnemyDefeated);
                evt_set_variable(script, LVar3, 7);
            } else {
                if (csSelectedRow == 0) {
                    if (csSelectedCol == 0) {
                        evt_set_variable(script, LVar2, MSG_Space_CharacterSelectHammer);
                        evt_set_variable(script, LVar3, 3);
                    } else {
                        evt_set_variable(script, LVar2, MSG_Space_CharacterSelectLakitu);
                        evt_set_variable(script, LVar3, 4);
                    }
                } else {
                    if (csSelectedCol == 0) {
                        evt_set_variable(script, LVar2, MSG_Space_CharacterSelectWhale);
                        evt_set_variable(script, LVar3, 5);
                    } else {
                        evt_set_variable(script, LVar2, MSG_Space_CharacterSelectKoopa);
                        evt_set_variable(script, LVar3, 6);
                    }
                } 
            }
        } else {
            evt_set_variable(script, LVar1, FALSE);
        }
    } else {
        evt_set_variable(script, LVar1, TRUE);
        evt_set_variable(script, MF_HUDInitialized, TRUE);
        evt_set_variable(script, MV_SelectedCol, csSelectedCol);
        evt_set_variable(script, MV_SelectedRow, csSelectedRow);
        evt_set_variable(script, LVar2, MSG_Space_CharacterSelectIntro);
        evt_set_variable(script, LVar3, 2);
    }

    return ApiStatus_DONE2;
}


API_CALLABLE(N(DoCleanup)) {
    game_over_cleanup();
    return ApiStatus_DONE2;
}

EvtScript N(ShowCharacterString) = {
    EVT_LOOP(0)
        EVT_CALL(N(Menu_GetCharacterStringAndAnim))
        EVT_IF_TRUE(LVar1)
            EVT_IF_TRUE(LVarF)
                EVT_CALL(RemoveEffect, LVarF)
            EVT_END_IF
            EVT_CALL(PlayEffect, EFFECT_TATTLE_WINDOW, 0, 300, 45, 0, EVT_FLOAT(0.4), -1)
            EVT_CALL(SetNpcPos, MV_ActiveNpcID, 0, -1000, 0)
            EVT_CALL(SetNpcScale, MV_ActiveNpcID, EVT_FLOAT(1.0), EVT_FLOAT(1.0), EVT_FLOAT(1.0))
            EVT_SET(MV_SpriteShowWait, 0)
            EVT_SET(MV_ActiveNpcID, LVar3)
            EVT_CALL(Menu_SetCharacterMessage, LVar2)
        EVT_END_IF
        EVT_IF_EQ(MV_SpriteShowWait, 10)
            EVT_CALL(SetNpcPos, MV_ActiveNpcID, 225, 100, 16)
            EVT_CALL(SetNpcScale, MV_ActiveNpcID, EVT_FLOAT(2.0), EVT_FLOAT(2.0), EVT_FLOAT(2.0))
        EVT_ELSE
            EVT_ADD(MV_SpriteShowWait, 1)
        EVT_END_IF
        EVT_WAIT(1)
    EVT_END_LOOP
    EVT_RETURN
    EVT_END
};

EvtScript N(EVS_Main) = {
    EVT_SET(GB_WorldLocation, LOCATION_MARIOS_HOUSE) // pause_map.c
    EVT_CALL(SetSpriteShading, SHADING_NONE)
    EVT_SETUP_CAMERA_NO_LEAD()
    EVT_CALL(N(DoCleanup))
    EVT_CALL(DisablePlayerInput, TRUE)
    EVT_CALL(DisablePlayerPhysics, TRUE)
    EVT_CALL(SetMusicTrack, 0, SONG_TUBBA_BLUBBA_THEME, 0, 8)
    EVT_CALL(MakeNpcs, FALSE, EVT_PTR(N(DefaultNpcs)))
    EVT_THREAD
        EVT_WAIT(20)
        EVT_CALL(SetMenuType, MENU_TYPE_CHARACTER_SELECT)
    EVT_END_THREAD
    EVT_EXEC(N(SetupTexturePan))
    EVT_EXEC(N(ShowCharacterString))
    EVT_RETURN
    EVT_END
};
