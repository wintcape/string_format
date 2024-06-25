/**
 * @file platform/macos.m
 * @brief Implementation of the platform header for macOS/OSX-based
 * operating systems.
 *
 * TODO: Make this not just a copy of platform/linux.c.
 *
 * (see platform.h for additional details)
 */
#include "platform/platform.h"

////////////////////////////////////////////////////////////////////////////////
// Begin platform layer.
#if PLATFORM_MACOS == 1

#include "container/string.h"
#include "core/logger.h"
#include "platform/memory.h"
#include "math/clamp.h"

// Platform layer dependencies.
#define _FILE_OFFSET_BITS 64
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

// Standard libc dependencies.
#include <stdlib.h>
#include <string.h>

/** @brief Type definition for a platform-dependent file data structure. */
typedef struct
{
    i32         descriptor;
    const char* path;
    FILE_MODE   mode;
    u64         size;
    u64         position;
    bool        initialized;
}
platform_file_t;

// Global definitions for standard input, output, and error file streams.
static platform_file_t platform_stdin;  /** @brief Standard input stream handle. */
static platform_file_t platform_stdout; /** @brief Standard output stream handle. */
static platform_file_t platform_stderr; /** @brief Standard error stream handle. */

void*
platform_memory_allocate
(   u64 size
)
{
    return malloc ( size );
}

void
platform_memory_free
(   void* blk
)
{
    free ( blk );
}

void*
platform_memory_clear
(   void*   blk
,   u64     size
)
{
    return memset ( blk , 0 , size );
}

void*
platform_memory_set
(   void*       blk
,   const i32   value
,   u64         size
)
{
    return memset ( blk , value , size );
}

void*
platform_memory_copy
(   void*       dst
,   const void* src
,   u64         size
)
{
    return memcpy ( dst , src , size );
}

void*
platform_memory_move
(   void*       dst
,   const void* src
,   u64         size
)
{
    return memmove ( dst , src , size );
}

bool
platform_memory_equal
(   const void* s1
,   const void* s2
,   u64         size
)
{
    return !memcmp ( s1 , s2 , size );
}

u64
platform_string_length
(   const char* string
)
{
    return strlen ( string );
}

u64
platform_string_length_clamped
(   const char* string
,   u64         limit
)
{
    return strnlen ( string , limit );
}

void
platform_array_sort
(   void*                   array
,   u64                     array_length
,   u64                     array_stride
,   comparator_function_t   comparator
)
{
    qsort ( array , array_length , array_stride , comparator );
}

bool
platform_file_exists
(   const char* path
,   FILE_MODE   mode_
)
{
    if ( !path )
    {
        LOGERROR ( "platform_file_exists ("PLATFORM_STRING"): Missing argument: path (filepath to test)." );
        return false;
    }

    i32 mode;
    if ( ( mode_ & FILE_MODE_READ ) && ( mode_ & FILE_MODE_WRITE ) )
    {
        mode = R_OK | W_OK;
    }
    else if ( ( mode_ & FILE_MODE_READ ) && !( mode_ & FILE_MODE_WRITE ) )
    {
        mode = R_OK;
    }
    else if ( !( mode_ & FILE_MODE_READ ) && ( mode_ & FILE_MODE_WRITE ) )
    {
        mode = W_OK;
    }
    else
    {
        mode = F_OK;
    }

    return !access ( path , mode );
}

bool
platform_file_open
(   const char* path
,   FILE_MODE   mode_
,   file_t*     file_
)
{
    if ( !file_ )
    {
        LOGERROR ( "platform_file_open ("PLATFORM_STRING"): Missing argument: file (output buffer)." );
        return false;
    }

    file_->valid = false;
    file_->handle = 0;

    if ( !path )
    {
        LOGERROR ( "platform_file_open ("PLATFORM_STRING"): Missing argument: path (filepath to open)." );
        return false;
    }

    i32 mode;
    bool truncate;
    if ( ( mode_ & FILE_MODE_READ ) && ( mode_ & FILE_MODE_WRITE ) )
    {
        mode = O_RDWR;
        truncate = false;
    }
    else if ( ( mode_ & FILE_MODE_READ ) && !( mode_ & FILE_MODE_WRITE ) )
    {
        mode = O_RDONLY;
        truncate = false;
    }
    else if ( !( mode_ & FILE_MODE_READ ) && ( mode_ & FILE_MODE_WRITE ) )
    {
        mode = O_WRONLY;
        truncate = true;
    }
    else
    {
        LOGERROR ( "platform_file_open ("PLATFORM_STRING"): Value of mode argument was invalid; it should be a valid file mode." );
        return false;
    }

    i32 descriptor = open ( path , mode | O_CREAT , S_IRWXU );

    if ( descriptor == -1 )
    {
        platform_log_error ( "platform_file_open ("PLATFORM_STRING"): open failed on file: %s"
                           , path
                           );
        return false;
    }

    // Precompute the file size.
    struct stat file_info;
    if ( fstat ( descriptor , &file_info ) == -1 )
    {
        platform_log_error ( "platform_file_open ("PLATFORM_STRING"): fstat failed on file: %s"
                           , path
                           );
        return false;
    }

    // Set read-write position to the start of the file.
    if ( lseek ( descriptor , 0 , SEEK_SET ) == -1 )
    {
        platform_log_error ( "platform_file_open ("PLATFORM_STRING"): lseek failed on file: %s"
                           , path
                           );
        return false;
    }

    // If in write mode, truncate the file content.
    if ( truncate && file_info.st_size )
    {
        if ( ftruncate ( descriptor , 0 ) )
        {
            platform_log_error ( "platform_file_open ("PLATFORM_STRING"): ftruncate failed on file: %s"
                               , path
                               );
            return false;
        }
        file_info.st_size = 0;
    }

    platform_file_t* file = memory_allocate ( sizeof ( platform_file_t ) /* , MEMORY_TAG_FILE */ );
    file->descriptor = descriptor;
    file->path = path;
    file->mode = mode_;
    file->size = file_info.st_size;
    file->position = 0;
    file->initialized = true;
    
    file_->handle = file;
    file_->valid = true;
    return true;
}

void
platform_file_close
(   file_t* file_
)
{
    if ( !file_ || !file_->handle || !file_->valid )
    {
        return;
    }

    file_->valid = false;

    platform_file_t* file = file_->handle;
    if ( close ( file->descriptor ) == -1 )
    {
        platform_log_error ( "platform_file_close ("PLATFORM_STRING"): close failed on file: %s"
                           , file->path
                           );
    }

    memory_free ( file /* , sizeof ( platform_file_t ) , MEMORY_TAG_FILE */ );
    file_->handle = 0;
}

bool
platform_file_update
(   file_t* file_
)
{
    if ( !file_ )
    {
        LOGERROR ( "platform_file_update ("PLATFORM_STRING"): Missing argument: file." );
        return false;
    }

    if ( !file_->handle || !file_->valid )
    {
        return false;
    }

    platform_file_t* file = file_->handle;

    // Recompute file size.
    struct stat file_info;
    if ( fstat ( file->descriptor , &file_info ) == -1 )
    {
        platform_log_error ( "platform_file_update ("PLATFORM_STRING"): fstat failed on file: %s"
                           , file->path
                           );
        return false;
    }
    
    // Recompute file position.
    const off_t position = lseek ( file->descriptor , 0 , SEEK_CUR );
    if ( position == -1 )
    {
        platform_log_error ( "platform_file_update ("PLATFORM_STRING"): lseek failed on file: %s"
                           , file->path
                           );
        return false;
    }
    
    if (   file->size == ( ( u64 ) file_info.st_size )
        && file->position == ( ( u64 ) position )
       )
    {
        return false;
    }

    file->size = file_info.st_size;
    file->position = position;
    return true;
}

const char*
platform_file_path
(   file_t* file_
)
{
    if ( !file_ )
    {
        LOGERROR ( "platform_file_path ("PLATFORM_STRING"): Missing argument: file." );
        return 0;
    }

    if ( !file_->handle || !file_->valid )
    {
        return 0;
    }

    platform_file_t* file = file_->handle;
    return file->path;
}

FILE_MODE
platform_file_mode
(   file_t* file_
)
{
    if ( !file_ )
    {
        LOGERROR ( "platform_file_mode ("PLATFORM_STRING"): Missing argument: file." );
        return 0;
    }

    if ( !file_->handle || !file_->valid )
    {
        return 0;
    }

    platform_file_t* file = file_->handle;
    return file->mode;
}

u64
platform_file_size
(   file_t* file_
)
{
    if ( !file_ )
    {
        LOGERROR ( "platform_file_size ("PLATFORM_STRING"): Missing argument: file." );
        return 0;
    }

    if ( !file_->handle || !file_->valid )
    {
        return 0;
    }

    platform_file_t* file = file_->handle;
    return file->size;
}

u64
platform_file_position_get
(   file_t* file_
)
{
    if ( !file_ )
    {
        LOGERROR ( "platform_file_position_get ("PLATFORM_STRING"): Missing argument: file." );
        return 0;
    }

    if ( !file_->handle || !file_->valid )
    {
        return 0;
    }

    platform_file_t* file = file_->handle;
    return file->position;
}

bool
platform_file_position_set
(   file_t* file_
,   u64     position
)
{
    if ( !file_ )
    {
        LOGERROR ( "platform_file_position_set ("PLATFORM_STRING"): Missing argument: file." );
        return 0;
    }

    if ( !file_->handle || !file_->valid )
    {
        return 0;
    }

    platform_file_t* file = file_->handle;

    // Out-of-bounds position? Y/N
    if ( position > file->size )
    {
        LOGERROR ( "platform_file_position_set ("PLATFORM_STRING"): The provided position is outside of the file boundary." );
        return 0;
    }

    // Update host platform file position.
    if ( lseek ( file->descriptor , position , SEEK_SET ) == -1 )
    {
        platform_log_error ( "platform_file_position_set ("PLATFORM_STRING"): lseek failed on file: %s"
                           , file->path
                           );
        return false;
    }

    // Update host platform file position.
    file->position = position;
    
    return true;
}

bool
platform_file_read
(   file_t* file_
,   u64     size
,   void*   dst
,   u64*    read_
)
{
    if ( !file_ || !dst || !read_ )
    {
        if ( !file_ )
        {
            LOGERROR ( "platform_file_read ("PLATFORM_STRING"): Missing argument: file (file to read from)." );
        }
        if ( !dst )
        {
            LOGERROR ( "platform_file_read ("PLATFORM_STRING"): Missing argument: dst (output buffer)." );
        }
        if ( !read_ )
        {
            LOGERROR ( "platform_file_read ("PLATFORM_STRING"): Missing argument: read (output buffer)." );
        }
        else
        {
            *read_ = 0;
        }
        return false;
    }

    if ( !file_->handle || !file_->valid )
    {
        *read_ = 0;
        return false;
    }

    platform_file_t* file = file_->handle;

    // Illegal mode? Y/N
    if ( !( file->mode & FILE_MODE_READ ) )
    {
        LOGERROR ( "platform_file_read ("PLATFORM_STRING"): The provided file is not opened for reading: %s"
                 , file->path
                 );
        *read_ = 0;
        return false;
    }

    size = MIN ( size , file->size - file->position );
    u64 total_bytes_read = 0;
    while ( total_bytes_read < size )
    {
        const ssize_t bytes_read = read ( file->descriptor
                                        , ( ( u8* ) dst ) + total_bytes_read
                                        , size - total_bytes_read
                                        );
        if ( bytes_read == -1 )
        {
            platform_log_error ( "platform_file_read ("PLATFORM_STRING"): read failed on file: %s"
                               , file->path
                               );
            *read_ = total_bytes_read;
            return false;
        }

        // Update internal file position.
        file->position += bytes_read;

        total_bytes_read += bytes_read;
    }

    *read_ = total_bytes_read;
    return total_bytes_read == size;
}

bool
platform_file_read_line
(   file_t* file_
,   char**  dst
)
{
    if ( !file_ || !dst )
    {
        if ( !file_ )
        {
            LOGERROR ( "platform_file_read_line ("PLATFORM_STRING"): Missing argument: file (file to read from)." );
        }
        if ( !dst )
        {
            LOGERROR ( "platform_file_read_line ("PLATFORM_STRING"): Missing argument: dst (output buffer)." );
        }
        else
        {
            *dst = 0;
        }
        return false;
    }

    if ( !file_->handle || !file_->valid )
    {
        *dst = 0;
        return false;
    }

    platform_file_t* file = file_->handle;

    // Illegal mode? Y/N
    if ( !( file->mode & FILE_MODE_READ ) )
    {
        LOGERROR ( "platform_file_read_line ("PLATFORM_STRING"): The provided file is not opened for reading: %s"
                 , file->path
                 );
        *dst = 0;
        return false;
    }

    char buffer[ STACK_STRING_MAX_SIZE ];
    char* string = string_create ();

    // Nothing to copy? Y/N
    u64 bytes_remaining = file->size - file->position;
    if ( !bytes_remaining )
    {
        *dst = string;
        return true;
    }

    bool end_of_line = false;
    do
    {
        // Read file content into a buffer for processing.
        const ssize_t bytes_read = read ( file->descriptor
                                        , buffer
                                        , MIN ( bytes_remaining
                                              , STACK_STRING_MAX_SIZE
                                              ));
        if ( bytes_read == -1 )
        {
            platform_log_error ( "platform_file_read_line ("PLATFORM_STRING"): read failed on file: %s"
                               , file->path
                               );
            string_destroy ( string );
            *dst = 0;
            return false;
        }

        // Update internal file position.
        file->position += bytes_read;

        // End of line? Y/N
        u64 length = 0;
        while ( length < ( ( u64 ) bytes_read ) )
        {
            if ( !buffer[ length ] || newline ( buffer[ length ] ) )
            {
                end_of_line = true;

                // Update host platform file position.
                u64 amount = bytes_read - length - 1;
                amount = -amount;
                if ( lseek ( file->descriptor , amount , SEEK_CUR ) == -1 )
                {
                    platform_log_error ( "platform_file_read_line ("PLATFORM_STRING"): lseek failed on file: %s"
                                       , file->path
                                       );
                    string_destroy ( string );
                    *dst = 0;
                    return false;
                }

                // Update internal file position.
                file->position += amount;

                break;
            }

            length += 1;
        }

        // Append the line segment to the output buffer.
        string_append ( string , buffer , length );
        
        bytes_remaining -= bytes_read;
    }
    while ( bytes_remaining && !end_of_line );
    
    *dst = string;
    return true;
}

bool
platform_file_read_all
(   file_t* file_
,   u8**    dst
,   u64*    read_
)
{
    if ( !file_ || !dst || !read_ )
    {
        if ( !file_ )
        {
            LOGERROR ( "platform_file_read_all ("PLATFORM_STRING"): Missing argument: file (file to read from)." );
        }
        if ( !dst )
        {
            LOGERROR ( "platform_file_read_all ("PLATFORM_STRING"): Missing argument: dst (output buffer)." );
        }
        else
        {
            *dst = 0;
        }
        if ( !read_ )
        {
            LOGERROR ( "platform_file_read_all ("PLATFORM_STRING"): Missing argument: read (output buffer)." );
        }
        else
        {
            *read_ = 0;
        }
        return false;
    }

    if ( !file_->handle || !file_->valid )
    {
        *dst = 0;
        *read_ = 0;
        return false;
    }

    platform_file_t* file = file_->handle;

    // Illegal mode? Y/N
    if ( !( file->mode & FILE_MODE_READ ) )
    {
        LOGERROR ( "platform_file_read_all ("PLATFORM_STRING"): The provided file is not opened for reading: %s"
                 , file->path
                 );
        *dst = 0;
        *read_ = 0;
        return false;
    }

    u8* string = ( u8* ) string_allocate ( sizeof ( u8 ) * ( file->size + 1 ) );

    // Nothing to copy? Y/N
    if ( !file->size )
    {
        *dst = string;
        *read_ = 0;
        return true;
    }

    // Set host-platform read-write position to the start of the file.
    if ( lseek ( file->descriptor , 0 , SEEK_SET ) == -1 )
    {
        platform_log_error ( "platform_file_read_all ("PLATFORM_STRING"): lseek failed on file: %s"
                           , file->path
                           );
        string_free ( string );
        *dst = 0;
        *read_ = 0;
        return false;
    }

    // Update internal file position.
    file->position = 0;

    u64 total_bytes_read = 0;
    do
    {
        const ssize_t bytes_read = read ( file->descriptor
                                        , string + total_bytes_read
                                        , file->size - total_bytes_read
                                        );
        if ( bytes_read == -1 )
        {
            platform_log_error ( "platform_file_read_all ("PLATFORM_STRING"): read failed on file: %s"
                               , file->path
                               );
            string_free ( string );
            *dst = 0;
            *read_ = total_bytes_read;
            return false;
        }

        // Update internal file position.
        file->position += bytes_read;

        total_bytes_read += bytes_read;
    }
    while ( total_bytes_read < file->size );

    // Rewind host platform file position.
    if ( lseek ( file->descriptor , 0 , SEEK_SET ) == -1 )
    {
        platform_log_error ( "platform_file_read_all ("PLATFORM_STRING"): lseek failed on file: %s"
                           , file->path
                           );
        string_free ( string );
        *dst = 0;
        *read_ = total_bytes_read;
        return false;
    }

    // Update internal file position.
    file->position = 0;

    *dst = string;
    *read_ = total_bytes_read;
    return total_bytes_read == file->size;
}

bool
platform_file_write
(   file_t*     file_
,   u64         size
,   const void* src
,   u64*        written
)
{
    if ( !file_ || !src || !written )
    {
        if ( !file_ )
        {
            LOGERROR ( "platform_file_write ("PLATFORM_STRING"): Missing argument: file (file to write to)." );
        }
        if ( !src )
        {
            LOGERROR ( "platform_file_write ("PLATFORM_STRING"): Missing argument: src (content to write)." );
        }
        if ( !written )
        {
            LOGERROR ( "platform_file_write ("PLATFORM_STRING"): Missing argument: written (output buffer)." );
        }
        else
        {
            *written = 0;
        }
        return false;
    }

    if ( !file_->handle || !file_->valid )
    {
        *written = 0;
        return false;
    }

    platform_file_t* file = file_->handle;

    // Illegal mode? Y/N
    if ( !( file->mode & FILE_MODE_WRITE ) )
    {
        LOGERROR ( "platform_file_write ("PLATFORM_STRING"): The provided file is not opened for writing: %s"
                 , file->path
                 );
        *written = 0;
        return false;
    }
    
    u64 total_bytes_written = 0;
    while ( total_bytes_written < size )
    {
        const ssize_t bytes_written = write ( file->descriptor
                                            , ( ( u8* ) src ) + total_bytes_written
                                            , size - total_bytes_written
                                            );
        if ( bytes_written == -1 )
        {
            platform_log_error ( "platform_file_write ("PLATFORM_STRING"): write failed on file: %s"
                               , file->path
                               );
            *written = bytes_written;
            return false;
        }

        // Update internal file position and size.
        file->position += bytes_written;
        file->size += bytes_written;

        total_bytes_written += bytes_written;
    }

    *written = total_bytes_written;
    return total_bytes_written == size;
}

bool
platform_file_write_line
(   file_t*     file_
,   u64         size
,   const char* src
)
{
    if ( !file_ || !src )
    {
        if ( !file_ )
        {
            LOGERROR ( "platform_file_write_line ("PLATFORM_STRING"): Missing argument: file (file to write to)." );
        }
        if ( !src )
        {
            LOGERROR ( "platform_file_write_line ("PLATFORM_STRING"): Missing argument: src (content to write)." );
        }
        return false;
    }

    if ( !file_->handle || !file_->valid )
    {
        return false;
    }

    platform_file_t* file = file_->handle;

    // Illegal mode? Y/N
    if ( !( file->mode & FILE_MODE_WRITE ) )
    {
        LOGERROR ( "platform_file_write_line ("PLATFORM_STRING"): The provided file is not opened for writing: %s"
                 , file->path
                 );
        return false;
    }

    u64 total_bytes_written = 0;
    while ( total_bytes_written < size )
    {
        const ssize_t bytes_written = write ( file->descriptor
                                            , ( ( u8* ) src ) + total_bytes_written
                                            , size - total_bytes_written
                                            );
        if ( bytes_written == -1 )
        {
            platform_log_error ( "platform_file_write ("PLATFORM_STRING"): write failed on file: %s"
                               , file->path
                               );
            return false;
        }

        // Update internal file position and size.
        file->position += bytes_written;
        file->size += bytes_written;

        total_bytes_written += bytes_written;
    }

    // Append a newline to the file.
    const char newline = '\n';
    const ssize_t bytes_written = write ( file->descriptor
                                        , &newline
                                        , sizeof ( newline )
                                        );
    if ( bytes_written == -1 )
    {
        platform_log_error ( "platform_file_write ("PLATFORM_STRING"): write failed on file: %s"
                            , file->path
                            );
        return false;
    }

    // Update internal file position and size.
    file->position += bytes_written;
    file->size += bytes_written;

    total_bytes_written += bytes_written;

    return total_bytes_written == size + sizeof ( newline );
}

void
platform_file_stdin
(   file_t* file
)
{
    if ( !platform_stdin.initialized )
    {
        platform_stdin.descriptor = 0;
        platform_stdin.mode = FILE_MODE_READ;
        platform_stdin.path = "stdin";
        platform_stdin.size = 0;
        platform_stdin.position = 0;
        platform_stdin.initialized = true;
    }
    file->handle = &platform_stdin;
    file->valid = true;
}

void
platform_file_stdout
(   file_t* file
)
{
    if ( !platform_stdout.initialized )
    {
        platform_stdout.descriptor = 1;
        platform_stdout.mode = FILE_MODE_WRITE;
        platform_stdout.path = "stdout";
        platform_stdout.size = 0;
        platform_stdout.position = 0;
        platform_stdout.initialized = true;
    }
    file->handle = &platform_stdout;
    file->valid = true;
}

void
platform_file_stderr
(   file_t* file
)
{
    if ( !platform_stderr.initialized )
    {
        platform_stderr.descriptor = 2;
        platform_stderr.mode = FILE_MODE_WRITE;
        platform_stderr.path = "stderr";
        platform_stderr.size = 0;
        platform_stderr.position = 0;
        platform_stderr.initialized = true;
    }
    file->handle = &platform_stderr;
    file->valid = true;
}

i64
platform_error_code
( void )
{
    return errno;
}

u64
platform_error_message
(   i64     error
,   char*   dst
,   u64     dst_length
)
{
    if ( !dst || !dst_length )
    {
        if ( !dst )
        {
            LOGERROR ( "platform_error_message ("PLATFORM_STRING"): Missing argument: dst (output buffer)." );
        }
        if ( !dst_length )
        {
            LOGERROR ( "platform_error_message ("PLATFORM_STRING"): Value of dst_length argument must be non-zero." );
        }
        return 0;
    }
    if ( strerror_r ( error , dst , dst_length ) )
    {
        LOGERROR ( "platform_error_message ("PLATFORM_STRING"): Failed to retrieve an error report from the host platform." );
        return 0;
    }
    return _string_length_clamped ( dst , dst_length );
}

f64
platform_absolute_time
( void )
{
    struct timespec time;
    clock_gettime ( CLOCK_MONOTONIC , &time );
    return time.tv_sec + time.tv_nsec * 0.000000001;
}

#endif  // End platform layer.
////////////////////////////////////////////////////////////////////////////////
