/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Tunder $
   $Notice: (C) Copyright 2014 by SpaceCat, Inc. All Rights Reserved. $
   ======================================================================== */

#include "bluepng.h"

void blMemImage( blImage *image, void *memory )
{
    int32_t *ptr = (int32_t*)memory;
    
    image->width = *ptr++;
    image->height = *ptr++;
    image->pixels = (uint8_t*)ptr;
}

static void blPNGReadFunc( png_structp pPNG, png_bytep pData, png_size_t length )
{
    blPNGIOPTR *io = (blPNGIOPTR*)png_get_io_ptr( pPNG );

    if( io->offset + length <= io->size )
    {
        memcpy( pData, io->data+io->offset, length );
        io->offset += length;
    }
    else
        png_error( pPNG, "Buffer overflow." );
}

bool32_t blMemPNG( blImage *image, void *memory, int size )
{
    bool32_t result = 0;

    image->width = image->height = 0;

    png_structp pPNG = png_create_read_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0 );
    png_infop pInfo = NULL;

    if( pPNG )
    {
        pInfo = png_create_info_struct( pPNG );
        if( pInfo )
        {
            if( !setjmp( png_jmpbuf( pPNG ) ) )
            {
                blPNGIOPTR io = { (uint8_t*)memory, size, 0 };
                png_set_read_fn( pPNG, &io, blPNGReadFunc );
                png_read_info( pPNG, pInfo );

                png_uint_32 width, height;
                int bitDepth, colorType, interlaceType;

                png_get_IHDR( pPNG, pInfo, &width, &height, &bitDepth, &colorType, &interlaceType, NULL, NULL );
                png_set_strip_16( pPNG );

                if( colorType == PNG_COLOR_TYPE_PALETTE )
                    png_set_palette_to_rgb( pPNG );
                if( colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8 )
                    png_set_expand_gray_1_2_4_to_8( pPNG );
                if( png_get_valid( pPNG, pInfo, PNG_INFO_tRNS ) )
                    png_set_tRNS_to_alpha( pPNG );

                int bpp = 32 / bitDepth;
                if( height * width * bpp <= (unsigned int)image->size )
                {
                    image->size = height * width * bpp;
                    image->width = width;
                    image->height = height;

                    // TODO: Allow caller to supply memory
                    png_bytep *rowPointers = (png_bytep*)malloc( height * sizeof(png_bytep) );

                    for( unsigned int i=0; i<height; i++ )
                        rowPointers[i] = (png_bytep)&image->pixels[i*width*bpp];

                    png_read_image( pPNG, rowPointers );
                    png_read_end( pPNG, pInfo );

                    free( rowPointers );
                    result = 1;
                }
            }
        }
    }
    
    return result;
}

bool32_t blReadPNG( const char *filename, blImage *image )
{
    bool32_t result = false;

    image->width = image->height = 0;

    FILE *file = fopen( filename, "rb" );
    if( file )
    {
        png_structp pPNG = png_create_read_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0 );
        png_infop pInfo = NULL;
        if( pPNG )
        {
            pInfo = png_create_info_struct( pPNG );
            if( pInfo )
            {
                if( !setjmp( png_jmpbuf( pPNG ) ) )
                {
                    png_init_io( pPNG, file );
                    png_read_info( pPNG, pInfo );

                    png_uint_32 width, height;
                    int bitDepth, colorType, interlaceType;

                    png_get_IHDR( pPNG, pInfo, &width, &height, &bitDepth, &colorType, &interlaceType, NULL, NULL );
                    png_set_strip_16( pPNG );

                    if( colorType == PNG_COLOR_TYPE_PALETTE )
                        png_set_palette_to_rgb( pPNG );
                    if( colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8 )
                        png_set_expand_gray_1_2_4_to_8( pPNG );
                    if( png_get_valid( pPNG, pInfo, PNG_INFO_tRNS ) )
                        png_set_tRNS_to_alpha( pPNG );

                    int bpp = 32 / bitDepth;
                    if( height * width * bpp <= (unsigned int)image->size )
                    {
                        image->size = height * width * bpp;
                        image->width = width;
                        image->height = height;

                        // TODO: Allow caller to supply memory
                        png_bytep *rowPointers = (png_bytep*)malloc( height * sizeof(png_bytep) );

                        for( unsigned int i=0; i<height; i++ )
                            rowPointers[i] = (png_bytep)&image->pixels[i*width*bpp];

                        png_read_image( pPNG, rowPointers );
                        png_read_end( pPNG, pInfo );

                        free( rowPointers );
                        result = 1;
                    }
                }
            }
        }

        png_destroy_read_struct( &pPNG, &pInfo, 0 );
        fclose( file );
    }

    return result;
}

bool32_t blWritePNG( const char *filename, blImage *image )
{
    bool32_t result = 0;

    FILE *file = fopen( filename, "wb" );
    if( file )
    {
        png_structp pPNG = png_create_write_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0 );
        png_infop pInfo = NULL;
        if( pPNG )
        {
            pInfo = png_create_info_struct( pPNG );
            if( pInfo )
            {
                if( !setjmp( png_jmpbuf( pPNG ) ) )
                {
                    png_init_io( pPNG, file );
                    png_set_IHDR( pPNG, pInfo, image->width, image->height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE );
                    png_write_info( pPNG, pInfo );

                    if( image->height < PNG_UINT_32_MAX/(sizeof(png_bytep)) )
                    {
                        // TODO: Allow caller to supply memory
                        png_bytep *rowPointers = (png_bytep*)malloc( image->height*sizeof(png_bytep) );
                        png_bytep pixelData = (png_bytep)image->pixels;

                        for( int i=0; i<image->height; i++ )
                            rowPointers[i] = (uint8_t*)pixelData + i*image->width*4; // bpp = 4

                        png_write_image( pPNG, rowPointers );
                        png_write_end( pPNG, pInfo );

                        free( rowPointers );
                        result = 1;
                    }
                }
            }
        }

        png_destroy_write_struct( &pPNG, &pInfo );
        fclose( file );
    }
    
    return result;
}
