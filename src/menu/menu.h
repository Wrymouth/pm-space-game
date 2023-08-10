#ifndef SPACE_MENU
#define SPACE_MENU

enum Difficulty {
    DIFFICULTY_STANDARD = 0,
    DIFFICULTY_EASY = 1,
    DIFFICULTY_HARDCORE = 2,
};

typedef enum MenuType {
    MENU_TYPE_NONE             = 0,
    MENU_TYPE_TITLE            = 1,
    MENU_TYPE_GAME_OVER        = 2,
    MENU_TYPE_CHARACTER_SELECT = 3,
    MENU_TYPE_STORY            = 4,
} MenuType;

typedef enum MenuIconState {
    ICON_INIT,
    ICON_UPDATE,
} MenuIconState;

typedef struct CharacterSelectPanel {
    u8* destMap;
    s8 cleared;
    void (*drawCallback)(void* arg);
} CharacterSelectPanel;

extern s32 csSelectedRow;
extern s32 csSelectedCol;
extern CharacterSelectPanel csPanels[2][2];

ApiStatus SetMenuType(Evt* script, s32 isInitialCall);
API_CALLABLE(Menu_SetCharacterMessage);
void title_menu_push(void);
void title_menu_pop(void);
void menu_close(void);
void render_game_menus(void);
void render_title_menu(void);

#endif
