
#include "common.h"
#include "gcc/string.h"
#include "message_ids.h"
#include "menu.h"

void title_menu_cb_newgame(void* arg);
void title_menu_cb_credits(void* arg);


struct title_menu_item {
    char* name;
    void (*callback)(void* arg);
    void* callbackArg;
} dx_rootDebugMenuItems[] = {
    { "Start Game", title_menu_cb_newgame },
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
} dx_debugMenu = {
    .title = "",
    .selectedIndex = 0,
    .scrollY = 0,
    .targetScrollY = 0,
    .items = dx_rootDebugMenuItems,
    .previous = NULL,
};

Evt *credits_script = NULL;
Evt *credits_notif_script = NULL;

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
CharacterSelectPanel csPanels[2][2] = {{{"spc_06", FALSE}, {"spc_07", FALSE}}, {{"spc_08", FALSE}, {"spc_09", FALSE}}}; // rows, columns

// game over
s32 gvOpacity = 0;



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
    struct title_menu_item* item = dx_debugMenu.items;

    gDPSetScissor(gMainGfxPos++, G_SC_NON_INTERLACE, baseX, baseY, baseX+width, baseY+height);

    while (item && item->name != NULL) {
        s32 isSelected = item == &dx_debugMenu.items[dx_debugMenu.selectedIndex];

        dx_string_to_msg(msgbuf, item->name);
        draw_msg((s32)msgbuf, x, y - dx_debugMenu.scrollY, 255, isSelected, 0);
        y += 16;

        if (isSelected) {
            // Scroll to us if we're not visible
            if (y - dx_debugMenu.scrollY > baseY + height - 16) {
                dx_debugMenu.targetScrollY = y - height + 16;
            }
            if (y - dx_debugMenu.scrollY < baseY + 16) {
                dx_debugMenu.targetScrollY = y - 16;
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
    EVT_SET(GF_CreditsDisplayed, TRUE)
    EVT_CALL(ShowMessageAtScreenPos, MSG_Space_Credits, 160, 40)
    EVT_SET(GF_CreditsDisplayed, FALSE)
    EVT_SET(GF_CreditsSeen, TRUE)
    EVT_RETURN
    EVT_END
};

EvtScript ShowCreditsNotif = {
    EVT_SET(GF_CreditsDisplayed, TRUE)
    EVT_CALL(ShowMessageAtScreenPos, MSG_Space_CreditsNotif, 160, 40)
    EVT_SET(GF_CreditsDisplayed, FALSE)
    EVT_RETURN
    EVT_END
};

void title_menu_cb_credits(void* arg) {
    credits_script = start_script(&ShowCreditsMessage, 0xF, 0);
}

void menu_close(void) {
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

// Callback for the "New Game" option
// Lists the areas
void title_menu_cb_newgame(void* arg) {
    if (!evt_get_variable(NULL, GF_CreditsSeen)) {
        credits_notif_script = start_script(&ShowCreditsNotif, 0xA, 0);
    } else {
        menu_gotomap("spc_03", 0);
    }
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

    load_font(1);
    draw_msg(MSG_Space_CharacterSelect, 95, 95, 255, 0, 0);   

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
            draw_box(0, ws, xStart + (col * width), yStart + (row * height), 0, width, height, 255, 0, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, NULL, NULL, NULL, SCREEN_WIDTH, SCREEN_HEIGHT, NULL); 
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

    if (evt_get_variable(credits_script, GF_CreditsDisplayed)) {
        return;
    }

    // Handle selection movement
    if (gGameStatus.heldButtons[0] & BUTTON_STICK_UP) {
        dx_debugMenu.selectedIndex--;
        if (dx_debugMenu.selectedIndex < 0) {
            dx_debugMenu.selectedIndex = 0;
        }
    }
    if (gGameStatus.heldButtons[0] & BUTTON_STICK_DOWN) {
        dx_debugMenu.selectedIndex++;
        if (dx_debugMenu.items && dx_debugMenu.items[dx_debugMenu.selectedIndex].name == NULL) {
            dx_debugMenu.selectedIndex--;
        }
    }

    // Animate scroll position
    dx_debugMenu.scrollY -= (dx_debugMenu.scrollY - dx_debugMenu.targetScrollY) / 6;

    title_menu_draw_contents(NULL, x, y, width, height, 255, 0);


    dx_string_to_msg(msgbuf, dx_debugMenu.title);
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
        if (gGameStatus.pressedButtons[0] & BUTTON_B) {
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

    draw_msg(MSG_Space_Retry, x, y, gvOpacity, 0, 0);
}

void render_story(void) {
    draw_msg(MSG_Space_IntroStoryTitle, storyX + 90, storyY, 255, 0, 0);
    draw_msg(MSG_Space_IntroStory, storyX, storyY + 50, 255, 0, 0);
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
