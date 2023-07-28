#include "kpa_62.h"

EntryList N(Entrances) = {
    [kpa_62_ENTRY_0]    {  1554.0,    23.0, -94.0,  180.0 },
    [kpa_62_ENTRY_1]    {  276.0,    0.0, -305.0,  180.0 },
    [kpa_62_ENTRY_2]    {  386.0,  -45.0, -231.0,  180.0 },
    [kpa_62_ENTRY_3]    {  1554.0,    23.0, -94.0,  180.0 },
};

MapSettings N(settings) = {
    .main = &N(EVS_Main),
    .entryList = &N(Entrances),
    .entryCount = ENTRY_COUNT(N(Entrances)),
    .background = 0,
    .tattle = { MSG_MapTattle_kpa_62 },
};
