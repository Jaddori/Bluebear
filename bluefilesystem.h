#if !defined(BLUEFILESYSTEM_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Tunder $
   $Notice: (C) Copyright 2014 by SpaceCat, Inc. All Rights Reserved. $
   ======================================================================== */

#include "blueutil.h"

#ifdef _WIN32
#include <windows.h>

struct blFileInfo
{
    uint64_t creationTime;
    uint64_t lastAccessTime;
    uint64_t lastWriteTime;
    uint64_t size;
};

#define BL_FILE_NAME_MAX_LEN 127
struct blFile
{
    uint8_t namelen;
    char name[BL_FILE_NAME_MAX_LEN];
};

bool32_t blGetFileInfo( const char *file, blFileInfo *info );
bool32_t blFileModified( const char *file, blFileInfo *info );
uint64_t blGetFileSize( const char *file );

bool32_t blGetFiles( const char *directory, blFile *files, int *max );
bool32_t blReadFile( const char *file, void *memory, uint64_t *max );
bool32_t blReadFromFile( const char *file, void *memory, int len, int offset = 0 );
bool32_t blWriteFile( const char *file, blMemory *memory );

#else // Linux

#endif

#define BLUEFILESYSTEM_H
#endif
