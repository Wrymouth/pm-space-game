#include "common.h"
#include "sprite/player.h"
#include "world/action/enemy_bullet.h"
#include "world/action/damage_system.h"


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

EvtScript N(GameLoop) = {
    EVT_CALL(DisablePlayerPhysics, TRUE)
    EVT_CALL(N(EnableSpaceShipMode), TRUE)
    EVT_CALL(N(SetEnemyHPBar))
    EVT_LOOP(0)
        // get stick input and process
        EVT_CALL(GetPlayerPos, LVar4, LVar5, LVar6)
        EVT_CALL(N(CheckStickInput), LVar0, LVar1)
        EVT_DIV(LVar0, 8)
        EVT_DIV(LVar1, 8)
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
        // handle player damage
        EVT_CALL(N(CheckBulletDamage))
        EVT_CALL(N(SetInvFrames))
        EVT_WAIT(1)
    EVT_END_LOOP
    EVT_RETURN
    EVT_END
};
