#ifndef _WORLD_AREA_SPC_SPC_H_
#define _WORLD_AREA_SPC_SPC_H_

enum {
    AB_SPC_Unused_0     = AreaByte(0),
    AB_SPC_Unused_1     = AreaByte(1),
    AB_SPC_Unused_2     = AreaByte(2),
    AB_SPC_Unused_3     = AreaByte(3),
    AB_SPC_Unused_4     = AreaByte(4),
    AB_SPC_Unused_5     = AreaByte(5),
    AB_SPC_Unused_6     = AreaByte(6),
    AB_SPC_Unused_7     = AreaByte(7),
    AB_SPC_Unused_8     = AreaByte(8),
    AB_SPC_Unused_9     = AreaByte(9),
    AB_SPC_Unused_A     = AreaByte(10),
    AB_SPC_Unused_B     = AreaByte(11),
    AB_SPC_Unused_C     = AreaByte(12),
    AB_SPC_Unused_D     = AreaByte(13),
    AB_SPC_Unused_E     = AreaByte(14),
    AB_SPC_Unused_F     = AreaByte(15),
};

enum {
    AF_Dead = AreaFlag(0)
};

enum {
    MapXLeft = -300,
    MapXRight = 300,
    MapYBottom = -200,
    MapYTop = 150,
};

extern MapSettings spc_00_settings;
extern MapSettings spc_01_settings;
extern MapSettings spc_02_settings;
extern MapSettings spc_03_settings;
extern MapSettings spc_04_settings;
extern MapSettings spc_05_settings;
extern MapSettings spc_06_settings;
extern MapSettings spc_07_settings;
extern MapSettings spc_08_settings;
extern MapSettings spc_09_settings;
extern MapSettings spc_10_settings;

extern s32 spc_00_map_init(void);
extern s32 spc_01_map_init(void);
extern s32 spc_02_map_init(void);
extern s32 spc_03_map_init(void);
extern s32 spc_04_map_init(void);
extern s32 spc_05_map_init(void);
extern s32 spc_06_map_init(void);
extern s32 spc_07_map_init(void);
extern s32 spc_08_map_init(void);
extern s32 spc_09_map_init(void);
extern s32 spc_10_map_init(void);

#endif
