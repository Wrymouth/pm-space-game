#include "fio.h"
#include "gcc/string.h"
#include "filemenu_create_instant.h"

extern MenuPanel filemenu_main_menuBP;
extern MenuPanel filemenu_yesno_menuBP;
extern MenuPanel filemenu_info_menuBP;
extern MenuPanel filemenu_createfile_menuBP;

u8 custom_filemenu_main_gridData[] = {
    0, 0, 1, 2, 2, 3, 4, 5, 6, 0, 0, 1, 2, 2, 3, 6, 6, 6, 0, 0, 1, 2, 2, 3, 6, 6, 6, 0, 0, 1, 2, 2, 3, 6, 6, 6,
#if !VERSION_PAL
    0, 0, 1, 2, 2, 3, 6, 6, 6,
#endif
};

MenuPanel custom_filemenu_main_menuBP = { .initialized   = FALSE,
                                          .col           = 0,
                                          .row           = 0,
                                          .selected      = 0,
                                          .page          = 0,
                                          .numCols       = 3,
                                          .numRows       = 3,
                                          .numPages      = 0,
                                          .gridData      = custom_filemenu_main_gridData,
                                          .fpInit        = &filemenu_main_init,
                                          .fpHandleInput = &filemenu_main_handle_input,
                                          .fpUpdate      = &filemenu_main_update,
                                          .fpCleanup     = &filemenu_main_cleanup };

MenuPanel* custom_filemenu_menus[] = {
    &custom_filemenu_main_menuBP,
};

void instant_filemenu_set_selected(MenuPanel* menu, s32 col, s32 row) {
    menu->col = col;
    menu->row = row;
    menu->selected =
        menu->gridData[(menu->page * menu->numCols * menu->numRows) + (menu->numCols * menu->row) + menu->col];
}

void write_filename(void) {
    s32   i;
    char* filename = "FISHGAME";
    char* map = "kmr_20";
    s16 areaID;
    s16 mapID;

    for (i = 0; i <= ARRAY_COUNT(filemenu_filename); i++) {
        filemenu_filename[i] = (u8)filename[i];
    }

    get_map_IDs_by_name(map, &areaID, &mapID);

    // Go there
    gGameStatusPtr->areaID = areaID;
    gGameStatusPtr->mapID = mapID;
    gGameStatusPtr->entryID = 0;
    fio_save_game(custom_filemenu_menus[0]->selected);
    sfx_play_sound(0x42);
    gSaveSlotHasData[custom_filemenu_menus[0]->selected] = TRUE;
};

void load_game_instant(void) {
    instant_filemenu_set_selected(
        custom_filemenu_menus[0], (gGameStatusPtr->saveSlot & 1) * 2, gGameStatusPtr->saveSlot >> 1
    );
    if (!(gSaveSlotHasData[custom_filemenu_menus[0]->selected])) {
        write_filename();
    }
    fio_load_game(custom_filemenu_menus[0]->selected);
    gSaveSlotMetadata[custom_filemenu_menus[0]->selected] = gSaveSlotMetadata[custom_filemenu_menus[0]->selected];

}
