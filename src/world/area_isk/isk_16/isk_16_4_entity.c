#include "isk_16.h"
#include "effects.h"

EvtScript N(EVS_SetupFlames) = {
    EVT_PLAY_EFFECT(EFFECT_FLAME, 1, 415, -713, 291, EVT_FLOAT(0.3), LVar0)
    EVT_PLAY_EFFECT(EFFECT_FLAME, 1, 291, -713, 415, EVT_FLOAT(0.3), LVar0)
    EVT_PLAY_EFFECT(EFFECT_FLAME, 1, 415, -843, 291, EVT_FLOAT(0.3), LVar0)
    EVT_PLAY_EFFECT(EFFECT_FLAME, 1, 291, -843, 415, EVT_FLOAT(0.3), LVar0)
    EVT_RETURN
    EVT_END
};
