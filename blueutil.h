#if !defined(BLUEUTIL_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Tunder $
   $Notice: (C) Copyright 2014 by SpaceCat, Inc. All Rights Reserved. $
   ======================================================================== */

#include <stdint.h>

typedef int32_t bool32_t;

struct blMemory
{
    uint64_t size;
    void* pointer;
};

#define BLUEUTIL_H
#endif
