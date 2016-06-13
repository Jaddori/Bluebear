#if !defined(BLUEWINDOWS_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Tunder $
   $Notice: (C) Copyright 2014 by SpaceCat, Inc. All Rights Reserved. $
   ======================================================================== */

#include "blueutil.h"

enum
{
    BL_MOUSE_LEFT_BUTTON,
    BL_MOUSE_RIGHT_BUTTON,
    BL_MOUSE_MIDDLE_BUTTON,
    BL_MOUSE_MAX_BUTTONS
};

#define BL_KEYBOARD_MAX_KEYS 128
struct blInput
{
    int8_t keys[BL_KEYBOARD_MAX_KEYS];
    int8_t prevKeys[BL_KEYBOARD_MAX_KEYS];
    int8_t buttons[BL_MOUSE_MAX_BUTTONS];
    int8_t prevButtons[BL_MOUSE_MAX_BUTTONS];
    int32_t mouseX, mouseY;
    int32_t prevMouseX, prevMouseY;
    int32_t wheel, prevWheel;
};

#ifdef _WIN32
#include <windows.h>
#include <windowsx.h>
#include <gl\GL.h>

#define BL_WINDOW_MAIN WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
#define BL_WINDOW_NORMAL_FLAGS WS_CAPTION | WS_POPUPWINDOW
#define BL_WINDOW_FULLSCREEN_FLAGS WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS

#define BL_ALLOC_FUNCTION(name) int name( blWindow* window, blMemory* memory )
#define BL_DEALLOC_FUNCTION(name) void name( blWindow* window, blMemory* memory )
#define BL_UPDATE_FUNCTION(name) int name( blWindow* window, blMemory* memory, blInput* input )
#define BL_RENDER_FUNCTION(name) void name( blWindow* window, blMemory* memory )

struct blWindow; // forward declaration
typedef BL_ALLOC_FUNCTION(blAllocFunc_t);
typedef BL_DEALLOC_FUNCTION(blDeallocFunc_t);
typedef BL_UPDATE_FUNCTION(blUpdateFunc_t);
typedef BL_RENDER_FUNCTION(blRenderFunc_t);

struct blWindow
{
    int x, y, prevX, prevY, width, height;
    int fps, memorySize;
    const char *title, *className;
    uint64_t performanceCounterFrequency;
    bool32_t fullscreen;

    HWND windowHandle;
    HDC deviceContext;
    HGLRC renderContext;

    blAllocFunc_t* AllocFunction;
    blDeallocFunc_t* DeallocFunction;
    blUpdateFunc_t* UpdateFunction;
    blRenderFunc_t* RenderFunction;
};

BL_ALLOC_FUNCTION(blAllocStub);
BL_DEALLOC_FUNCTION(blDeallocStub);
BL_UPDATE_FUNCTION(blUpdateStub);
BL_RENDER_FUNCTION(blRenderStub);

const blWindow blDefaultWindow =
{
    128, 128, 128, 128, 640, 480,
    30, BL_MEGABYTES(16),
    NULL, NULL,
    0, 0, 0, 0, 0,

    blAllocStub,
    blDeallocStub,
    blUpdateStub,
    blRenderStub,
};

uint64_t blGetClock();
float blGetSeconds( uint64_t start, uint64_t end, blWindow* window );
int blWindowRun( HINSTANCE hInstance, blWindow *window );
bool32_t blToggleFullscreen( blWindow *window );
bool32_t blKeyDown( blInput *input, char key );
bool32_t blKeyUp( blInput *input, char key );
bool32_t blKeyPressed( blInput *input, char key );
bool32_t blKeyReleased( blInput *input, char key );
void blKeysDown( blInput* input, int8_t *keys, int *max );
bool32_t blButtonDown( blInput *input, int button );
bool32_t blButtonUp( blInput *input, int button );
bool32_t blButtonPressed( blInput *input, int button );
bool32_t blButtonReleased( blInput *input, int button );
void blMousePositionDelta( blInput *input, int32_t *x, int32_t *y );
int32_t blMouseWheelDelta( blInput *input );

#else // Linux
#endif

#define BLUEWINDOWS_H
#endif
