#include "pause_common.h"
#include "sprite.h"
#include "sprite/npc/world_eldstar.h"
#include "sprite/npc/world_mamar.h"
#include "sprite/npc/world_skolar.h"
#include "sprite/npc/world_muskular.h"
#include "sprite/npc/world_misstar.h"
#include "sprite/npc/world_klevar.h"
#include "sprite/npc/world_kalmar.h"

extern Gfx gPauseDLSpiritsBg[];
extern s8 pause_spirits_bg_png[];

void pause_spirits_draw_contents(MenuPanel* menu, s32 baseX, s32 baseY, s32 width, s32 height, s32 opacity, s32 darkening);
void pause_spirits_draw_title(MenuPanel* menu, s32 baseX, s32 baseY, s32 width, s32 height, s32 opacity, s32 darkening);
void pause_spirits_init(MenuPanel* panel);
void pause_spirits_handle_input(MenuPanel* panel);
void pause_spirits_update(MenuPanel* panel);
void pause_spirits_cleanup(MenuPanel* panel);

static s32 gPauseSpiritsSpriteIDs[7];
static s32 D_802706DC;
static s32 gPauseSpiritsIndexes[7];
static s32 gPauseSpiritsNumSpirits;

Vp gPauseSpiritsViewport = {
    .vp = {
        .vscale = { 640, 480, 511, 0 },
        .vtrans = { 640, 480, 511, 0 },
    }
};
s32 gPauseSpiritsSpriteAnims[][4] = {
    {
        NPC_ANIM_world_eldstar_Palette_00_Anim_0,
        NPC_ANIM_world_eldstar_Palette_00_Anim_1,
        NPC_ANIM_world_eldstar_Palette_00_Anim_2,
        ANIM_END
    },
    {
        NPC_ANIM_world_mamar_Palette_00_Anim_0,
        NPC_ANIM_world_mamar_Palette_00_Anim_1,
        NPC_ANIM_world_mamar_Palette_00_Anim_2,
        ANIM_END
    },
    {
        NPC_ANIM_world_skolar_Palette_00_Anim_0,
        NPC_ANIM_world_skolar_Palette_00_Anim_1,
        NPC_ANIM_world_skolar_Palette_00_Anim_2,
        ANIM_END
    },
    {
        NPC_ANIM_world_muskular_Palette_00_Anim_0,
        NPC_ANIM_world_muskular_Palette_00_Anim_1,
        NPC_ANIM_world_muskular_Palette_00_Anim_2,
        ANIM_END
    },
    {
        NPC_ANIM_world_misstar_Palette_00_Anim_0,
        NPC_ANIM_world_misstar_Palette_00_Anim_1,
        NPC_ANIM_world_misstar_Palette_00_Anim_2,
        ANIM_END
    },
    {
        NPC_ANIM_world_klevar_Palette_00_Anim_0,
        NPC_ANIM_world_klevar_Palette_00_Anim_1,
        NPC_ANIM_world_klevar_Palette_00_Anim_3,
        ANIM_END
    },
    {
        NPC_ANIM_world_kalmar_Palette_00_Anim_0,
        NPC_ANIM_world_kalmar_Palette_00_Anim_1,
        NPC_ANIM_world_kalmar_Palette_00_Anim_2,
        ANIM_END
    }
};
s8 gPauseSpiritsGridData[] = {
    5, 6, 0, 1, 2,
    5, 4, 4, 3, 2
};
Vec2i gPauseSpiritsPositions[] = {
    { .x = 120, .y = -5 },
    { .x = 179, .y = -2 },
    { .x = 222, .y = 28 },
    { .x = 165, .y = 45 },
    { .x = 76, .y = 45 },
    { .x = 17, .y = 28 },
    { .x = 59, .y = -2 }
};
s32 gPauseSpiritsDrawOrder[] = { 6, 0, 1, 5, 2, 4, 3 };
Vec2i gPauseSpiritsCursorPositions[] = {
    { .x = 120, .y = 50 },
    { .x = 180, .y = 55 },
    { .x = 220, .y = 90 },
    { .x = 165, .y = 110 },
    { .x = 75, .y = 110 },
    { .x = 20, .y = 90 },
    { .x = 60, .y = 55 }
};
MenuWindowBP gPauseSpiritsWindowsBPs[] = {
    {
        .windowID = WINDOW_ID_PAUSE_SPIRITS,
        .unk_01 = 0,
        .pos = { .x = 3, .y = 16 },
        .width = 289,
        .height = 154,
        .unk_0A = { 1, 0 },
        .fpDrawContents = &pause_spirits_draw_contents,
        .tab = NULL,
        .parentID = WINDOW_ID_PAUSE_MAIN,
        .fpUpdate = { 2 },
        .unk_1C = 0,
        .style = &gPauseWS_23
    },
    {
        .windowID = WINDOW_ID_PAUSE_SPIRITS_TITLE,
        .unk_01 = 0,
        .pos = { .x = 86, .y = 124 },
        .width = 120,
        .height = 20,
        .unk_0A = { 0, 0 },
        .fpDrawContents = &pause_spirits_draw_title,
        .tab = NULL,
        .parentID = WINDOW_ID_PAUSE_SPIRITS,
        .fpUpdate = { 1 },
        .unk_1C = 0,
        .style = &gPauseWS_24
    }
};
MenuPanel gPausePanelSpirits = {
    .initialized = FALSE,
    .col = 2,
    .row = 0,
    .selected = 0,
    .page = 0,
    .numCols = 5,
    .numRows = 2,
    .numPages = 0,
    .gridData = gPauseSpiritsGridData,
    .fpInit = &pause_spirits_init,
    .fpHandleInput = &pause_spirits_handle_input,
    .fpUpdate = &pause_spirits_update,
    .fpCleanup = &pause_spirits_cleanup
};

void pause_spirits_draw_contents(MenuPanel* menu, s32 baseX, s32 baseY, s32 width, s32 height, s32 opacity, s32 darkening) {
    Matrix4f matrix1;
    Matrix4f matrix2;
    s32 i, j;
    s32 s0;
    s32 s1;
    s32 index;
    f32 x, y;
    f32 offsetY;
    s32 x1, y1, x2, y2;
    f32 frameCounter;
    f32 scale;
    PlayerData* playerData = get_player_data();

    gDPPipeSync(gMasterGfxPos++);
    gSPViewport(gMasterGfxPos++, &gPauseSpiritsViewport);
    gSPDisplayList(gMasterGfxPos++, gPauseDLSpiritsBg);

    for (i = 0; i < 5; i++) {
        gDPLoadTextureTile_4b(gMasterGfxPos++, pause_spirits_bg_png, G_IM_FMT_CI, 128, 110,
                               0, i * 22, 127, i * 22 + 21, 0,
                               G_TX_MIRROR, G_TX_CLAMP, 7, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
        pause_draw_rect((baseX + 15) * 4, (baseY + 22 + i * 22) * 4, (baseX + 271) * 4, (baseY + 22 + i * 22 + 22) * 4, 0, 16, 16 + i * 704, 0x400, 0x400);
        gDPPipeSync(gMasterGfxPos++);
    }

    guOrthoF(matrix1, 0.0f, 320.0f, 240.0f, 0.0f, -1000.0f, 1000.0f, 1.0f);
    guMtxF2L(matrix1, &gDisplayContext->matrixStack[gMatrixListPos]);
    gSPMatrix(gMasterGfxPos++, &gDisplayContext->matrixStack[gMatrixListPos++], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guTranslateF(matrix1, 0.0f, 0.0f, 0.0f);
    guMtxF2L(matrix1, &gDisplayContext->matrixStack[gMatrixListPos]);
    gSPMatrix(gMasterGfxPos++, &gDisplayContext->matrixStack[gMatrixListPos++], G_MTX_PUSH | G_MTX_LOAD | G_MTX_MODELVIEW);


    for (i = 0; i < gPauseSpiritsNumSpirits; i++) {
        index = gPauseSpiritsDrawOrder[i];
        frameCounter = gGameStatusPtr->frameCounter * 4;
        x = gPauseSpiritsPositions[index].x;
        y = gPauseSpiritsPositions[index].y;

        if (playerData->maxStarPower < index + 1) {
            s0 = 0;
            s1 = 0x80;
            offsetY = 0.0f;
        } else {
            s0 = 0xFF;
            s1 = 0xFF;
            offsetY = sin_deg(index * index * index + frameCounter * 0.4321 + frameCounter * (index * 0.02 + 0.1324))
                  * 5.0f
                  * sin_deg(index * 0.25 + frameCounter + frameCounter * (0.0432 - index * 0.01));

        }

        func_802DE894(gPauseSpiritsSpriteIDs[gPauseSpiritsIndexes[index]], 8, s0, s0, s0, s1, 0x40);
        guTranslateF(matrix1, baseX + 22 + x, baseY + 77 + y + offsetY, 0.0f);
        guRotateF(matrix2, 180.0f, 0.0f, 0.0f, 1.0f);
        guMtxCatF(matrix2, matrix1, matrix1);

        if (index == 1 || index == 2 || index == 3) {
            guRotateF(matrix2, 180.0f, 0.0f, 1.0f, 0.0f);
            guMtxCatF(matrix2, matrix1, matrix1);
        }

        guScaleF(matrix2, 0.9f, 0.9f, 0.9f);
        guMtxCatF(matrix2, matrix1, matrix1);
        spr_draw_npc_sprite(gPauseSpiritsSpriteIDs[gPauseSpiritsIndexes[index]], 0, 0, 0, matrix1);
    }

    gSPPopMatrix(gMasterGfxPos++, G_MTX_MODELVIEW);

    x1 = baseX + 7;
    y1 = baseY + 14;
    x2 = baseX + 279;
    y2 = baseY + 140;

    if (x1 <= 0) {
        x1 = 1;
    }
    if (y1 <= 0) {
        y1 = 1;
    }

    if (x2 <= 0 || y2 <= 0 || x1 >= SCREEN_WIDTH - 1 || y1 >= SCREEN_HEIGHT - 1) {
        return;
    }

    if (x2 >= SCREEN_WIDTH - 1) {
        x2 = SCREEN_WIDTH - 1;
    }
    if (y2 >= SCREEN_HEIGHT - 1) {
        y2 = SCREEN_HEIGHT - 1;
    }

    gDPSetScissor(gMasterGfxPos++, G_SC_NON_INTERLACE, x1, y1, x2, y2);
    draw_box(0, &gPauseWS_25, baseX + 7, baseY + 14, 0, 272, 126, opacity, darkening, 1.0f, 1.0f, 0, 0, 0, 0, 0, 0, width, height, 0);

    if (gPauseMenuCurrentTab == 5) {
        pause_set_cursor_pos(WINDOW_ID_PAUSE_SPIRITS, baseX + gPauseSpiritsCursorPositions[menu->selected].x, baseY + gPauseSpiritsCursorPositions[menu->selected].y);
    }
}

void pause_spirits_draw_title(MenuPanel* menu, s32 baseX, s32 baseY, s32 width, s32 height, s32 opacity, s32 darkening) {
    s32 msgID;
    PlayerData* playerData = get_player_data();

    if (gPauseMenuCurrentTab == 5) {
        if (playerData->maxStarPower > gPauseSpiritsIndexes[menu->selected]) {
            msgID = gPauseSpiritsIndexes[menu->selected] + MESSAGE_ID(0x1D, 0x2A);
        } else {
            msgID = pause_get_menu_msg(0x56);
        }
        draw_msg(msgID, baseX + ((width - get_msg_width(msgID, 0)) >> 1), baseY + 1, 255, 0, 0);
    }
}

void pause_spirits_init(MenuPanel* panel) {
    s32 i;

    get_player_data();
    gPauseSpiritsNumSpirits = 0;

    for (i = 0; i < ARRAY_COUNT(gPauseSpiritsIndexes); i++) {
        gPauseSpiritsIndexes[gPauseSpiritsNumSpirits] = i;
        gPauseSpiritsNumSpirits++;
    }

    for (i = 0; i < ARRAY_COUNT(gPauseSpiritsSpriteAnims); i++) {
        gPauseSpiritsSpriteIDs[i] = spr_load_npc_sprite(gPauseSpiritsSpriteAnims[i][0], gPauseSpiritsSpriteAnims[i]);
    }

    for (i = 0; i < ARRAY_COUNT(gPauseSpiritsWindowsBPs); i++) {
        gPauseSpiritsWindowsBPs[i].tab = panel;
    }
    setup_pause_menu_tab(gPauseSpiritsWindowsBPs, ARRAY_COUNT(gPauseSpiritsWindowsBPs));
    panel->initialized = TRUE;
}

void pause_spirits_handle_input(MenuPanel* panel) {
    s32 oldSelected = panel->selected;

    if (gPauseHeldButtons & BUTTON_STICK_LEFT) {
        while (TRUE) {
            panel->col--;
            if (panel->col < 0) {
                panel->col = 0;
                break;
            }

            if (panel->selected != MENU_PANEL_SELECTED_GRID_DATA(panel)) {
                break;
            }
        }
    }

    if (gPauseHeldButtons & BUTTON_STICK_RIGHT) {
        while (TRUE) {
            panel->col++;
            if (panel->col >= panel->numCols) {
                panel->col = panel->numCols - 1;
                break;
            }

            if (panel->selected != MENU_PANEL_SELECTED_GRID_DATA(panel)) {
                break;
            }
        }
    }

    if (gPauseHeldButtons & BUTTON_STICK_UP) {
        if (panel->selected == 2) {
            panel->col = 3;
            panel->row = 0;
        } else if (panel->selected == 5) {
            panel->col = 1;
            panel->row = 0;
        } else {
            while (TRUE) {
                panel->row--;
                if (panel->row < 0) {
                    panel->row = 0;
                    break;
                }
                if (panel->selected != MENU_PANEL_SELECTED_GRID_DATA(panel)) {
                    break;
                }
            }
        }
    }

    if (gPauseHeldButtons & BUTTON_STICK_DOWN) {
        if (panel->selected == 2) {
            panel->col = 3;
            panel->row = 1;
        } else if (panel->selected == 5) {
            panel->col = 1;
            panel->row = 1;
        } else {
            while (TRUE) {
                panel->row++;
                if (panel->row >= panel->numRows) {
                    panel->row = panel->numRows - 1;
                    break;
                }
                if (panel->selected != MENU_PANEL_SELECTED_GRID_DATA(panel)) {
                    break;
                }
            }
        }
    }

    panel->selected = MENU_PANEL_SELECTED_GRID_DATA(panel);

    if (panel->selected == 4) {
        panel->col = 1;
        panel->row = 1;
    }

    if (panel->selected != oldSelected) {
        sfx_play_sound(SOUND_MENU_CHANGE_SELECTION);
    }

    if (gPausePressedButtons & BUTTON_B) {
        gPauseMenuCurrentTab = 0;
        sfx_play_sound(SOUND_MENU_BACK);
        return;
    }

    gPauseCurrentDescIconScript = 0;

    if (get_player_data()->maxStarPower <= gPauseSpiritsIndexes[panel->selected]) {
        gPauseCurrentDescMsg = pause_get_menu_msg(0x56);
    } else {
        gPauseCurrentDescMsg = MESSAGE_ID(0x1D, 0x31) + gPauseSpiritsIndexes[panel->selected];
    }
}

void pause_spirits_update(MenuPanel* panel) {
    PlayerData* playerData = get_player_data();
    s32 i;

    for (i = 0; i < gPauseSpiritsNumSpirits; i++) {
        if (i < playerData->maxStarPower && gPauseMenuCurrentTab == 5 && i == panel->selected) {
            spr_update_sprite(gPauseSpiritsSpriteIDs[i], gPauseSpiritsSpriteAnims[i][1], 1.0f);
        } else {
            //TODO find better match
            do { spr_update_sprite(gPauseSpiritsSpriteIDs[i], gPauseSpiritsSpriteAnims[i][0], 1.0f); } while (0);
        }
    }
}

void pause_spirits_cleanup(MenuPanel* panel) {
    s32 i;

    for (i = 0; i < ARRAY_COUNT(gPauseSpiritsSpriteIDs); i++) {
        spr_free_sprite(gPauseSpiritsSpriteIDs[i]);
    }
}