#include "common.h"
#include "model.h"

API_CALLABLE(N(UnkBackgroundFunc)) {
    if (isInitialCall) {
        script->functionTemp[0] = 20;
    }

    set_background_color_blend(0, 0, 0, script->functionTemp[0] * 10);
    script->functionTemp[0] -= 1;

    if (script->functionTemp[0] == 0) {
        set_background_color_blend(0, 0, 0, 0);
        return ApiStatus_DONE2;
    }

    return ApiStatus_BLOCK;
}
