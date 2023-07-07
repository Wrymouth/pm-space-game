
#include "common.h"
#include "gcc/string.h"
#include "message_ids.h"

void title_menu_cb_newgame(void* arg);
void title_menu_cb_continue(void* arg);
void title_menu_cb_credits(void* arg);

struct title_menu_item {
    char* name;
    void (*callback)(void* arg);
    void* callbackArg;
} dx_rootDebugMenuItems[] = {
    { "New Game", title_menu_cb_newgame },
    { "Continue", title_menu_cb_continue },
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
s32 are_credits_displayed = FALSE;
Evt *credits_script = NULL;

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

void render_title_menu(void) {
    s32 x = 200;
    s32 y = 142;
    s32 width = 130;
    s32 height = 150;
    char msgbuf[0x100];

    // Don't render if the menu isn't visible
    if (!title_menu_isVisible || evt_get_variable(credits_script, GF_Credits_Displayed)) {
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


    //     // Items box
    // draw_box(
    //     DRAW_FLAG_ANIMATED_BACKGROUND, (WindowStyle)WINDOW_STYLE_0,
    //     x, y, 0,
    //     width, height,
    //     255, 0,
    //     1.0f, 1.0f,
    //     0.0f, 0.0f, 0.0f,
    //     (void*)title_menu_draw_contents, NULL,
    //     NULL,
    //     SCREEN_WIDTH, SCREEN_HEIGHT,
    //     NULL
    // );

    title_menu_draw_contents(NULL, x, y, width, height, 255, 0);


    dx_string_to_msg(msgbuf, dx_debugMenu.title);
    draw_msg((s32)msgbuf, x + 20, y - 12, 255, 0, 0);
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

void title_menu_close(void) {
    while (dx_debugMenu.previous != NULL) {
        title_menu_pop();
    }
    title_menu_isVisible = FALSE;
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

void title_menu_cb_startgame(char* argMap) {
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

    title_menu_close();
}

// Callback for the "New Game" option
// Lists the areas
void title_menu_cb_newgame(void* arg) {
    title_menu_cb_startgame("spc_05");
}

void title_menu_cb_continue(void* arg) {
    recover_hp(-1);
    recover_fp(-1);
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

API_CALLABLE(EnableTitleMenu) {
    Bytecode* args = script->ptrReadPos;
    s32 enable = *args++;
    title_menu_isVisible = enable;
    return ApiStatus_DONE2;
}
