/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Tunder $
   $Notice: (C) Copyright 2014 by SpaceCat, Inc. All Rights Reserved. $
   ======================================================================== */

#include "bluewindow.h"
#include <iostream>

static BL_ALLOC_FUNCTION(Alloc)
{
    std::cout << "Alloc function works." << std::endl;

    return 1;
}

static BL_DEALLOC_FUNCTION(Dealloc)
{
    std::cout << "Dealloc function workds." << std::endl;
}

static BL_UPDATE_FUNCTION(Update)
{
    static bool done = false;
    if( !done )
    {
        std::cout << "Update function works." << std::endl;
        done = true;
    }

    return 1;
}

static BL_RENDER_FUNCTION(Render)
{
    static bool done = false;
    if( !done )
    {
        std::cout << "Render function works." << std::endl;
        done = true;
    }
}

int BL_WINDOW_MAIN
{
    blWindow window = {};
    window.width = 640;
    window.height = 480;
    window.title = "BlueWindow test";

    window.AllocFunction = Alloc;
    window.DeallocFunction = Dealloc;
    window.UpdateFunction = Update;
    window.RenderFunction = Render;
    
    return blWindowRun( hInstance, &window );
}
