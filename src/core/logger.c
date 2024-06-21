/**
 * @file core/logger.c
 * @brief Implementation of the core/logger and core/assert headers.
 * (see core/logger.h and core/assert.h for additional details)
 */
#include "core/logger.h"

#include "container/string.h"
#include "platform/memory.h"

/**
 * @brief Primary implementation of print (see print).
 * 
 * @param file The file to print to. Must be non-zero.
 * @param string The string to print. Must be non-zero.
 * @param string_length The number of characters in the string.
 */
void
_print
(   file_t*         file
,   const char*     string
,   const u64       string_length
);

void
print
(   file_t*         file
,   const char*     message
,   args_t          args
)
{
    if ( !file || !file->handle || !file->valid )
    {
        return;
    }
    string_t* raw = _string_format ( message , args );
    string_t* formatted = string_format ( ANSI_CC_RESET"%S"ANSI_CC_RESET , raw );
    _print ( file , formatted , string_length ( formatted ) );
    string_destroy ( raw );
    string_destroy ( formatted );
}

void
_print
(   file_t*         file
,   const char*     string
,   const u64       string_length
)
{
    u64 written;
    file_write ( file
               , ( string_length + 1 ) * sizeof ( char )
               , string     //     ^ terminator
               , &written
               );
}