#if !defined(BLUECONTROLLER_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Tunder $
   $Notice: (C) Copyright 2014 by SpaceCat, Inc. All Rights Reserved. $
   ======================================================================== */

#include <windows.h>
#include <Xinput.h>
#include "blueutil.h"

enum
{
    blPlayerOne = 0,
    blPlayerTwo,
    blPlayerThree,
    blPlayerFour,
    BL_MAX_CONTROLLERS
};


enum
{
    BL_CONTROLLER_DPAD_UP = XINPUT_GAMEPAD_DPAD_UP,
    BL_CONTROLLER_DPAD_DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
    BL_CONTROLLER_DPAD_LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
    BL_CONTROLLER_DPAD_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,
    BL_CONTROLLER_START = XINPUT_GAMEPAD_START,
    BL_CONTROLLER_BACK = XINPUT_GAMEPAD_BACK,
    BL_CONTROLLER_LEFT_THUMB = XINPUT_GAMEPAD_LEFT_THUMB,
    BL_CONTROLLER_RIGHT_THUMB = XINPUT_GAMEPAD_RIGHT_THUMB,
    BL_CONTROLLER_LEFT_SHOULDER = XINPUT_GAMEPAD_LEFT_SHOULDER,
    BL_CONTROLLER_RIGHT_SHOULDER = XINPUT_GAMEPAD_RIGHT_SHOULDER,
    BL_CONTROLLER_A = XINPUT_GAMEPAD_A,
    BL_CONTROLLER_B = XINPUT_GAMEPAD_B,
    BL_CONTROLLER_X = XINPUT_GAMEPAD_X,
    BL_CONTROLLER_Y = XINPUT_GAMEPAD_Y,
};

enum
{
    blTriggerLeft = 0,
    blTriggerRight,
};

struct blController
{
    XINPUT_STATE curState;
    XINPUT_STATE prevState;
    bool32_t connected;
};

bool32_t blControllerUpdate( blController *controller, int playerIndex );
void blLeftStick( blController *controller, float *x, float *y );
void blRightStick( blController *controller, float *x, float *y );
void blSticks( blController *controller, float *result );
float blTrigger( blController *controller, int trigger );
bool32_t blTriggerDown( blController *controller, int trigger );
bool32_t blTriggerUp( blController *controller, int trigger );
bool32_t blTriggerPressed( blController *controller, int trigger );
bool32_t blTriggerReleased( blController *controller, int trigger );
bool32_t blControllerButtonDown( blController *controller, int button );
bool32_t blControllerButtonUp( blController *controller, int button );
bool32_t blControllerButtonPressed( blController *controller, int button );
bool32_t blControllerButtonReleased( blController *controller, int button );

#define BLUECONTROLLER_H
#endif
