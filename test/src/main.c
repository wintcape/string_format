/**
 * @file main.c
 * @brief Test driver.
 */
#include "core/logger.h"
#include "container/string/format.h"

int
main
( void )
{
    PRINT ( ANSI_CC2 ( ANSI_CC_BG_GREEN , ANSI_CC_FG_BLACK ) "Hello world!"
            ANSI_CC_RESET "\n"
          );
    // Your code here.
    return 0;
}