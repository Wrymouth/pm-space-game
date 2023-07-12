
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

int title_menu_isVisible = FALSE;
int character_select_isVisible = FALSE;
Evt *credits_script = NULL;

MenuType menuType = MENU_TYPE_NONE;

s32 csSelectedRow = 0;
s32 csSelectedCol = 0;
s32 selectionDelay = 3;

u8* destMaps[2][2] = {{"spc_06", "spc_07"}, {"spc_08", "spc_09"}};

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
    EVT_SET(GF_Credits_Displayed, TRUE)
    EVT_CALL(ShowMessageAtScreenPos, MSG_Space_Credits, 160, 40)
    EVT_SET(GF_Credits_Displayed, FALSE)
    EVT_RETURN
    EVT_END
};

void title_menu_cb_credits(void* arg) {
    credits_script = start_script(&ShowCreditsMessage, 0xF, 0);
}

void menu_close(void) {
    menuType = MENU_TYPE_NONE;
}

void title_menu_pop(void) {
    struct title_menu* previous = dx_debugMenu.previous;

    if (previous == NULL) {
        return;
    }

    // Free the items
    if (dx_debugMenu.items != NULL) {
        general_heap_free(dx_debugMenu.items);
    }

    // Restore the previous menu
    memcpy(&dx_debugMenu, previous, sizeof(struct title_menu));
    general_heap_free(previous);
}

void menu_gotomap(char* argMap) {
    char* map = argMap;

    // Get area/map id pair
    s16 areaID;
    s16 mapID;
    get_map_IDs_by_name(map, &areaID, &mapID);

    // Go there
    gGameStatusPtr->areaID = areaID;
    gGameStatusPtr->mapID = mapID;
    gGameStatusPtr->entryID = 0;
    set_map_transition_effect(0);
    set_game_mode(GAME_MODE_UNUSED);

    // Stop ambient sounds
    play_ambient_sounds(AMBIENT_SILENCE, 1);
    menu_close();
}

// Callback for the "New Game" option
// Lists the areas
void title_menu_cb_newgame(void* arg) {
    menu_gotomap("spc_03");
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
    WindowStyle ws;

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
        menu_gotomap(destMaps[csSelectedCol][csSelectedRow]);
    }

    load_font(1);
    draw_msg(MSG_Space_CharacterSelect, 95, 95, 255, 0, 0);   

    for (row = 0; row < MAX_ROWS; row++)
    {
        for (col = 0; col < MAX_COLS; col++)
        {
            if (selectionDelay > 0) {
                ws = (WindowStyle) ((row == csSelectedRow && col == csSelectedCol) ? WINDOW_STYLE_13 : WINDOW_STYLE_9);
                selectionDelay--;
            } else {
                ws = (WindowStyle) ((row == csSelectedRow && col == csSelectedCol) ? WINDOW_STYLE_11 : WINDOW_STYLE_9);
            }
            draw_box(0, ws, xStart + (col * width), yStart + (row * height), 0, width, height, 255, 0, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, NULL, NULL, NULL, SCREEN_WIDTH, SCREEN_HEIGHT, NULL); 
        }
    }
}

void render_title_menu(void) {
    s32 x = 200;
    s32 y = 142;
    s32 width = 130;
    s32 height = 150;
    char msgbuf[0x100];

    if (evt_get_variable(credits_script, GF_Credits_Displayed)) {
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

void render_game_menus(void) {
    switch (menuType) {
        case MENU_TYPE_TITLE:
            render_title_menu();
            break;
        case MENU_TYPE_CHARACTER_SELECT:
            render_character_select();
            break;
        default:
            break;
    }
}

void title_menu_push(void) {
    struct title_menu* previous = general_heap_malloc(sizeof(struct title_menu));
    memcpy(previous, &dx_debugMenu, sizeof(struct title_menu));

    dx_debugMenu.title = "Untitled menu";
    dx_debugMenu.selectedIndex = 0;
    dx_debugMenu.scrollY = 0;
    dx_debugMenu.targetScrollY = 0;
    dx_debugMenu.items = NULL;
    dx_debugMenu.previous = previous;
}





API_CALLABLE(SetMenuType) {
    Bytecode* args = script->ptrReadPos;
    s32 type = *args++;
    menuType = type;
    return ApiStatus_DONE2;
}
