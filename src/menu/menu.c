
#include "common.h"
#include "gcc/string.h"
#include "message_ids.h"
#include "hud_element.h"
#include "menu.h"

void title_menu_cb_newgame(void* arg);
void title_menu_cb_settings(void* arg);
void title_menu_cb_credits(void* arg);

void draw_hammer_icon(void* arg0, s32 baseX, s32 baseY, s32 width, s32 height, s32 opacity, s32 darkening);
void draw_huff_icon(void* arg0, s32 baseX, s32 baseY, s32 width, s32 height, s32 opacity, s32 darkening);
void draw_whale_icon(void* arg0, s32 baseX, s32 baseY, s32 width, s32 height, s32 opacity, s32 darkening);
void draw_koopa_icon(void* arg0, s32 baseX, s32 baseY, s32 width, s32 height, s32 opacity, s32 darkening);

struct title_menu_item {
    char* name;
    void (*callback)(void* arg);
    void* callbackArg;
} titleMenuItems[] = {
    { "Start Game", title_menu_cb_newgame },
    { "Settings", title_menu_cb_settings },
    { "Credits", title_menu_cb_credits },
    { NULL, NULL },
    {},
};

struct title_menu {
    char* title;
    s32 selectedIndex;
    s32 scrollY;
    s32 targetScrollY;
    struct title_menu_item* items; // zero-terminated

    struct title_menu* previous; // linked list
} titleMenu = {
    .title = "",
    .selectedIndex = 0,
    .scrollY = 0,
    .targetScrollY = 0,
    .items = titleMenuItems,
    .previous = NULL,
};

extern HudScript HES_Item_Apple;
extern HudScript HES_Item_ApplePie;
extern HudScript HES_Item_AllorNothing;
extern HudScript HES_Item_Artifact;

Evt *script_credits = NULL;
Evt *script_credits_notif = NULL;
Evt *script_settings_explain_speed;

MenuType menuType = MENU_TYPE_NONE;


// story
s32 storyX = 20;
s32 storyY = 200;
s32 shiftDelay = 3;
s32 skipOpacity = 255;

// character select
s32 csSelectedRow = 0;
s32 csSelectedCol = 0;
s32 selectionDelay = 8;
s32 displayMessageID = MSG_Space_CharacterSelectIntro;

MenuIconState iconState = ICON_INIT;
s32 koopaIcon;
s32 huffIcon;
s32 hammerIcon;
s32 whaleIcon;

CharacterSelectPanel csPanels[2][2] = {{{"spc_06", FALSE, draw_hammer_icon}, {"spc_07", FALSE, draw_huff_icon}}, {{"spc_08", FALSE, draw_whale_icon}, {"spc_09", FALSE, draw_koopa_icon}}}; // rows, columns


// game over
s32 gvOpacity = 0;


void menu_close(void) {
    if (menuType == MENU_TYPE_CHARACTER_SELECT) {
        hud_element_free(koopaIcon);
        hud_element_free(huffIcon);
        hud_element_free(hammerIcon);
        hud_element_free(whaleIcon);
    }
    iconState = ICON_INIT;
    menuType = MENU_TYPE_NONE;
}

void menu_gotomap(char* argMap, s32 entry) {
    char* map = argMap;

    // Get area/map id pair
    s16 areaID;
    s16 mapID;
    get_map_IDs_by_name(map, &areaID, &mapID);

    // Go there
    gGameStatusPtr->areaID = areaID;
    gGameStatusPtr->mapID = mapID;
    gGameStatusPtr->entryID = entry;
    set_map_transition_effect(0);
    set_game_mode(GAME_MODE_UNUSED);

    // Stop ambient sounds
    play_ambient_sounds(AMBIENT_SILENCE, 1);
    menu_close();
}

void title_menu_push(void) {
    struct title_menu* previous = general_heap_malloc(sizeof(struct title_menu));
    memcpy(previous, &titleMenu, sizeof(struct title_menu));

    titleMenu.title = "Untitled menu";
    titleMenu.selectedIndex = 0;
    titleMenu.scrollY = 0;
    titleMenu.targetScrollY = 0;
    titleMenu.items = NULL;
    titleMenu.previous = previous;
}

void title_menu_pop(void) {
    struct title_menu* previous = titleMenu.previous;

    if (previous == NULL) {
        return;
    }

    // Free the items
    if (titleMenu.items != NULL) {
        general_heap_free(titleMenu.items);
    }

    // Restore the previous menu
    memcpy(&titleMenu, previous, sizeof(struct title_menu));
    general_heap_free(previous);
}

u8 dx_ascii_char_to_msg(u8 in) {
    switch (in) {
        case '\0': return MSG_CHAR_READ_END;
        case ' ': return MSG_CHAR_READ_SPACE;
        case '\n': return MSG_CHAR_READ_ENDL;
        default:
            if (in < 0x20) {
                return MSG_CHAR_NOTE;
            }
            return in - 0x20;
    }
}

u8* dx_string_to_msg(u8* msg, const u8* str) {
    while (*str) {
        *msg++ = dx_ascii_char_to_msg(*str++);
    }

    *msg = MSG_CHAR_READ_END;
    return msg;
}

void title_menu_draw_contents(void* arg0, s32 baseX, s32 baseY, s32 width, s32 height, s32 opacity, s32 darkening) {
    char msgbuf[0x100];
    s32 x = baseX + 6;
    s32 y = baseY + 6;
    struct title_menu_item* item = titleMenu.items;

    gDPSetScissor(gMainGfxPos++, G_SC_NON_INTERLACE, baseX, baseY, baseX+width, baseY+height);

    while (item && item->name != NULL) {
        s32 isSelected = item == &titleMenu.items[titleMenu.selectedIndex];

        dx_string_to_msg(msgbuf, item->name);
        draw_msg((s32)msgbuf, x, y - titleMenu.scrollY, 255, isSelected, 0);
        y += 16;

        if (isSelected) {
            // Scroll to us if we're not visible
            if (y - titleMenu.scrollY > baseY + height - 16) {
                titleMenu.targetScrollY = y - height + 16;
            }
            if (y - titleMenu.scrollY < baseY + 16) {
                titleMenu.targetScrollY = y - 16;
            }

            // Handle pick
            if (gGameStatus.pressedButtons[0] & BUTTON_A) {
                if (item->callback) item->callback(item->callbackArg);
            }
        }

        item++;
    }
}

EvtScript ShowCreditsMessage = {
    EVT_SET(GF_TitleStringDisplayed, TRUE)
    EVT_CALL(ShowMessageAtScreenPos, MSG_Space_CreditsA, 160, 40)
    EVT_CALL(ShowMessageAtScreenPos, MSG_Space_CreditsB, 160, 40)
    EVT_CALL(ShowMessageAtScreenPos, MSG_Space_CreditsC, 160, 40)
    EVT_SET(GF_TitleStringDisplayed, FALSE)
    EVT_SET(GF_CreditsSeen, TRUE)
    EVT_RETURN
    EVT_END
};

EvtScript ShowCreditsNotif = {
    EVT_SET(GF_TitleStringDisplayed, TRUE)
    EVT_CALL(ShowMessageAtScreenPos, MSG_Space_CreditsNotif, 160, 40)
    EVT_SET(GF_TitleStringDisplayed, FALSE)
    EVT_SET(GF_CreditsSeen, TRUE)
    EVT_RETURN
    EVT_END
};

EvtScript EVS_SettingsExplainSpeed = {
    EVT_SET(GF_TitleStringDisplayed, TRUE)
    EVT_CALL(ShowMessageAtScreenPos, MSG_Space_SettingsExplain_Speed, 160, 40)
    EVT_SET(GF_TitleStringDisplayed, FALSE)
    EVT_RETURN
    EVT_END
};

// Callback for the "New Game" option
void title_menu_cb_newgame(void* arg) {
    if (!evt_get_variable(NULL, GF_CreditsSeen)) {
        script_credits_notif = start_script(&ShowCreditsNotif, 0xA, 0);
    } else {
        if (evt_get_variable(NULL, GF_JrTroopaDefeated)) {
            menu_gotomap("spc_03", 0);
        } else {
            menu_gotomap("spc_05", 0);
        }
    }
}

void settings_set_speed(void* arg) {
    sfx_play_sound(0x05);
    evt_set_variable(NULL, GB_Settings_ShipSpeed, arg);
}

void settings_explain_speed(void* arg) {
    script_settings_explain_speed = start_script(&EVS_SettingsExplainSpeed, 0xA, 0);
}

void title_menu_settings_speed(void* arg) {
    struct title_menu_item* items;

    items = general_heap_malloc((3 + 1) * sizeof(struct title_menu_item));

    items[0].name = "Slow (PJ64)";
    items[0].callback = settings_set_speed;
    items[0].callbackArg = 8;

    items[1].name = "Fast (Other)";
    items[1].callback = settings_set_speed;
    items[1].callbackArg = 6;

    items[2].name = "(Explain)";
    items[2].callback = settings_explain_speed;
    items[2].callbackArg = NULL;

    items[3].name = NULL;
    items[3].callback = NULL;
    items[3].callbackArg = NULL;

    // Push new menu
    title_menu_push();
    titleMenu.title = "";
    titleMenu.items = items;

}

// Callback for the "Settings" option
void title_menu_cb_settings(void* arg) {
    const s32 settingsCount = 2;
    
    struct title_menu_item* items;


    // Allocate the items list with space for the terminator
    items = general_heap_malloc((settingsCount + 1) * sizeof(struct title_menu_item));
    if (items == NULL) {
        return;
    }

    items[0].name = "Ship speed";
    items[0].callback = title_menu_settings_speed;
    items[0].callbackArg = NULL;

    items[1].name = NULL;
    items[1].callback = NULL;
    items[1].callbackArg = NULL;

    // Push new menu
    title_menu_push();
    titleMenu.title = "";
    titleMenu.items = items;
}

void title_menu_cb_credits(void* arg) {
    script_credits = start_script(&ShowCreditsMessage, 0xF, 0);
}

void draw_koopa_icon(void* arg0, s32 baseX, s32 baseY, s32 width, s32 height, s32 opacity, s32 darkening) {
    hud_element_set_flags(koopaIcon, HUD_ELEMENT_FLAG_80 | HUD_ELEMENT_FLAG_FRONTUI);
    hud_element_set_render_pos(koopaIcon, baseX + 25, baseY + 25);
    hud_element_draw_clipped(koopaIcon);
}

void draw_huff_icon(void* arg0, s32 baseX, s32 baseY, s32 width, s32 height, s32 opacity, s32 darkening) {
    hud_element_set_flags(huffIcon, HUD_ELEMENT_FLAG_80 | HUD_ELEMENT_FLAG_FRONTUI);
    hud_element_set_render_pos(huffIcon, baseX + 25, baseY + 25);
    hud_element_draw_clipped(huffIcon);
}

void draw_hammer_icon(void* arg0, s32 baseX, s32 baseY, s32 width, s32 height, s32 opacity, s32 darkening) {
    hud_element_set_flags(hammerIcon, HUD_ELEMENT_FLAG_80 | HUD_ELEMENT_FLAG_FRONTUI);
    hud_element_set_render_pos(hammerIcon, baseX + 25, baseY + 25);
    hud_element_draw_clipped(hammerIcon);
}

void draw_whale_icon(void* arg0, s32 baseX, s32 baseY, s32 width, s32 height, s32 opacity, s32 darkening) {
    hud_element_set_flags(whaleIcon, HUD_ELEMENT_FLAG_80 | HUD_ELEMENT_FLAG_FRONTUI);
    hud_element_set_render_pos(whaleIcon, baseX + 25, baseY + 25);
    hud_element_draw_clipped(whaleIcon);
}

void init_character_icons(void) {
    koopaIcon = hud_element_create(&HES_Item_Apple);
    huffIcon = hud_element_create(&HES_Item_ApplePie);
    hammerIcon = hud_element_create(&HES_Item_AllorNothing);
    whaleIcon = hud_element_create(&HES_Item_Artifact);
}

void render_character_select(void) {
    const s32 MAX_ROWS = 2;
    const s32 MAX_COLS = 2;
    s32 xStart = 110;
    s32 yStart = 110;
    s32 width = 50;
    s32 height = 50;
    s32 row;
    s32 col;
    s32 enemyCounter = 0;
    WindowStyle ws;

    if (evt_get_variable(NULL, GB_BossesDefeated) == 5) {
        menu_gotomap("spc_10", 0);
        return;
    }
    if (evt_get_variable(NULL, GB_BossesDefeated) == 6) {
        menu_gotomap("kpa_63", 1);
        return;
    }
    
    draw_msg(displayMessageID, 20, 20, 255, 0, 0);
    draw_msg(MSG_Space_CharacterSelect, 65, 80, 255, 0, 0);   

    for (row = 0; row < MAX_ROWS; row++)
    {
        for (col = 0; col < MAX_COLS; col++)
        {
            csPanels[row][col].cleared = evt_get_variable(NULL, GameFlag(enemyCounter + 12));
            enemyCounter++;
            if ((row == csSelectedRow && col == csSelectedCol)) {
                if (selectionDelay > 0) {
                    ws = (WindowStyle) WINDOW_STYLE_13;
                    selectionDelay--;
                } else {
                    ws = (WindowStyle) WINDOW_STYLE_11;
                }
                
            } else {
                ws = (WindowStyle) ((csPanels[row][col].cleared) ? WINDOW_STYLE_20 : WINDOW_STYLE_9);
            }
            draw_box(0, ws, xStart + (col * width), yStart + (row * height), 0, width, height, 255, 0, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, csPanels[row][col].drawCallback, NULL, NULL, SCREEN_WIDTH, SCREEN_HEIGHT, NULL); 
        }
    }

    // Handle selection movement
    if (gGameStatus.heldButtons[0] & BUTTON_STICK_UP) {
        csSelectedRow--;
        if (csSelectedRow < 0) {
            csSelectedRow = 0;
        }
    }
    if (gGameStatus.heldButtons[0] & BUTTON_STICK_DOWN) {
        csSelectedRow++;
        if (csSelectedRow == MAX_ROWS) {
            csSelectedRow--;
        }
    }
    if (gGameStatus.heldButtons[0] & BUTTON_STICK_LEFT) {
        csSelectedCol--;
        if (csSelectedCol < 0) {
            csSelectedCol = 0;
        }
    }
    if (gGameStatus.heldButtons[0] & BUTTON_STICK_RIGHT) {
        csSelectedCol++;
        if (csSelectedCol == MAX_COLS) {
            csSelectedCol--;
        }
    }
    if (gGameStatus.pressedButtons[0] & BUTTON_A) {
        selectionDelay = 8;
        if (!(csPanels[csSelectedRow][csSelectedCol].cleared)) {
            menu_gotomap(csPanels[csSelectedRow][csSelectedCol].destMap, 0);
        } else {
            sfx_play_sound(0x21D);
        }
    }
}

void render_title_menu(void) {
    s32 x = 200;
    s32 y = 142;
    s32 width = 130;
    s32 height = 150;
    char msgbuf[0x100];

    if (evt_get_variable(script_credits, GF_TitleStringDisplayed)) {
        return;
    }

    // Handle selection movement
    if (gGameStatus.heldButtons[0] & BUTTON_STICK_UP) {
        titleMenu.selectedIndex--;
        if (titleMenu.selectedIndex < 0) {
            titleMenu.selectedIndex = 0;
        }
    }
    if (gGameStatus.heldButtons[0] & BUTTON_STICK_DOWN) {
        titleMenu.selectedIndex++;
        if (titleMenu.items && titleMenu.items[titleMenu.selectedIndex].name == NULL) {
            titleMenu.selectedIndex--;
        }
    }
    if (gGameStatus.pressedButtons[0] & BUTTON_B) {
        if (titleMenu.previous) {
            title_menu_pop();
        }
    }

    // Animate scroll position
    titleMenu.scrollY -= (titleMenu.scrollY - titleMenu.targetScrollY) / 6;

    title_menu_draw_contents(NULL, x, y, width, height, 255, 0);


    dx_string_to_msg(msgbuf, titleMenu.title);
    draw_msg((s32)msgbuf, x + 20, y - 12, 255, 0, 0);
}

void menu_retry(s32 prevMapID) {
    char* baseMap;

    if (prevMapID < 10) {
        sprintf(baseMap, "spc_0%d", prevMapID);
    } else {
        sprintf(baseMap, "spc_%d", prevMapID);
    }
    menu_gotomap(baseMap, 0);
}

void render_game_over() {
    s32 x = 100;
    s32 y = 100;
    s16 destAreaID;
    s16 destMapID;
    s32 prevMapID = evt_get_variable(NULL, GB_GameOverEntry);
    s32 enemyDefeated = evt_get_variable(NULL, GameFlag(prevMapID+6));

    if (enemyDefeated) {
        evt_set_variable(NULL, GameFlag(prevMapID+6), FALSE);
        evt_set_variable(NULL, GB_BossesDefeated, (evt_get_variable(NULL, GB_BossesDefeated) - 1));
    }

    gvOpacity += 5;
    if (gvOpacity > 255) {
        gvOpacity = 255;

        if (gGameStatus.pressedButtons[0] & BUTTON_A) {
            gPlayerData.curHP = 5;
            menu_retry(prevMapID);
            gvOpacity = 0;
        }
        if (gGameStatus.pressedButtons[0] & BUTTON_B && evt_get_variable(NULL, GF_JrTroopaDefeated)) {
            gPlayerData.curHP = 5;
            menu_gotomap("spc_03", 0);
            gvOpacity = 0;
        }
        if (gGameStatus.pressedButtons[0] & BUTTON_Z) {
            gPlayerData.curHP = 5;
            menu_gotomap("spc_01", 0);
            gvOpacity = 0;
        }
    }
    if (evt_get_variable(NULL, GF_JrTroopaDefeated)) {
        draw_msg(MSG_Space_Retry, x, y, gvOpacity, 0, 0);
    } else {
        draw_msg(MSG_Space_Retry_NoCharacterSelect, x, y, gvOpacity, 0, 0);
    }
}

void render_story(void) {
    draw_msg(MSG_Space_IntroStoryTitle, storyX + 75, storyY, 255, 0, 0);
    draw_msg(MSG_Space_IntroStorySubtitle, storyX + 25, storyY + 30, 255, 0, 0);
    draw_msg(MSG_Space_IntroStory, storyX, storyY + 60, 255, 0, 0);
    shiftDelay--;
    if (shiftDelay <= 0) {
        shiftDelay = 3;
        storyY--;
    }

    if (evt_get_variable(NULL, GF_IntroSeen) && skipOpacity > 0) {
        draw_msg(MSG_Space_SkipOpening, 240, 25, skipOpacity, 0, 0);
        if (storyY < 130) {
            skipOpacity -= 2;
        }
    }
    
    if (storyY < -567) {
        menu_gotomap("mac_05", 1);
    }

    if (gGameStatus.pressedButtons[0] & BUTTON_START) {
        menu_gotomap("spc_01", 0);
        evt_set_variable(NULL, GF_IntroSeen, TRUE);
    }
}

void render_game_menus(void) {
    switch (menuType) {
        case MENU_TYPE_TITLE:
            render_title_menu();
            break;
        case MENU_TYPE_CHARACTER_SELECT:
            if (iconState == ICON_INIT) {
                init_character_icons();
                iconState = ICON_UPDATE;
            }
            render_character_select();
            break;
        case MENU_TYPE_GAME_OVER:
            render_game_over();
            break;
        case MENU_TYPE_STORY:
            render_story();
            break;
        default:
            break;
    }
}

API_CALLABLE(SetMenuType) {
    Bytecode* args = script->ptrReadPos;
    s32 type = *args++;
    menuType = type;
    return ApiStatus_DONE2;
}

API_CALLABLE(Menu_SetCharacterMessage) {
    displayMessageID = evt_get_variable(script, LVar2);
    return ApiStatus_DONE2;
}
