#include "gv_01.h"

EntryList N(Entrances) = {
    [gv_01_ENTRY_0]     {    0.0,    -26.0,    0.0,   90.0 },
    [gv_01_ENTRY_1]     {    0.0,    -26.0,    0.0,   90.0 },
    [gv_01_ENTRY_2]     {    0.0,    -26.0,    0.0,   90.0 },
    [gv_01_ENTRY_3]     {    0.0,    -26.0,    0.0,   90.0 },
    [gv_01_ENTRY_4]     {    0.0,    -26.0,    0.0,   90.0 },
    [gv_01_ENTRY_5]     {    0.0,    -26.0,    0.0,   90.0 },
    [gv_01_ENTRY_6]     {    0.0,    -26.0,    0.0,   90.0 },
    [gv_01_ENTRY_7]     {    0.0,    -26.0,    0.0,   90.0 },
    [gv_01_ENTRY_8]     {    0.0,    -26.0,    0.0,   90.0 },
    [gv_01_ENTRY_9]     {    0.0,    -26.0,    0.0,   90.0 },
    [gv_01_ENTRY_10]     {    0.0,    -26.0,    0.0,   90.0 },
};

MapSettings N(settings) = {
    .main = &N(EVS_Main),
    .entryList = &N(Entrances),
    .entryCount = ENTRY_COUNT(N(Entrances)),
};
