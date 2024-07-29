/**
 * @file core/logger.c
 * @brief Implementation of the core/logger and core/assert headers.
 * (see core/logger.h and core/assert.h for additional details)
 */
#include "core/assert.h"
#include "core/logger.h"

#include "container/string.h"
#include "platform/memory.h"

/** @brief Output message prefixes. */
static const char* log_level_prefixes[] = { LOG_LEVEL_PREFIX_FATAL
                                          , LOG_LEVEL_PREFIX_ERROR
                                          , LOG_LEVEL_PREFIX_WARN
                                          , LOG_LEVEL_PREFIX_INFO
                                          , LOG_LEVEL_PREFIX_DEBUG
                                          , LOG_LEVEL_PREFIX_TRACE
                                          , LOG_LEVEL_PREFIX_SILENT
                                          };

/** @brief Output message colors. */
static const char* log_level_colors[] = { LOG_LEVEL_COLOR_FATAL
                                        , LOG_LEVEL_COLOR_ERROR
                                        , LOG_LEVEL_COLOR_WARN
                                        , LOG_LEVEL_COLOR_INFO
                                        , LOG_LEVEL_COLOR_DEBUG
                                        , LOG_LEVEL_COLOR_TRACE
                                        };

/** @brief Type definition for logger subsystem state. */
typedef struct
{
    bool        initialized;

    file_t      file;
    const char* filepath;

    bool        owns_memory;
}
state_t;

/** @brief Global subsystem state. */
static state_t* state = 0;

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

/**
 * @brief Appends a message to the log file.
 * 
 * Use logger_file_append to explicitly specify string length, or 
 * _logger_file_append to compute the length of a null-terminated string before
 * passing it to logger_file_append.
 * 
 * @param message The message string to append. Must be non-zero.
 * @param message_length The message length (in characters).
 */
void
logger_file_append
(   const char* message
,   const u64   message_length
);

#define _logger_file_append(message) \
    logger_file_append ( (message) , _string_length ( message ) )

bool
logger_startup
(   const char* filepath
,   u64*        memory_requirement_
,   void*       state_
)
{
    if ( state )
    {
        LOGERROR ( "logger_startup: Called more than once." );
        return false;
    }

    const u64 memory_requirement = sizeof ( state_t );

    if ( memory_requirement_ )
    {
        *memory_requirement_ = memory_requirement;
        if ( !state_ )
        {
            return true;
        }
    }

    if ( state_ )
    {
        state = state_;
        state->owns_memory = false;
    }
    else
    {
        state = memory_allocate ( memory_requirement /*, MEMORY_TAG_LOGGER */ );
        state->owns_memory = true;
    }

    state->initialized = false;
    state->filepath = filepath;

    // CASE: Log file requested.
    if ( state->filepath )
    {
        if ( !file_open ( state->filepath , FILE_MODE_WRITE , &state->file ) )
        {
            LOGERROR ( "logger_startup: Unable to open log file for writing:  %s."
                     , state->filepath
                     );
            return false;
        }
    }

    // CASE: No log file requested.
    else
    {
        state->file.handle = 0;
        state->file.valid = false;
    }

    state->initialized = true;

    return true;
}

bool
logger_shutdown
( void )
{
    if ( !state )
    {
        return true;
    }

    state->initialized = false;

    // Close log file.
    file_close ( &state->file );

    const u64 memory_requirement = sizeof ( state_t );
    if ( state->owns_memory )
    {
        memory_free ( state /* , memory_requirement , MEMORY_TAG_LOGGER */ );
    }
    else
    {
        memory_clear ( state , memory_requirement );
    }

    state = 0;
    return true;
}

void
logger_log
(   LOG_LEVEL   level
,   const char* message
,   args_t      args
)
{
    const bool err = level < LOG_WARN;
    const bool colored = level != LOG_INFO;

    string_t* raw = _string_format ( message , args );
    string_t* plaintext = 0;
    string_t* formatted = 0;

    // Write plaintext to log file.
    if ( state && state->initialized && state->file.valid )
    {
        plaintext = _string_copy ( raw );
        __string_strip_ansi ( plaintext );
        _string_prepend ( plaintext , log_level_prefixes[ level ] );
        logger_file_append ( plaintext , string_length ( plaintext ) );
    }

    // CASE: Silent log elevation.
    if ( level == LOG_SILENT )
    {
        string_destroy ( raw );
        string_destroy ( plaintext );
        return;
    }

    // Write ANSI-formatted text to console.
    formatted = string_format ( ANSI_CC_RESET"%s%s%s%S"ANSI_CC_RESET"\n"
                              , log_level_colors[ level ]
                              , log_level_prefixes[ level ]
                              , ( colored ) ? "" : ANSI_CC_RESET
                              , raw
                              );
    file_t file;
    ( err ) ? file_stderr ( &file ) : file_stdout ( &file );
    _print ( &file , formatted , string_length ( formatted ) );

    string_destroy ( raw );
    string_destroy ( plaintext );
    string_destroy ( formatted );
}

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
    string_t* formatted = string_format ( ANSI_CC_RESET"%S"ANSI_CC_RESET  , raw );
    _print ( file , formatted , string_length ( formatted ) );
    string_destroy ( raw );
    string_destroy ( formatted );
}

void
assertf
(   const char* expression
,   const char* file
,   const i64   line
,   const char* message
,   args_t      args
)
{
    if ( string_empty ( message ) )
    {
        LOGFATAL ( "Assertion failure in file:  %s:%i\n\t"
                   "Expression:  %s"
                 , file , line , expression
                 );
        return;
    }

    string_t* raw = string_format ( "Assertion failure in file:  %s:%i"
                                    ANSI_CC_RESET "\n\t" LOG_LEVEL_COLOR_FATAL
                                    "Expression:  %s"
                                    ANSI_CC_RESET "\n\t" LOG_LEVEL_COLOR_FATAL
                                    "Message: %s"
                                  , file
                                  , line
                                  , expression
                                  , message
                                  );
    logger_log ( LOG_FATAL , raw , args );
    string_destroy ( raw );
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

void
logger_file_append
(   const char* message
,   const u64   message_length
)
{
    if ( !file_write_line ( &state->file , message_length , message ) )
    {
        state->file.valid = false; // Invalidate the log file.
        LOGERROR ( "logger_file_append: Error writing to log file:  %s"
                 , state->filepath
                 );
    }
}