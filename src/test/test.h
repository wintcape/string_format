/**
 * @author Matthew Weissel
 * @file test/test.h
 * @brief Test management subsystem.
 */
#ifndef TEST_H
#define TEST_H

#include "common.h"

/** @brief Test bypass return code. */
#define BYPASS 2

/** @brief Type definition for a test callback function. */
typedef u8 ( *test_function_t )();

/** @brief Type definition for a container to hold test info. */
typedef struct
{
    test_function_t function;
    char*           description;
}
test_entry_t;

/**
 * @brief Initializes the test manager.
 */
void
test_startup
( void );

/**
 * @brief Registers a test with the test manager.
 * 
 * @param function A callback function.
 * @param description Test description string.
 * @return true on success; false otherwise.
 */
bool
test_register
(   test_function_t function
,   char*           description
);

/**
 * @brief Runs all registered tests.
 * 
 * @return true if all tests passed; false otherwise.
 */
bool
test_run_all
( void );

#endif  // TEST_H