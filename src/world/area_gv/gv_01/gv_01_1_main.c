#include "gv_01.h"
#include "sprite.h"
#include "hud_element.h"
#include "sprite/player.h"

#include "world/common/entity/Pipe.inc.c"

#include "world/common/atomic/TexturePan.inc.c"

#include "world/action/damage_system.h"

#include "menu/menu.h"

s32 N(DropLetterYCurve)[] = {
    200, 170, 140, 110, 80, 50, 20, 10,
    26, 29, 30, 29, 26, 10, 0, 12,
    15, 16, 15, 11, 6, 0, 4, 3,
    1, 0,
};

s32 N(DropLastLetterYCurve)[] = {
    200, 170, 140, 110, 80, 50, 20, 0,
    12, 15, 16, 15, 11, 6, 0, 4,
    3, 1, 0,
};

API_CALLABLE(N(FadeToTitleScreen));
API_CALLABLE(N(ChangeStateToTitleScreen));

EvtScript N(EVS_DropLetter) = {
    EVT_USE_BUF(N(DropLetterYCurve))
    EVT_LOOP(ARRAY_COUNT(N(DropLetterYCurve)))
        EVT_BUF_READ1(LVar1)
        EVT_CALL(TranslateGroup, LVar0, 0, LVar1, 0)
        EVT_WAIT(1)
    EVT_END_LOOP
    EVT_RETURN
    //@bug not terminated!
};

EvtScript N(EVS_DropLastLetter) = {
    EVT_USE_BUF(N(DropLastLetterYCurve))
    EVT_LOOP(ARRAY_COUNT(N(DropLastLetterYCurve)))
        EVT_BUF_READ1(LVar1)
        EVT_CALL(TranslateGroup, LVar0, 0, LVar1, 0)
        EVT_WAIT(1)
    EVT_END_LOOP
    EVT_RETURN
    //@bug not terminated!
};

#if VERSION_PAL
// TODO: define MODEL_ constants for PAL version of gv_01 map

EvtScript N(EVS_E8C6F8) = {
    EVT_CALL(EnableGroup, 20, 0)
    EVT_CALL(EnableGroup, 22, 0)
    EVT_CALL(EnableGroup, 24, 0)
    EVT_CALL(EnableGroup, 26, 0)
    EVT_CALL(EnableGroup, 28, 0)
    EVT_CALL(EnableGroup, 30, 0)
    EVT_CALL(EnableGroup, 32, 0)
    EVT_CALL(EnableGroup, 34, 0)
    EVT_CALL(EnableGroup, 36, 0)
    EVT_CALL(EnableGroup, 38, 0)
    EVT_CALL(EnableGroup, 40, 0)
    EVT_CALL(TranslateGroup, 4, 0, 200, 0)
    EVT_CALL(TranslateGroup, 6, 0, 200, 0)
    EVT_CALL(TranslateGroup, 8, 0, 200, 0)
    EVT_CALL(TranslateGroup, 10, 0, 200, 0)
    EVT_CALL(TranslateGroup, 12, 0, 200, 0)
    EVT_CALL(TranslateGroup, 14, 0, 200, 0)
    EVT_CALL(TranslateGroup, 16, 0, 200, 0)
    EVT_CALL(TranslateGroup, 18, 0, 200, 0)
    EVT_WAIT(5)
    EVT_SET(LVar0, 4)
    EVT_EXEC(N(EVS_DropLetter))
    EVT_WAIT(5)
    EVT_SET(LVar0, 6)
    EVT_EXEC(N(EVS_DropLetter))
    EVT_WAIT(10)
    EVT_SET(LVar0, 8)
    EVT_EXEC(N(EVS_DropLetter))
    EVT_WAIT(5)
    EVT_SET(LVar0, 10)
    EVT_EXEC(N(EVS_DropLetter))
    EVT_WAIT(5)
    EVT_SET(LVar0, 12)
    EVT_EXEC(N(EVS_DropLetter))
    EVT_WAIT(5)
    EVT_SET(LVar0, 14)
    EVT_EXEC(N(EVS_DropLetter))
    EVT_WAIT(5)
    EVT_SET(LVar0, 16)
    EVT_EXEC(N(EVS_DropLetter))
    EVT_WAIT(15)
    EVT_SET(LVar0, 18)
    EVT_EXEC(N(EVS_DropLastLetter))
    EVT_RETURN
    EVT_END
};

EvtScript N(EVS_E8CA04) = {
    EVT_CALL(EnableGroup, 4, 0)
    EVT_CALL(EnableGroup, 6, 0)
    EVT_CALL(EnableGroup, 8, 0)
    EVT_CALL(EnableGroup, 10, 0)
    EVT_CALL(EnableGroup, 12, 0)
    EVT_CALL(EnableGroup, 14, 0)
    EVT_CALL(EnableGroup, 16, 0)
    EVT_CALL(EnableGroup, 18, 0)
    EVT_CALL(TranslateGroup, 20, 0, 200, 0)
    EVT_CALL(TranslateGroup, 22, 0, 200, 0)
    EVT_CALL(TranslateGroup, 24, 0, 200, 0)
    EVT_CALL(TranslateGroup, 26, 0, 200, 0)
    EVT_CALL(TranslateGroup, 28, 0, 200, 0)
    EVT_CALL(TranslateGroup, 30, 0, 200, 0)
    EVT_CALL(TranslateGroup, 32, 0, 200, 0)
    EVT_CALL(TranslateGroup, 34, 0, 200, 0)
    EVT_CALL(TranslateGroup, 36, 0, 200, 0)
    EVT_CALL(TranslateGroup, 38, 0, 200, 0)
    EVT_CALL(TranslateGroup, 40, 0, 200, 0)
    EVT_WAIT(5)
    EVT_SET(LVar0, 20)
    EVT_EXEC(N(EVS_DropLetter))
    EVT_WAIT(4)
    EVT_SET(LVar0, 22)
    EVT_EXEC(N(EVS_DropLetter))
    EVT_WAIT(7)
    EVT_SET(LVar0, 24)
    EVT_EXEC(N(EVS_DropLetter))
    EVT_WAIT(4)
    EVT_SET(LVar0, 26)
    EVT_EXEC(N(EVS_DropLetter))
    EVT_WAIT(4)
    EVT_SET(LVar0, 28)
    EVT_EXEC(N(EVS_DropLetter))
    EVT_WAIT(4)
    EVT_SET(LVar0, 30)
    EVT_EXEC(N(EVS_DropLetter))
    EVT_WAIT(7)
    EVT_SET(LVar0, 32)
    EVT_EXEC(N(EVS_DropLetter))
    EVT_WAIT(4)
    EVT_SET(LVar0, 34)
    EVT_EXEC(N(EVS_DropLetter))
    EVT_WAIT(4)
    EVT_SET(LVar0, 36)
    EVT_EXEC(N(EVS_DropLetter))
    EVT_WAIT(4)
    EVT_SET(LVar0, 38)
    EVT_EXEC(N(EVS_DropLetter))
    EVT_WAIT(12)
    EVT_SET(LVar0, 40)
    EVT_EXEC(N(EVS_DropLastLetter))
    EVT_RETURN
    EVT_END
};
#endif

API_CALLABLE(N(DoCleanup)) {
    game_over_cleanup();
    return ApiStatus_DONE2;
}

EvtScript N(EVS_Main) = {
    EVT_CALL(DisablePlayerInput, TRUE)
    EVT_CALL(DisablePlayerPhysics, TRUE)
    EVT_CALL(N(DoCleanup))
    EVT_CALL(GetEntryID, LVar0)
    EVT_SET(GB_GameOverEntry, LVar0)
    EVT_CALL(SetSpriteShading, SHADING_NONE)
    EVT_CALL(SetMenuType, MENU_TYPE_GAME_OVER)
    EVT_CALL(SetMusicTrack, 0, SONG_KAMMY_KOOPA_THEME, 0, 8)
    EVT_SETUP_CAMERA_NO_LEAD()
    EVT_RETURN
    EVT_END
};

API_CALLABLE(N(FadeToTitleScreen)) {
    if (isInitialCall) {
        script->functionTemp[0] = 0;
        set_curtain_scale_goal(1.0);
        set_curtain_fade(0);
    }

    script->functionTemp[0] += (s32)(5 / DT);
    if (script->functionTemp[0] > 255) {
        script->functionTemp[0] = 255;
    }

    set_screen_overlay_color(SCREEN_LAYER_BACK, 208, 208, 208);
    set_screen_overlay_params_back(OVERLAY_SCREEN_COLOR, script->functionTemp[0]);
    set_screen_overlay_alpha(SCREEN_LAYER_BACK, 0);
    return script->functionTemp[0] == 255;
}

API_CALLABLE(N(ChangeStateToTitleScreen)) {
    gGameStatusPtr->isBattle = FALSE;
    gGameStatusPtr->unk_76 = 0;
    gGameStatusPtr->disableScripts = 0;
    gGameStatusPtr->keepUsingPartnerOnMapChange = FALSE;
    gOverrideFlags &= ~GLOBAL_OVERRIDES_8;
    general_heap_create();
    clear_render_tasks();
    clear_worker_list();
    clear_script_list();
    create_cameras_a();
    spr_init_sprites(PLAYER_SPRITES_MARIO_WORLD);
    clear_entity_models();
    clear_animator_list();
    clear_model_data();
    clear_sprite_shading_data();
    reset_background_settings();
    hud_element_clear_cache();
    clear_trigger_data();
    clear_printers();
    clear_entity_data(0);
    clear_screen_overlays();
    clear_player_status();
    clear_npcs();
    clear_player_data();
    reset_battle_status();
    init_encounter_status();
    clear_effect_data();
    clear_item_entity_data();
    clear_saved_variables();
    initialize_collision();
    intro_logos_set_fade_alpha(255);
    intro_logos_set_fade_color(208);
    set_game_mode(GAME_MODE_TITLE_SCREEN);
    return ApiStatus_FINISH;
}
