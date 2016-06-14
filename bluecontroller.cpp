/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Tunder $
   $Notice: (C) Copyright 2014 by SpaceCat, Inc. All Rights Reserved. $
   ======================================================================== */

#include "bluecontroller.h"



bool32_t blControllerUpdate( blController *controller, int playerIndex )
{
    controller->connected = 0;
    controller->prevState = controller->curState;
    memset( &controller->curState, 0, sizeof(XINPUT_STATE) );
    DWORD connected = XInputGetState( playerIndex, &controller->curState );

    if( connected == ERROR_SUCCESS )
    {
        controller->connected = 1;

        // trunc left thumbstick
        if( ( controller->curState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && controller->curState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ) &&
            ( controller->curState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && controller->curState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ) )
        {
            controller->curState.Gamepad.sThumbLX = 0;
            controller->curState.Gamepad.sThumbLY = 0;
        }

        // trunc left thumbstick
        if( ( controller->curState.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && controller->curState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) &&
            ( controller->curState.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && controller->curState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) )
        {
            controller->curState.Gamepad.sThumbRX = 0;
            controller->curState.Gamepad.sThumbRY = 0;
        }
    }

    return controller->connected;
}

inline float blNormByte( BYTE b )
{
    return ( b / 255.0f );
}

inline float blNormShort( SHORT s )
{
    return ( s / 65535.0f );
}

void blLeftStick( blController *controller, float *x, float *y )
{
    *x = blNormShort( controller->curState.Gamepad.sThumbLX );
    *y = blNormShort( controller->curState.Gamepad.sThumbLY );
}

void blRightStick( blController *controller, float *x, float *y )
{
    *x = blNormShort( controller->curState.Gamepad.sThumbRX );
    *y = blNormShort( controller->curState.Gamepad.sThumbRY );
}

void blSticks( blController *controller, float *result )
{
    blLeftStick( controller, result++, result++ );
    blRightStick( controller, result++, result );
}

float blTrigger( blController *controller, int trigger )
{
    return blNormByte( trigger == blTriggerLeft ?
                       controller->curState.Gamepad.bLeftTrigger :
                       controller->curState.Gamepad.bRightTrigger );
}

bool32_t blTriggerDown( blController *controller, int trigger, float threshold )
{
    return ( blTrigger( controller, trigger ) > threshold );
}

bool32_t blTriggerUp( blController *controller, int trigger, float threshold )
{
    return ( blTrigger( controller, trigger ) < threshold );
}

bool32_t blTriggerPressed( blController *controller, int trigger, float threshold )
{
    return ( trigger == blTriggerLeft ?
             ( blNormByte( controller->curState.Gamepad.bLeftTrigger ) > threshold &&
               blNormByte( controller->prevState.Gamepad.bLeftTrigger ) < threshold ) :
             ( blNormByte( controller->curState.Gamepad.bRightTrigger ) > threshold &&
               blNormByte( controller->prevState.Gamepad.bRightTrigger ) < threshold ) );
}

bool32_t blTriggerReleased( blController *controller, int trigger, float threshold )
{
    return ( trigger == blTriggerLeft ?
             ( blNormByte( controller->curState.Gamepad.bLeftTrigger ) < threshold &&
               blNormByte( controller->prevState.Gamepad.bLeftTrigger ) > threshold ) :
             ( blNormByte( controller->curState.Gamepad.bRightTrigger ) < threshold &&
               blNormByte( controller->prevState.Gamepad.bRightTrigger ) > threshold ) );
}

bool32_t blControllerButtonDown( blController *controller, int button )
{
    return ( controller->curState.Gamepad.wButtons & button );
}

bool32_t blControllerButtonUp( blController *controller, int button )
{
    return ( controller->curState.Gamepad.wButtons & button == 0 );
}

bool32_t blControllerButtonPressed( blController *controller, int button )
{
    return ( ( controller->curState.Gamepad.wButtons & button ) > 0 &&
             ( controller->prevState.Gamepad.wButtons & button ) == 0 );
}

bool32_t blControllerButtonReleased( blController *controller, int button )
{
    return ( ( controller->curState.Gamepad.wButtons & button ) == 0 &&
             ( controller->prevState.Gamepad.wButtons & button ) > 0 );
}
