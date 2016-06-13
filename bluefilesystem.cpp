/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Tunder $
   $Notice: (C) Copyright 2014 by SpaceCat, Inc. All Rights Reserved. $
   ======================================================================== */

#include "bluefilesystem.h"

#ifdef _WIN32

#define UINT(high,low) ( ( (uint64_t)high << 32 ) | ( low ) )
#define UINT_TIME(time) UINT(time.dwHighDateTime,time.dwLowDateTime)

bool32_t blGetFileInfo( const char *file, blFileInfo *info )
{
    bool32_t result = 0;

    WIN32_FIND_DATA findData;
    HANDLE fileHandle = FindFirstFile( file, &findData );
    if( fileHandle != INVALID_HANDLE_VALUE )
    {
        FindClose( fileHandle );

        info->creationTime = UINT_TIME( findData.ftCreationTime );
        info->lastAccessTime = UINT_TIME( findData.ftLastAccessTime );
        info->lastWriteTime = UINT_TIME( findData.ftLastWriteTime );
        info->size = UINT( findData.nFileSizeHigh, findData.nFileSizeLow );

        result = 1;
    }
    
    return result;
}

bool32_t blFileModified( const char *file, blFileInfo *info )
{
    bool32_t result = 0;

    blFileInfo curInfo;
    if( blGetFileInfo( file, &curInfo ) )
    {
        //result = ( info->lastWriteTime != curInfo.lastWriteTime );
        result = ( info->lastWriteTime - curInfo.lastWriteTime );
        *info = curInfo;
    }
    
    return result;
}

uint64_t blGetFileSize( const char *file )
{
    uint64_t result = 0;

    HANDLE fileHandle = CreateFile( file,
                                    GENERIC_READ,
                                    FILE_SHARE_READ,
                                    NULL,
                                    OPEN_EXISTING,
                                    FILE_ATTRIBUTE_NORMAL,
                                    NULL );

    if( fileHandle != INVALID_HANDLE_VALUE )
    {
        LARGE_INTEGER size;
        if( GetFileSizeEx( fileHandle, &size ) )
        {
            result = (uint64_t)size.QuadPart;
        }

        CloseHandle( fileHandle );
    }

    return result;
}

bool32_t blGetFiles( const char *directory, blFile *files, int *max )
{
    bool32_t result = 0;

    WIN32_FIND_DATA findData;
    HANDLE fileHandle = FindFirstFile( directory, &findData );

    int i;
    for( i=0; i<*max && fileHandle != INVALID_HANDLE_VALUE; i++ )
    {
        files[i].namelen = strlen( findData.cFileName );
        if( files[i].namelen > BL_FILE_NAME_MAX_LEN )
            files[i].namelen = BL_FILE_NAME_MAX_LEN;

        strncpy( files[i].name, findData.cFileName, files[i].namelen );
        files[i].name[BL_FILE_NAME_MAX_LEN-1] = 0;
    }

    *max = i;
    FindClose( fileHandle );
    
    return result;
}

bool32_t blReadFile( const char *file, void *memory, uint64_t *max )
{
    bool32_t result = 0;

    HANDLE fileHandle = CreateFile( file,
                                    GENERIC_READ,
                                    FILE_SHARE_READ,
                                    NULL,
                                    OPEN_EXISTING,
                                    FILE_ATTRIBUTE_NORMAL,
                                    NULL );
    if( fileHandle != INVALID_HANDLE_VALUE )
    {
        LARGE_INTEGER size;
        if( GetFileSizeEx( fileHandle, &size ) )
        {
            uint64_t fileSize = size.QuadPart;
            if( fileSize < *max )
            {
                DWORD bytesRead;
                if( ReadFile( fileHandle, memory, (DWORD)fileSize, &bytesRead, NULL ) && bytesRead == (DWORD)fileSize )
                {
                    *max = fileSize;
                    result = 1;
                }
            }
        }
    }

    return result;
}

bool32_t blReadFromFile( const char *file, void *memory, int len, int offset )
{
    bool32_t result = 0;

    HANDLE fileHandle = CreateFile( file,
                                    GENERIC_READ,
                                    FILE_SHARE_READ,
                                    NULL,
                                    OPEN_EXISTING,
                                    FILE_ATTRIBUTE_NORMAL,
                                    NULL );
    if( fileHandle != INVALID_HANDLE_VALUE )
    {
        result = 1;

        DWORD bytesRead;
        if( offset > 0 )
        {
            if( !ReadFile( fileHandle, memory, offset, &bytesRead, 0 ) )
                result = ( bytesRead == offset );
        }

        if( result )
        {
            if( ReadFile( fileHandle, memory, len, &bytesRead, 0 ) )
                result = ( bytesRead == len );
        }
    }
    
    return result;
}

bool32_t blWriteFile( const char *file, blMemory *memory )
{
    bool32_t result = 0;

    HANDLE fileHandle = CreateFile( file,
                                    GENERIC_WRITE,
                                    0,
                                    NULL,
                                    CREATE_ALWAYS,
                                    FILE_ATTRIBUTE_NORMAL,
                                    NULL );
    if( fileHandle != INVALID_HANDLE_VALUE )
    {
        DWORD bytesWritten;
        result = ( WriteFile( fileHandle, memory->pointer, (DWORD)memory->size, &bytesWritten, NULL ) && bytesWritten == memory->size );
    }
    
    return result;
}

#else // Linux

#endif
