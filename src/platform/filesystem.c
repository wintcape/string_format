/**
 * @file platform/filesystem.c
 * @brief Implementation of the platform/filesystem header.
 * (see platform/filesystem.h for additional details)
 */
#include "platform/filesystem.h"
#include "platform/platform.h"

bool
file_exists
(   const char* path
,   FILE_MODE   mode
)
{
    return platform_file_exists ( path , mode );
}

bool
file_open
(   const char* path
,   FILE_MODE   mode
,   file_t*     file
)
{
    return platform_file_open ( path , mode , file );
}

void
file_close
(   file_t* file
)
{
    platform_file_close ( file );
}

bool
file_update
(   file_t* file
)
{
    return platform_file_update ( file );
}

const char*
file_path
(   file_t* file
)
{
    return platform_file_path ( file );
}

FILE_MODE
file_mode
(   file_t* file
)
{
    return platform_file_mode ( file );
}

u64
file_size
(   file_t* file
)
{
    return platform_file_size ( file );
}

u64
file_position_get
(   file_t* file
)
{
    return platform_file_position_get ( file );
}

bool
file_position_set
(   file_t* file
,   u64     position
)
{
    return platform_file_position_set ( file , position );
}

bool
file_read
(   file_t* file
,   u64     size
,   void*   dst
,   u64*    read
)
{
    return platform_file_read ( file , size , dst , read );
}

bool
file_read_line
(   file_t* file
,   char**  dst
)
{
    return platform_file_read_line ( file , dst );
}

bool
file_read_all
(   file_t* file
,   u8**    dst
,   u64*    read
)
{
    return platform_file_read_all ( file , dst , read );
}

bool
file_write
(   file_t*     file
,   u64         size
,   const void* src
,   u64*        written
)
{
    return platform_file_write ( file , size , src , written );
}

bool
file_write_line
(   file_t*     file
,   u64         size
,   const char* src
)
{
    return platform_file_write_line ( file , size , src );
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