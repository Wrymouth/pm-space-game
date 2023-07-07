#include "common.h"
#include "world/common/atomic/TexturePan.inc.c"

EvtScript N(SetupTexturePan) = {
    EVT_CALL(SetTexPanner, Model_ScrollingBG, TEX_PANNER_0)
    EVT_THREAD
        TEX_PAN_PARAMS_ID(TEX_PANNER_0)
        TEX_PAN_PARAMS_STEP(2000,2000,2000,0)
        TEX_PAN_PARAMS_FREQ(    0,    0,    0,    0)
        TEX_PAN_PARAMS_INIT(    0,    0,    0,    0)
        EVT_EXEC(N(EVS_UpdateTexturePan))
    EVT_END_THREAD
    EVT_RETURN
    EVT_END  
};
