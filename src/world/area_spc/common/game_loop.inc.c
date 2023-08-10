#include "common.h"
#include "sprite/player.h"
#include "world/action/enemy_bullet.h"
#include "world/action/bullet.h"
#include "world/action/damage_system.h"
#include "effects.h"
#include "message_ids.h"
#include "fio.h"

API_CALLABLE(N(SaveAndContinue)) {
    u16 curArea  = gGameStatusPtr->areaID;
    u16 curMap   = gGameStatusPtr->mapID;
    u16 curEntry = gGameStatusPtr->entryID;
    // save with map being spc_01, entry 0
    u16 areaID, mapID;

    get_map_IDs_by_name("spc_02", &areaID, &mapID);
    gGameStatusPtr->areaID  = areaID;
    gGameStatusPtr->mapID   = mapID;
    gGameStatusPtr->entryID = 0;
    fio_save_game(gGameStatusPtr->saveSlot);
    // and continue
    gGameStatusPtr->areaID  = curArea;
    gGameStatusPtr->mapID   = curMap;
    gGameStatusPtr->entryID = curEntry;
    return ApiStatus_DONE2;
}

EvtScript N(EVS_SaveAndContinue_Prompt) = {
    EVT_WAIT(10)
    EVT_CALL(ShowMessageAtScreenPos, MSG_Menus_SavePrompt, 160, 40)
    EVT_CALL(ShowChoice, MSG_Choice_0004, 160, 40)
    EVT_IF_EQ(LVar0, 0)
        EVT_CALL(N(SaveAndContinue))
        EVT_WAIT(10)
    EVT_END_IF
    EVT_CALL(CloseMessage)
    EVT_CALL(GotoMapSpecial, EVT_PTR("spc_03"), 0, TRANSITION_AFTER_SAVE_PROMPT) 
    EVT_WAIT(100)
    EVT_RETURN
    EVT_END
};

API_CALLABLE(N(CheckBulletDamage)) {
    if (test_enemy_bullet_first_strike()) {
        player_take_damage(1);
    }
    return ApiStatus_DONE2;
}

API_CALLABLE(N(SetEnemyHPBar)) {
    gPlayerData.maxStarPower = 4;
    gPlayerData.specialBarsFilled = 0x0400;
    return ApiStatus_DONE2;
}

API_CALLABLE(N(EnableSpaceShipMode)) {
    Bytecode* args = script->ptrReadPos;
    s32 enable = *args++;
    if (enable) {
        gPlayerStatus.flags |=  PS_FLAG_IN_SPACESHIP;
    } else {
        gPlayerStatus.flags &= ~PS_FLAG_IN_SPACESHIP;
    }
    return ApiStatus_DONE2;
}

API_CALLABLE(N(CheckStickInput)) {
    Bytecode* args = script->ptrReadPos;
    s8 joystickX = gGameStatusPtr->stickX[0];
    s8 joystickY = gGameStatusPtr->stickY[0];
    evt_set_variable(script, *args++, joystickX);
    evt_set_variable(script, *args++, joystickY);
    return ApiStatus_DONE2;
};

API_CALLABLE(N(SetInvFrames)) {
    Bytecode* args = script->ptrReadPos;
    s32 blink;

    if (gPlayerStatus.invFrames >= PLAYER_INV_FRAMES) {
        gPlayerStatus.flags &= ~PS_FLAG_HAZARD_INVINCIBILITY;
        gPlayerStatus.invFrames = 0;
        gPlayerStatus.anim = ANIM_Mario1_Still;
    }
    if (gPlayerStatus.flags & PS_FLAG_HAZARD_INVINCIBILITY) {
        gPlayerStatus.invFrames++;
        gPlayerStatus.anim = ANIM_Mario1_Hurt;
    }
    return ApiStatus_DONE2;
}

API_CALLABLE(N(DoGameOver)) {
    goto_gameover();
    return ApiStatus_DONE2;
}

API_CALLABLE(N(HealPlayer)) {
    gPlayerData.curHP = gPlayerData.curMaxHP;
    return ApiStatus_DONE2;
}

EvtScript N(Die) = {
    EVT_CALL(RandInt, 25, LVarA)
    EVT_IF_EQ(LVarA, 1)
        EVT_CALL(PlaySound, SOUND_SHY_GUY_SCREAMS1)
    EVT_END_IF
    EVT_SET(LVar0, 7)
    EVT_SET(LVar1, -7)
    EVT_SET(LVarC, 0)
    EVT_CALL(N(EnableSpaceShipMode), FALSE)
    EVT_CALL(SetMusicTrack, 0, SONG_PEACH_CAUGHT, 0, 8)
    EVT_LOOP(120)
        EVT_ADD(MV_ShipPosX, LVar0)
        EVT_ADD(MV_ShipPosY, LVar1)
        EVT_CALL(TranslateGroup, Model_Spaceship, MV_ShipPosX, MV_ShipPosY, 0)
        EVT_SET(LVar4, MV_ShipPosX)
        EVT_SUB(LVar4, 12) 
        EVT_SET(LVar5, MV_ShipPosY)
        EVT_ADD(LVar5, 20) 
        EVT_CALL(SetPlayerPos, LVar4, LVar5, 12)
        EVT_IF_EQ(LVarC, 12)
            EVT_SET(LVarC, 0)
            EVT_CALL(GetPlayerPos, LVarD, LVarE, LVarF)
            EVT_IF_NE(LVarA, 1)
                EVT_CALL(PlaySound, 0x2076)
            EVT_END_IF
            EVT_ADD(LVarD, 12)
            EVT_CALL(PlayEffect, EFFECT_EXPLOSION, 1, LVarD, LVarE, LVarF)
        EVT_END_IF
        EVT_ADD(LVarC, 1)
        EVT_WAIT(1)
    EVT_END_LOOP
    EVT_CALL(N(DoGameOver))
    EVT_RETURN
    EVT_END
};

EvtScript N(Win) = {
    EVT_IF_TRUE(GF_BowserDefeated)
        EVT_WAIT(20)
        EVT_CALL(SetMusicTrack, 0, SONG_PEACH_QUIZ_INTRO, 0, 8)
    EVT_ELSE
        EVT_CALL(SetMusicTrack, 0, SONG_BATTLE_END, 0, 8)
    EVT_END_IF
    EVT_CALL(SetPlayerAnimation, ANIM_MarioB1_Hammer3_FingerWag)
    EVT_WAIT(20)
    EVT_CALL(MakeLerp, 0, 500, 90, EASING_CUBIC_IN)
    EVT_THREAD
        EVT_WAIT(15)
        EVT_CALL(N(HealPlayer))
    EVT_END_THREAD
    EVT_LOOP(90)
        EVT_CALL(UpdateLerp)
        EVT_ADD(MV_ShipPosX, LVar0)
        EVT_CALL(TranslateGroup, Model_Spaceship, MV_ShipPosX, MV_ShipPosY, 0)
        EVT_SET(LVar4, MV_ShipPosX)
        EVT_SUB(LVar4, 12) 
        EVT_SET(LVar5, MV_ShipPosY)
        EVT_ADD(LVar5, 20) 
        EVT_CALL(SetPlayerPos, LVar4, LVar5, 12)
        EVT_WAIT(1)
    EVT_END_LOOP
    EVT_EXEC_WAIT(N(EVS_SaveAndContinue_Prompt))
    EVT_RETURN
    EVT_END
};

EvtScript N(GameLoop) = {
    EVT_IF_EQ(GB_Settings_ShipSpeed, 0)
        EVT_SET(GB_Settings_ShipSpeed, 6)
    EVT_END_IF
    EVT_CALL(DisablePlayerPhysics, TRUE)
    EVT_CALL(N(EnableSpaceShipMode), TRUE)
    EVT_CALL(N(SetEnemyHPBar))
    EVT_LOOP(0)
        EVT_IF_TRUE(AF_PlayerDead)
            EVT_EXEC_WAIT(N(Die))
            EVT_BREAK_LOOP
        EVT_END_IF
        EVT_IF_TRUE(MF_EnemyDefeated)
            EVT_EXEC_WAIT(N(Win))
            EVT_BREAK_LOOP
        EVT_END_IF
        // get stick input and process
        EVT_IF_TRUE(GF_StartedSpaceShip)
            EVT_CALL(GetPlayerPos, LVar4, LVar5, LVar6)
            EVT_CALL(N(CheckStickInput), LVar0, LVar1)
            EVT_DIV(LVar0, GB_Settings_ShipSpeed)
            EVT_DIV(LVar1, GB_Settings_ShipSpeed)
            // check position against map borders
            EVT_IF_LT(LVar4, MapXLeft)
                EVT_IF_LT(LVar0, 0)
                    EVT_SET(LVar0, 0)
                EVT_END_IF
            EVT_END_IF
            EVT_IF_GT(LVar4, MapXRight)
                EVT_IF_GT(LVar0, 0)
                    EVT_SET(LVar0, 0)
                EVT_END_IF
            EVT_END_IF
            EVT_IF_LT(LVar5, MapYBottom)
                EVT_IF_LT(LVar1, 0)
                    EVT_SET(LVar1, 0)
                EVT_END_IF
            EVT_END_IF
            EVT_IF_GT(LVar5, MapYTop)
                EVT_IF_GT(LVar1, 0)
                    EVT_SET(LVar1, 0)
                EVT_END_IF
            EVT_END_IF
            // move ship
            EVT_ADD(MV_ShipPosX, LVar0)
            EVT_ADD(MV_ShipPosY, LVar1)
            EVT_CALL(TranslateGroup, Model_Spaceship, MV_ShipPosX, MV_ShipPosY, 0)
            EVT_SET(LVar4, MV_ShipPosX)
            EVT_SUB(LVar4, 12) 
            EVT_SET(LVar5, MV_ShipPosY)
            EVT_ADD(LVar5, 20) 
            EVT_CALL(SetPlayerPos, LVar4, LVar5, 12)
        EVT_END_IF
        // handle player damage
        EVT_CALL(N(CheckBulletDamage))
        EVT_CALL(N(SetInvFrames))
        EVT_WAIT(1)
    EVT_END_LOOP
    EVT_RETURN
    EVT_END
};
