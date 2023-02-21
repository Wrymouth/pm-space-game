#include "common.h"
#include "battle/battle.h"
#include "mapfs/kmr_bt05_shape.h"

#define NAMESPACE b_area_kmr_part_3_kmr_05

#include "world/common/todo/clouds.inc.c"

EvtScript N(EVS_PreBattle) = {
    EVT_CALL(SetSpriteShading, SHADING_NONE)
    EVT_CALL(EnableModel, MODEL_yuka, FALSE)
    EVT_CALL(EnableModel, MODEL_o303, FALSE)
    EVT_RETURN
    EVT_END
};

EvtScript N(EVS_PostBattle) = {
    EVT_RETURN
    EVT_END
};

s32 N(ForegroundModels)[] = {
    MODEL_n2,
    MODEL_m4,
    STAGE_MODEL_LIST_END
};

Stage NAMESPACE = {
    .texture = "kmr_tex",
    .shape = "kmr_bt05_shape",
    .hit = "kmr_bt05_hit",
    .bg = "kmr_bg",
    .preBattle = &N(EVS_PreBattle),
    .postBattle = &N(EVS_PostBattle),
    .foregroundModelList = N(ForegroundModels),
};