/**
 * @file platform/filesystem.c
 * @brief Implementation of the platform/filesystem header.
 * (see platform/filesystem.h for additional details)
 */
#include "platform/filesystem.h"

#include "core/logger.h"
#include "platform/platform.h"

bool
file_exists
(   const char* path
,   FILE_MODE   mode
)
{
    if ( !path )
    {
        LOGERROR ( "file_exists: Missing argument: path (filepath to test)." );
        return false;
    }
    return platform_file_exists ( path , mode );
}

bool
file_open
(   const char* path
,   FILE_MODE   mode
,   file_t*     file
)
{
    if ( !file )
    {
        LOGERROR ( "file_open: Missing argument: file (output buffer)." );
        return false;
    }

    file->valid = false;
    file->handle = 0;

    if ( !path )
    {
        LOGERROR ( "file_open: Missing argument: path (filepath to open)." );
        return false;
    }

    file->valid = platform_file_open ( path , mode , file );
    return file->valid;
}

void
file_close
(   file_t* file
)
{
    if ( !file )
    {
        return;
    }
    file->valid = false;
    if ( file->handle )
    {
        platform_file_close ( file );
        file->handle = 0;
    }
}

bool
file_update
(   file_t* file
)
{
    if ( !file )
    {
        LOGERROR ( "file_update: Missing argument: file." );
        return false;
    }
    if ( !file->valid || !file->handle )
    {
        return false;
    }
    file->valid = platform_file_update ( file );
    return file->valid;
}

const char*
file_path
(   file_t* file
)
{
    if ( !file )
    {
        LOGERROR ( "file_path: Missing argument: file." );
        return "";
    }
    if ( !file->handle )
    {
        return "";
    }
    return platform_file_path ( file );
}

FILE_MODE
file_mode
(   file_t* file
)
{
    if ( !file )
    {
        LOGERROR ( "file_mode: Missing argument: file." );
        return 0;
    }
    if ( !file->handle )
    {
        return 0;
    }
    return platform_file_mode ( file );
}

u64
file_size
(   file_t* file
)
{
    if ( !file )
    {
        LOGERROR ( "file_size: Missing argument: file." );
        return 0;
    }
    if ( !file->handle )
    {
        return 0;
    }
    return platform_file_size ( file );
}

u64
file_position_get
(   file_t* file
)
{
    if ( !file )
    {
        LOGERROR ( "file_position_get: Missing argument: file." );
        return 0;
    }
    if ( !file->handle )
    {
        return 0;
    }
    return platform_file_position_get ( file );
}

bool
file_position_set
(   file_t* file
,   u64     position
)
{
    if ( !file )
    {
        LOGERROR ( "file_position_set: Missing argument: file." );
        return false;
    }
    if ( !file->valid || !file->handle )
    {
        return false;
    }
    if ( position > file_size ( file ) )
    {
        LOGERROR ( "file_position_set: The provided position is outside of the file boundary." );
        return false;
    }
    file->valid = platform_file_position_set ( file , position );
    return file->valid;
}

bool
file_read
(   file_t* file
,   u64     size
,   void*   dst
,   u64*    read
)
{
    if ( !file || !dst || !read )
    {
        if ( !file ) LOGERROR ( "file_read: Missing argument: file (file to read from)." );
        if ( !dst )  LOGERROR ( "file_read: Missing argument: dst (output buffer)." );
        if ( !read ) LOGERROR ( "file_read: Missing argument: read (output buffer)." );
        else
        {
            *read = 0;
        }
        return false;
    }
    *read = 0;
    if ( !file->handle || !file->valid )
    {
        return false;
    }
    if ( !( file_mode ( file ) & FILE_MODE_READ ) )
    {
        LOGERROR ( "file_read: The provided file is not opened for reading: %s"
                 , file_path ( file )
                 );
        return false;
    }
    file->valid = platform_file_read ( file , size , dst , read );
    return file->valid;
}

bool
file_read_line
(   file_t* file
,   char**  dst
)
{
    if ( !file || !dst )
    {
        if ( !file ) LOGERROR ( "file_read_line: Missing argument: file (file to read from)." );
        if ( !dst )  LOGERROR ( "file_read_line: Missing argument: dst (output buffer)." );
        return false;
    }
    if ( !file->handle || !file->valid )
    {
        return false;
    }
    if ( !( file_mode ( file ) & FILE_MODE_READ ) )
    {
        LOGERROR ( "file_read_line: The provided file is not opened for reading: %s"
                 , file_path ( file )
                 );
        return false;
    }
    file->valid = platform_file_read_line ( file , dst );
    return file->valid;
}

bool
file_read_all
(   file_t* file
,   u8**    dst
,   u64*    read
)
{
    if ( !file || !dst || !read )
    {
        if ( !file ) LOGERROR ( "file_read_all: Missing argument: file (file to read from)." );
        if ( !dst )  LOGERROR ( "file_read_all: Missing argument: dst (output buffer)." );
        if ( !read ) LOGERROR ( "file_read_all: Missing argument: read (output buffer)." );
        else
        {
            *read = 0;
        }
        return false;
    }
    *read = 0;
    if ( !file->handle || !file->valid )
    {
        return false;
    }
    if ( !( file_mode ( file ) & FILE_MODE_READ ) )
    {
        LOGERROR ( "file_read_all: The provided file is not opened for reading: %s"
                 , file_path ( file )
                 );
        return false;
    }
    file->valid = platform_file_read_all ( file , dst , read );
    return file->valid;
}

bool
file_write
(   file_t*     file
,   u64         size
,   const void* src
,   u64*        written
)
{
    if ( !file || ( !src && size ) || !written )
    {
        if ( !file )        LOGERROR ( "file_write: Missing argument: file (file to write to)." );
        if ( !src && size ) LOGERROR ( "file_write: Missing argument: src (content to write)." );
        if ( !written )     LOGERROR ( "file_write: Missing argument: written (output buffer)." );
        else
        {
            *written = 0;
        }
        return false;
    }
    *written = 0;
    if ( !file->handle || !file->valid )
    {
        return false;
    }
    if ( !( file_mode ( file ) & FILE_MODE_WRITE ) )
    {
        LOGERROR ( "file_write: The provided file is not opened for writing: %s"
                 , file_path ( file )
                 );
        return false;
    }
    file->valid = platform_file_write ( file , size , src , written );
    return file->valid;
}

bool
file_write_line
(   file_t*     file
,   u64         size
,   const char* src
)
{
    if ( !file || ( !src && size ) )
    {
        if ( !file )        LOGERROR ( "file_write_line: Missing argument: file (file to write to)." );
        if ( !src && size ) LOGERROR ( "file_write_line: Missing argument: src (content to write)." );
        return false;
    }
    if ( !file->handle || !file->valid )
    {
        return false;
    }
    if ( !( file_mode ( file ) & FILE_MODE_WRITE ) )
    {
        LOGERROR ( "file_write_line: The provided file is not opened for writing: %s"
                 , file_path ( file )
                 );
        return false;
    }
    file->valid = platform_file_write_line ( file , size , src );
    return file->valid;
}

void
file_stdin
(   file_t* file
)
{
    platform_file_stdin ( file );
}

void
file_stdout
(   file_t* file
)
{
    platform_file_stdout ( file );
}

void
file_stderr
(   file_t* file
)
{
    platform_file_stderr ( file );
}