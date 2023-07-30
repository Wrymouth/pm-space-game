#include "spc_05.h"
#include "menu/menu.h"
#include "world/common/npc/StarSpirit.h"
#include "world/common/npc/JrTroopa.h"
#include "world/area_spc/common/game_loop.inc.c"
#include "world/area_spc/common/texture_pan_bg.inc.c"

EntryList N(Entrances) = {
    [spc_05_ENTRY_0]    { -972.0,   -144.0,  -105.0,   90.0 },
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

EvtScript N(OpeningCutscene) = {
    EVT_CALL(N(EnableSpaceShipMode), FALSE)
    EVT_CALL(UseSettingsFrom, CAM_DEFAULT, -777, -156, -6)
    EVT_CALL(SetPanTarget, CAM_DEFAULT, -777, -156, -6)
    EVT_CALL(PanToTarget, CAM_DEFAULT, 0, 1)
    EVT_CALL(TranslateGroup, Model_Spaceship, -248, -181, 0)
    EVT_CALL(DisablePlayerInput, TRUE)
    EVT_CALL(DisablePlayerPhysics, TRUE)
    EVT_CALL(SetPlayerActionState, ACTION_STATE_LAND)
    EVT_CALL(SetPlayerPos, -260, -161, 12)
    EVT_CALL(SetNpcPos, 2,  -91, -181, 12)
    EVT_CALL(SpeakToPlayer, 2, ANIM_WorldEldstar_Talk, ANIM_WorldEldstar_Idle, 0, MSG_Space_OpeningScene1)
    EVT_CALL(UseSettingsFrom, CAM_DEFAULT, -260, -161, 12)
    EVT_CALL(ResetCam, CAM_DEFAULT, EVT_FLOAT(90.0))
    EVT_CALL(SetPanTarget, CAM_DEFAULT, -260, -161, 12)
    EVT_CALL(PanToTarget, CAM_DEFAULT, 0, 1)
    EVT_SET(GF_StartedSpaceShip, TRUE)
    EVT_WAIT_SECS(10)
    EVT_SET(GF_StartedSpaceShip, FALSE)
    EVT_CALL(SetNpcPos, 3, 320, 0, 12)
    EVT_CALL(SpeakToPlayer, 3, ANIM_JrTroopa_Talk, ANIM_JrTroopa_Idle, 0, MSG_Space_OpeningScene2)
    EVT_CALL(SpeakToPlayer, 2, ANIM_WorldEldstar_Talk, ANIM_WorldEldstar_Idle, 0, MSG_Space_OpeningScene3)
    EVT_CALL(SetMusicTrack, 0, SONG_JR_TROOPA_THEME, 0, 8 )
    EVT_WAIT(2)
    EVT_CALL(NpcFlyTo, 3, 260, 0, 12, 15, 0, 0)
    EVT_CALL(SpeakToPlayer, 3, ANIM_JrTroopa_Talk, ANIM_JrTroopa_Idle, 0, MSG_Space_OpeningScene4)
    EVT_CALL(SpeakToPlayer, 2, ANIM_WorldEldstar_Talk, ANIM_WorldEldstar_Idle, 0, MSG_Space_OpeningScene5)
    EVT_SET(GF_LearnedToShoot, TRUE)
    EVT_CALL(SpeakToPlayer, 3, ANIM_JrTroopa_Talk, ANIM_JrTroopa_Idle, 0, MSG_Space_OpeningScene6)
    EVT_CALL(SetMusicTrack, 0, SONG_JR_TROOPA_BATTLE, 0, 8 )
    EVT_SET(GF_StartedSpaceShip, TRUE)
    EVT_SET(MV_BattlePhase, 1)
    EVT_CALL(DisablePlayerInput, FALSE)
    EVT_RETURN
    EVT_END
};

EvtScript N(EVS_Main) = {
    EVT_SET(GB_WorldLocation, LOCATION_MARIOS_HOUSE) // pause_map.c
    EVT_CALL(SetSpriteShading, SHADING_NONE)
    EVT_SETUP_CAMERA_NO_LEAD()
    EVT_CALL(SetMusicTrack, 0, SONG_PARADE_DAY, 0, 8)
    EVT_CALL(MakeNpcs, FALSE, EVT_PTR(N(DefaultNpcs)))
    EVT_EXEC(N(SetupTexturePan))
    EVT_EXEC(N(OpeningCutscene))
    EVT_EXEC(N(GameLoop))
    EVT_RETURN
    EVT_END
};
