/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Tunder $
   $Notice: (C) Copyright 2014 by SpaceCat, Inc. All Rights Reserved. $
   ======================================================================== */

#include "bluewindow.h"

BL_ALLOC_FUNCTION(blAllocStub){return 1;}
BL_DEALLOC_FUNCTION(blDeallocStub){}
BL_UPDATE_FUNCTION(blUpdateStub){return 1;}
BL_RENDER_FUNCTION(blRenderStub){}

static bool32_t blCreateGLContext( blWindow *window )
{
    bool32_t result = 0;

    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof( PIXELFORMATDESCRIPTOR ),
        1,
        PFD_SUPPORT_OPENGL |
        PFD_DRAW_TO_WINDOW |
        PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        16,
        0, 0, 0, 0, 0, 0,
        0, 0, 0,
        0, 0, 0, 0,
        16, 0, 0,
        PFD_MAIN_PLANE,
        0, 0, 0, 0
    };

    int compatibleFormat = ChoosePixelFormat( window->deviceContext, &pfd );
    if( compatibleFormat > 0 )
    {
        SetPixelFormat( window->deviceContext, compatibleFormat, &pfd );
        window->renderContext = wglCreateContext( window->deviceContext );
        wglMakeCurrent( window->deviceContext, window->renderContext );

        result = 1;
    }
    
    return result;
}

uint64_t blGetClock()
{
    LARGE_INTEGER result;
    QueryPerformanceCounter( &result );
    return result.QuadPart;
}

float blGetSeconds( uint64_t start, uint64_t end, blWindow *window )
{
    float result = ((float)( end - start ) / (float)window->performanceCounterFrequency );
    return result;
}

LRESULT CALLBACK blWindowProcedure( HWND windowHandle, UINT msg, WPARAM w, LPARAM l )
{
    LRESULT result = 0;

    switch( msg )
    {
        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

        default:
            result = DefWindowProc( windowHandle, msg, w, l );
            break;
    }
    
    return result;
}

static bool32_t blProcessInput( MSG *msg, blInput *input )
{
    bool32_t result = 1;

    switch( msg->message )
    {
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            int index = msg->wParam;
            input->keys[index] = !( msg->lParam & ( 1 << 31 ) );
        } break;

        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
            input->buttons[BL_MOUSE_LEFT_BUTTON] = ( msg->wParam & MK_LBUTTON );
            input->buttons[BL_MOUSE_RIGHT_BUTTON] = ( msg->wParam & MK_RBUTTON );
            input->buttons[BL_MOUSE_MIDDLE_BUTTON] = ( msg->wParam & MK_MBUTTON );
            break;

        case WM_MOUSEWHEEL:
            input->wheel += GET_WHEEL_DELTA_WPARAM( msg->wParam );
            break;

        case WM_MOUSEMOVE:
            input->mouseX = GET_X_LPARAM( msg->lParam );
            input->mouseY = GET_Y_LPARAM( msg->lParam );
            break;

        default:
            result = 0;
            break;
    }

    return result;
}

static void blSyncInput( blInput *input )
{
    for( int i=0; i<BL_KEYBOARD_MAX_KEYS; i++ )
        input->prevKeys[i] = input->keys[i];

    for( int i=0; i<BL_MOUSE_MAX_BUTTONS; i++ )
        input->prevButtons[i] = input->buttons[i];

    input->prevMouseX = input->mouseX;
    input->prevMouseY = input->mouseY;
    input->prevWheel = input->wheel;
}

int blWindowRun( HINSTANCE hInstance, blWindow *window )
{
    int result = -1;

    if( window->title == NULL )
        window->title = "No title";
    if( window->className == NULL )
        window->className = "blWindowClassName";
    if( window->fps <= 0 )
        window->fps = 60;

    window->fullscreen = 0;
    window->prevX = window->prevY = 128;

    WNDCLASS windowClass = {};
    windowClass.hInstance = hInstance;
    windowClass.lpfnWndProc = blWindowProcedure;
    windowClass.lpszClassName = window->className;
    windowClass.hCursor = LoadCursor( NULL, IDC_ARROW );

    if( RegisterClass( &windowClass ) )
    {
        RECT clientArea = {};
        clientArea.right = window->width;
        clientArea.bottom = window->height;
        AdjustWindowRect( &clientArea, WS_OVERLAPPEDWINDOW, false );

        int windowWidth = clientArea.right - clientArea.left;
        int windowHeight = clientArea.bottom - clientArea.top;

        window->windowHandle = CreateWindowA( windowClass.lpszClassName,
                                              window->title, BL_WINDOW_NORMAL_FLAGS,
                                              window->x, window->y,
                                              windowWidth, windowHeight,
                                              NULL, NULL,
                                              hInstance, NULL );

        if( window->windowHandle )
        {
            window->deviceContext = GetDC( window->windowHandle );
            window->renderContext = 0;

            if( blCreateGLContext( window ) )
            {
                ShowWindow( window->windowHandle, SW_SHOW );

                // allocate memory
                blMemory memory;
                memory.size = window->memorySize;
                memory.pointer = calloc( memory.size, 1 );

                // create input container
                blInput input;

                // get clock resolution
                LARGE_INTEGER freqRes;
                QueryPerformanceFrequency( &freqRes );
                window->performanceCounterFrequency = freqRes.QuadPart;

                // set scheduler granularity
                UINT desiredSleepMS = 1;
                bool32_t sleepIsGranular = ( timeBeginPeriod( desiredSleepMS ) == TIMERR_NOERROR );

                float targetSecondsPerFrame = 1.0f / (float)window->fps;
                uint64_t lastCounter = blGetClock();

                int running = window->AllocFunction( window, &memory );
                while( running )
                {
                    blSyncInput( &input );

                    MSG message;
                    while( PeekMessage( &message, 0, 0, 0, PM_REMOVE ) )
                    {
                        if( message.message == WM_QUIT )
                            running = 0;
                        else
                        {
                            if( !blProcessInput( &message, &input ) )
                            {
                                TranslateMessage( &message );
                                DispatchMessage( &message );
                            }
                        }
                    }

                    // update
                    result = window->UpdateFunction( window, &memory, &input );
                    if( !result )
                        running = result;

                    // render
                    window->RenderFunction( window, &memory );

                    // adjust time
                    uint64_t workCounter = blGetClock();
                    float secondsElapsed = blGetSeconds( lastCounter, workCounter, window );
                    while( secondsElapsed < targetSecondsPerFrame )
                    {
                        if( sleepIsGranular )
                        {
                            DWORD sleepMS = (DWORD)( 1000.0f * ( targetSecondsPerFrame - secondsElapsed ) );
                            if( sleepMS > 0 )
                                Sleep( sleepMS );
                        }

                        while( secondsElapsed < targetSecondsPerFrame )
                            secondsElapsed = blGetSeconds( lastCounter, blGetClock(), window );
                    }
                    
                    // display frame
                    SwapBuffers( window->deviceContext );
                    lastCounter = blGetClock();
                }

                window->DeallocFunction( window, &memory );
            }
        }
    }

    return result;
}

bool32_t blToggleFullscreen( blWindow *window )
{
    bool32_t result = 0;

    if( window->fullscreen )
    {
        RECT rect = { 0, 0, window->width, window->height };
        SetWindowLongPtr( window->windowHandle, GWL_STYLE, BL_WINDOW_NORMAL_FLAGS | WS_VISIBLE );
        AdjustWindowRect( &rect, BL_WINDOW_NORMAL_FLAGS, FALSE );
        SetWindowPos( window->windowHandle, HWND_TOP, window->prevX, window->prevY, rect.right-rect.left, rect.bottom-rect.top, SWP_SHOWWINDOW );

        window->fullscreen = 0;
    }
    else
    {
        // TODO: Magic numbers
        // Query the operating system for the fullscreen dimensions of the screen
        SetWindowLongPtr( window->windowHandle, GWL_STYLE, BL_WINDOW_FULLSCREEN_FLAGS | WS_VISIBLE );
        SetWindowPos( window->windowHandle, HWND_TOP, 0, 0, 1680, 1050, SWP_SHOWWINDOW );

        window->fullscreen = 1;
    }
    
    return result;
}

bool32_t blKeyDown( blInput *input, char key )
{
    return input->keys[key];
}

bool32_t blKeyUp( blInput *input, char key )
{
    return !(input->keys[key]);
}

bool32_t blKeyPressed( blInput *input, char key )
{
    if( input->prevKeys[key] )
        return 0;
    return input->keys[key];
}

bool32_t blKeyReleased( blInput *input, char key )
{
    if( input->keys[key] )
        return 0;
    return input->prevKeys[key];
}

void blKeysDown( blInput *input, char *keys, int *max )
{
    int keysAdded = 0;
    for( int i=0; i<BL_KEYBOARD_MAX_KEYS && keysAdded < *max; i++ )
    {
        if( input->keys[i] )
        {
            keys[keysAdded++] = i;
        }
    }
}

bool32_t blButtonDown( blInput *input, int button )
{
    return input->buttons[button];
}

bool32_t blButtonUp( blInput *input, int button )
{
    return !(input->buttons[button]);
}

bool32_t blButtonPressed( blInput *input, int button )
{
    if( input->prevButtons[button] )
        return 0;
    return input->buttons[button];
}

bool32_t blButtonReleased( blInput *input, int button )
{
    if( input->buttons[button] )
        return 0;
    return input->buttons[button];
}

void blMousePositionDelta( blInput *input, int32_t *x, int32_t *y )
{
    *x = input->mouseX - input->prevMouseX;
    *y = input->mouseY - input->prevMouseY;
}

int32_t blMouseWheelDelta( blInput *input )
{
    return ( input->wheel - input->prevWheel );
}
