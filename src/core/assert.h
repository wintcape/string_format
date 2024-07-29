/**
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
 * The expression may be passed as a null-terminated string.
 * 
 * @param expression The expression which failed (stringified). Must be
 * non-zero.
 * @param file The path of the file containing the assertion. Must be non-zero.
 * @param line The line number in file containing the assertion.
 * @param message Formatted message to log on assertion failure
 * (see container/string/format.h) (optional). Pass 0 for no additional message.
 * @param args Variadic argument list (see common/args.h).
 */
void
assertf
(   const char* expression
,   const char* file
,   const i64   line
,   const char* message
,   args_t      args
);

/**
 * @brief Definition for runtime assertion.
 * 
 * @param expression The expression to assert.
 */
#define ASSERT(expression)                                      \
{                                                               \
    if ( expression ) {}                                        \
    else                                                        \
    {                                                           \
        assertf ( #expression , __FILE__ , __LINE__ , "" , 0 ); \
        BRKDBG ();                                              \
    }                                                           \
}

/**
 * @brief Definition for runtime assertion (with formatted message).
 * 
 * @param expression The expression to assert.
 * @param message Formatted message to log on assertion failure
 * (see container/string/format.h).
 */
#define ASSERTM(expression,message,...)      \
{                                            \
    if ( expression ) {}                     \
    else                                     \
    {                                        \
        DISABLE_WARNING ( -Wint-conversion ) \
        assertf ( #expression                \
                , __FILE__                   \
                , __LINE__                   \
                , message                    \
                , ARGS ( __VA_ARGS__ )       \
                );                           \
        REENABLE_WARNING ()                  \
        BRKDBG ();                           \
    }                                        \
}

/**
 * @brief Definition for debug-mode-only runtime assertions.
 * 
 * @param expression The expression to assert.
 */
#if VERSION_DEBUG == 1
#define ASSERT_DEBUG(expression)                                \
{                                                               \
    if ( expression ) {}                                        \
    else                                                        \
    {                                                           \
        assertf ( #expression , __FILE__ , __LINE__ , "" , 0 ); \
        BRKDBG ();                                              \
    }                                                           \
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