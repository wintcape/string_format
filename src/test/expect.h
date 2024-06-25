/**
 * @author Matthew Weissel
 * @file test/expect.h
 * @brief Provides predicates for evaluating test success.
 */
#ifndef EXPECT_H
#define EXPECT_H

#include "common.h"

#include "core/logger.h"
#include "math/math.h"

/** @brief Truth test predicate. */
#define EXPECT(p)                                         \
    if ( (p) != true )                                    \
    {                                                     \
        const char* __file__ = __FILE__;                  \
        const u64 __line__ = __LINE__;                    \
        LOGERROR ( "--> Expected true, tested false.\n\t" \
                   "    in file:  %s:%i."                 \
                 , __file__ , __line__                    \
                 );                                       \
        return false;                                     \
    }

/** @brief Falsehood test predicate. */
#define EXPECT_NOT(p)                                     \
    if ( (p) != false )                                   \
    {                                                     \
        const char* __file__ = __FILE__;                  \
        const u64 __line__ = __LINE__;                    \
        LOGERROR ( "--> Expected false, tested true.\n\t" \
                   "    in file:  %s:%i."                 \
                 , __file__ , __line__                    \
                 );                                       \
        return false;                                     \
    }

/** @brief Equality test predicate. */
#define EXPECT_EQ(expected,actual)                              \
    if ( (actual) != (expected) )                               \
    {                                                           \
        const char* __file__ = __FILE__;                        \
        const u64 __line__ = __LINE__;                          \
        LOGERROR ( "--> Expected %i, tested %i.\n\t"            \
                   "    in file:  %s:%i."                       \
                 , (expected) , (actual) , __file__ , __line__  \
                 );                                             \
        return false;                                           \
    }

/** @brief Inequality test predicate. */
#define EXPECT_NEQ(expected,actual)                                       \
    if ( (actual) == (expected) )                                         \
    {                                                                     \
        const char* __file__ = __FILE__;                                  \
        const u64 __line__ = __LINE__;                                    \
        LOGERROR ( "--> Expected something other than %i, tested %i.\n\t" \
                   "    in file:  %s:%i."                                 \
                 , (expected) , (actual) , __file__ , __line__            \
                 );                                                       \
        return false;                                                     \
    }

/** @brief Tests floats within a tolerance of K_FLOAT_EPSILON (see math/float.h). */
#define EXPECT_EQF(expected,actual,tolerance)          \
    if ( abs ( (expected) - (actual) ) > (tolerance) ) \
    {                                                  \
        const char* __file__ = __FILE__;               \
        const u64 __line__ = __LINE__;                 \
        const f64 expected__ = (expected);             \
        const f64 actual__ = (actual);                 \
        LOGERROR ( "--> Expected %f, tested %f.\n\t"   \
                   "    in file:  %s:%i."              \
                 , &expected__ , &actual__             \
                 , __file__ , __line__                 \
                 );                                    \
        return false;                                  \
    }

#endif  // EXPECT_H