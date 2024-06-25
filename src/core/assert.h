/**
 * @author Matthew Weissel
 * @file core/assert.h
 * @brief Defines runtime assertion behavior.
 */
#ifndef ASSERT_H
#define ASSERT_H

#include "common.h"

/** @brief Enable runtime assert statements? Y/N */
#define ASSERT_ENABLED 1

////////////////////////////////////////////////////////////////////////////////
#if ASSERT_ENABLED == 1

/** @brief Compiler-independent definition for code breakpoint. */
#if _MSC_VER
    #include <intrin.h>
    #define BRKDBG()    ( __debugbreak () )
#else
    #define BRKDBG()    ( __builtin_trap () )
#endif

/**
 * @brief Interface for logging information on assertion failure.
 * 
 * @param expression The expression to assert. Must be non-zero.
 * @param message The message to log on assertion failure. Must be non-zero.
 * @param file The file containing the caller of the BRKDBG invocation that was
 * triggered. Must be non-zero.
 * @param line The line number in file containing the caller of the BRKDBG
 * invocation that was triggered.
 */
void
assertf
(   const char* expression
,   const char* message
,   const char* file
,   const i32   line
);

/**
 * @brief Definition for runtime assertion.
 * 
 * @param expression The expression to assert.
 */
#define ASSERT(expression)                                  \
{                                                           \
    if ( expression ) {}                                    \
    else                                                    \
    {                                                       \
        assertf ( #expression , "" , __FILE__ , __LINE__ ); \
        BRKDBG ();                                          \
    }                                                       \
}

/**
 * @brief Definition for runtime assertion (with message).
 * 
 * @param expression The expression to assert.
 * @param message The message to log on assertion failure.
 */
#define ASSERTM(expression,message)                              \
{                                                                \
    if ( expression ) {}                                         \
    else                                                         \
    {                                                            \
        assertf ( #expression , message , __FILE__ , __LINE__ ); \
        BRKDBG ();                                               \
    }                                                            \
}

/**
 * @brief Definition for debug-mode-only runtime assertions.
 * 
 * @param expr The expression to assert.
 */
#if VERSION_DEBUG == 1
#define ASSERT_DEBUG(expression)                            \
{                                                           \
    if ( expression ) {}                                    \
    else                                                    \
    {                                                       \
        assertf ( #expression , "" , __FILE__ , __LINE__ ); \
        BRKDBG ();                                          \
    }                                                       \
}
#else
#define ASSERT_DEBUG(expression)
#endif
////////////////////////////////////////////////////////////////////////////////

// Disable all if ASSERT_ENABLED != 1.
#else
    #define ASSERT(expression)
    #define ASSERTM(expression,message)
    #define ASSERT_DEBUG(expression)

#endif  // ASSERT_ENABLED
#endif  // ASSERT_H