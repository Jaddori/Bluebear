#if !defined(BLUEUTIL_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Tunder $
   $Notice: (C) Copyright 2014 by SpaceCat, Inc. All Rights Reserved. $
   ======================================================================== */

#include <stdint.h>

#define BL_KILOBYTES(n) (n*1024)
#define BL_MEGABYTES(n) (n*BL_KILOBYTES(1024))
#define BL_GIGABYTES(n) (n*BL_MEGABYTES(1024))

typedef int32_t bool32_t;

struct blMemory
{
    uint64_t size;
    void* pointer;
};

#define BLUEUTIL_H
#endif
