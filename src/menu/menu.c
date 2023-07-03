
#include "common.h"
#include "gcc/string.h"

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
    .title = "Debug menu",
    .selectedIndex = 0,
    .scrollY = 0,
    .targetScrollY = 0,
    .items = dx_rootDebugMenuItems,
    .previous = NULL,
};

int title_menu_isVisible = FALSE;

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
    s32 x = 4;
    s32 y = 24;
    s32 width = 130;
    s32 height = 150;
    char msgbuf[0x100];

    // Don't render if the menu isn't visible
    if (!title_menu_isVisible) {
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

// Callack for when you pick a map on the "Go to map" menu
// Actually loads the map
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

// Callback for when you pick an area on the "Go to map" menu
// Lists the maps in that area
void title_menu_cb_gotomap_area(void* arg) {
    AreaConfig* area = arg;
    struct title_menu_item* items;
    s32 i;

    items = general_heap_malloc((area->mapCount + 1) * sizeof(struct title_menu_item));
    if (items == NULL) {
        return;
    }

    for (i = 0; i < area->mapCount; i++) {
        if (area->maps[i].id == NULL) {
            continue;
        }
        items[i].name = area->maps[i].id;
        items[i].callback = title_menu_cb_startgame;
        items[i].callbackArg = &area->maps[i];
    }

    items[i].name = NULL;
    items[i].callback = NULL;

    title_menu_push();
    dx_debugMenu.title = "Go to map";
    dx_debugMenu.items = items;
}

// Callback for the "New Game" option
// Lists the areas
void title_menu_cb_newgame(void* arg) {
    title_menu_cb_startgame("spc_00");
}

void title_menu_cb_continue(void* arg) {
    recover_hp(-1);
    recover_fp(-1);
}

void title_menu_cb_increasejump(void* args);
void title_menu_cb_decreasejump(void* args);

void title_menu_cb_increasehammer(void* args);
void title_menu_cb_decreasehammer(void* args);

void title_menu_cb_credits(void* arg) {
    struct title_menu_item* items;

    title_menu_push();
    dx_debugMenu.title = "Equipment";
    items = general_heap_malloc(5 * sizeof(struct title_menu_item));
    dx_debugMenu.items = items;
    items[0].name = "Increase Jump";
    items[0].callback = title_menu_cb_increasejump;
    items[1].name = "Decrease Jump";
    items[1].callback = title_menu_cb_decreasejump;
    items[2].name = "Increase Hammer";
    items[2].callback = title_menu_cb_increasehammer;
    items[3].name = "Decrease Hammer";
    items[3].callback = title_menu_cb_decreasehammer;
}


void title_menu_cb_increasejump(void* args){
    if (gPlayerData.bootsLevel < 2) {
        gPlayerData.bootsLevel++;
    }
}

void title_menu_cb_decreasejump(void* args){
    if (gPlayerData.bootsLevel > 0) {
        gPlayerData.bootsLevel--;
    }
}

void title_menu_cb_increasehammer(void* args){
    if (gPlayerData.hammerLevel < 2) {
        gPlayerData.hammerLevel++;
    }
}

void title_menu_cb_decreasehammer(void* args){
    if (gPlayerData.hammerLevel > -1) {
        gPlayerData.hammerLevel--;
    }
}

API_CALLABLE(EnableTitleMenu) {
    Bytecode* args = script->ptrReadPos;
    s32 enable = *args++;
    title_menu_isVisible = enable;
    return ApiStatus_DONE2;
}

