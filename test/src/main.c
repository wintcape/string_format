/**
 * @file main.c
 * @brief Entry point for the test suite program.
 */
#include "test/test.h"

#include "container/test_string.h"
#include "platform/test_filesystem.h"

#include "core/logger.h"
#include "core/string.h"

#include "math/math.h"

/** @brief Log filepath. */
#define LOG_FILEPATH \
    "console.log"

/** @brief Colored text. */
#define COLORED(text) \
    ANSI_CC ( ANSI_CC_FG_DARK_GREEN ) text ANSI_CC_RESET

int
main
( void )
{
    // Initialize logger.
    logger_startup ( LOG_FILEPATH , 0 , 0 );

    // Initialize tests.
    test_startup ();
    test_register_string ();
    // test_register_filesystem ();

    // Run tests.
    LOGINFO ( "Running test suite. . ." );
    const bool fail = test_run_all ();
    if ( !fail )
    {
        PRINT ( COLORED ( "string_format ("PLATFORM_STRING") ver. %i.%i.%i: All tests passed." ) "\n\n"
              , VERSION_MAJOR , VERSION_MINOR , VERSION_PATCH
              );
    }

    // Shutdown logger.
    logger_shutdown ();

    return fail;
}