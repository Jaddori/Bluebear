#if !defined(BLUEPNG_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Tunder $
   $Notice: (C) Copyright 2014 by SpaceCat, Inc. All Rights Reserved. $
   ======================================================================== */

#include "blueutil.h"
#include <fstream>
#include "png.h"

struct blImage
{
    uint8_t *pixels;
    int32_t size, width, height;
};

struct blPNGIOPTR
{
    uint8_t *data;
    int size, offset;
};

void blMemImage( blImage *image, void *memory );
bool32_t blMemPNG( blImage *image, void *memory, int size );
bool32_t blReadPNG( const char *filename, blImage *image );
bool32_t blWritePNG( const char *filename, blImage *image );

#define BLUEPNG_H
#endif
