typedef enum MenuType {
    MENU_TYPE_NONE             = 0,
    MENU_TYPE_TITLE            = 1,
    MENU_TYPE_GAME_OVER        = 2,
    MENU_TYPE_CHARACTER_SELECT = 3,
} MenuType;

ApiStatus SetMenuType(Evt* script, s32 isInitialCall);
void title_menu_push(void);
void title_menu_pop(void);
void menu_close(void);
void render_game_menus(void);
void render_title_menu(void);
