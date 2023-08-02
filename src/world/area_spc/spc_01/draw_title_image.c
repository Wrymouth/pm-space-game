#include "spc_01.h"

typedef struct TitleImageDataFile {
    /* 0x00 */ s32 img_offset_title;
    /* 0x04 */ s32 img_offset_copyright;
    /* 0x08 */ s32 img_offset_press_start;
    /* 0x0C */ unsigned char unk_0C[4];
    // end of header
    /* 0x10 */ s8 data[VLA];
} TitleImageDataFile; // size may vary

static TitleImageDataFile* N(TitleData);
static IMG_PTR N(TitleImage);

s32 N(TitlePrimAlpha) = 0;
s32 N(TitlePosY) = 106;

Gfx N(Gfx_TexSetup_TitleImage)[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPSetRenderMode(G_RM_XLU_SURF, G_RM_XLU_SURF2),
    gsDPSetCombineLERP(0, 0, 0, TEXEL0, TEXEL1, 0, PRIMITIVE, 0, 0, 0, 0, TEXEL0, TEXEL1, 0, PRIMITIVE, 0),
    gsDPSetTextureFilter(G_TF_POINT),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetColorDither(G_CD_DISABLE),
    gsDPSetAlphaDither(G_AD_DISABLE),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetCombineKey(G_CK_NONE),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsDPNoOp(),
    gsSPEndDisplayList(),
};


void N(worker_render_title_image)(void) {
    s32 i;

    gSPDisplayList(gMainGfxPos++, N(Gfx_TexSetup_TitleImage));
    gDPSetPrimColor(gMainGfxPos++, 0, 0, 0, 0, 0, N(TitlePrimAlpha));
    gDPPipeSync(gMainGfxPos++);

    for (i = 0; i < 56; i++) {
        gDPLoadTextureTile(gMainGfxPos++, &N(TitleImage)[1600 * i], G_IM_FMT_RGBA, G_IM_SIZ_32b, 200, 112,
                           0, 0, 199, 1, 0,
                           G_TX_WRAP, G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
        gSPTextureRectangle(gMainGfxPos++,
            /* ulx */ 60 * 4,
            /* uly */ (i * 2 + N(TitlePosY)) * 4,
            /* lrx */ 260 * 4,
            /* lry */ ((i * 2 + 2) + N(TitlePosY)) * 4,
            G_TX_RENDERTILE, 0, 0, 1024, 1024);
    }
    gDPPipeSync(gMainGfxPos++);
}

API_CALLABLE(N(LoadTitleImage)) {
    u32 assetSize;
    void* compressed = load_asset_by_name("title_data", &assetSize);
    N(TitleData) = (TitleImageDataFile*) heap_malloc(assetSize);

    decode_yay0(compressed, N(TitleData));
    general_heap_free(compressed);
    N(TitleImage) = (IMG_PTR)(N(TitleData)->img_offset_title + (s32)N(TitleData));
    create_worker_frontUI(NULL, N(worker_render_title_image));
    return ApiStatus_DONE2;
}

API_CALLABLE(N(SetTitlePrimAlpha)) {
    N(TitlePrimAlpha) = script->varTable[0];
    return ApiStatus_DONE2;
}

API_CALLABLE(N(SetTitlePosY)) {
    N(TitlePosY) = script->varTable[0];
    return ApiStatus_DONE2;
}

EvtScript N(ShowTitle) = {
    EVT_CALL(N(LoadTitleImage))
    EVT_SET(LVar0, 20)
    EVT_CALL(N(SetTitlePosY))
    EVT_SET(LVar0, -1)
    EVT_CALL(N(SetTitlePrimAlpha))
    EVT_LOOP(0)
        EVT_IF_TRUE(GF_TitleStringDisplayed)
            EVT_SET(LVar0, -150)
            EVT_CALL(N(SetTitlePosY))
        EVT_ELSE
            EVT_SET(LVar0, 20)
            EVT_CALL(N(SetTitlePosY))
        EVT_END_IF
        EVT_WAIT(1)
    EVT_END_LOOP
    EVT_RETURN
    EVT_END
};
