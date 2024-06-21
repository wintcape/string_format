/**
 * @file core/logger.h
 * @brief Provides an interface for the logging of runtime information.
 */
#ifndef LOGGER_H
#define LOGGER_H

#include "common.h"
#include "platform/platform.h"
#include "platform/filesystem.h"

/**
 * @brief Writes a formatted message to file.
 * 
 * Use PRINT to print to stdout, use PRINTERROR to print to stderr.
 * 
 * @param file The file to print to.
 * @param message Formatted message to print to file 
 * (see container/string/format.h).
 * @param args Variadic argument list (see common/args.h).
 */
void
print
(   file_t*         file
,   const char*     message
,   args_t          args
);

/** @brief Alias for calling print with __VA_ARGS__. */
#define PRINT(message,...)                                    \
    do                                                        \
    {                                                         \
        file_t file__;                                        \
        file_stdout ( &file__ );                              \
        DISABLE_WARNING ( -Wint-conversion )                  \
        print ( &file__ , (message) , ARGS ( __VA_ARGS__ ) ); \
        REENABLE_WARNING ()                                   \
    }                                                         \
    while ( 0 )

/** @brief Alias for calling print with __VA_ARGS__. */
#define PRINTERROR(message,...)                    \
    do                                             \
    {                                              \
        file_t file__;                             \
        file_stderr ( &file__ );                   \
        DISABLE_WARNING ( -Wint-conversion )       \
        print ( &file__                            \
              , ANSI_CC ( ANSI_CC_FG_RED ) message \
              , ARGS ( __VA_ARGS__ )               \
              );                                   \
        REENABLE_WARNING ()                        \
    }                                              \
    while ( 0 )

/** @brief Logs a platform-specific formatted error message. */
#define platform_log_error(message,...)                          \
    do                                                           \
    {                                                            \
        const i64 error__ = platform_error_code ();              \
        char message__[ STACK_STRING_MAX_SIZE ];                 \
        platform_error_message ( error__                         \
                               , message__                       \
                               , STACK_STRING_MAX_SIZE           \
                               );                                \
        PRINTERROR ( message "\n\tReason:  %s.\n\tCode:    %i\n" \
                 , ##__VA_ARGS__                                 \
                 , message__                                     \
                 , error__                                       \
                 );                                              \
    }                                                            \
    while ( 0 )

#endif  // LOGGER_H