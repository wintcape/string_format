/**
 * @author Matthew Weissel
 * @file math/random64.h
 * @brief 64-bit random number generator functions.
 */
#ifndef MATH_RANDOM64_H
#define MATH_RANDOM64_H

#include "common.h"

/**
 * @brief Defines an alias for each random function. This resolves a name
 * conflict with the random functions included by the standard libc headers.
 */
#define random64()           math_random_64 ()                  /** @brief Defines an alias for the random function. This resolves a name conflict with the random function provided by the standard libc headers. */
#define random64_2(min,max)  math_random2_64 ( (min) , (max) )  /** @brief Defines an alias for the random2 function. */
#define randomf64()          math_randomf_64 ()                 /** @brief Defines an alias for the randomf function. */
#define randomf64_2(min,max) math_randomf2_64 ( (min) , (max) ) /** @brief Defines an alias for the randomf2 function. */

/**
 * @brief Generates a random integer.
 * 
 * @return A random integer.
 */
i64
math_random_64
( void );

/**
 * @brief Generates a random integer in the specified range.
 * 
 * @param min lower bound (inclusive).
 * @param max upper bound (inclusive).
 * @return A random integer in the range [ min , max ].
 */
i64
math_random2_64
(   i64 min
,   i64 max
);

/**
 * @brief Generates a random floating point number.
 * 
 * @return A random floating point number in the range [ 0.0 , 1.0 ].
 */
f64
math_randomf_64
( void );

/**
 * @brief Generates a random floating point number in the specified range.
 * 
 * @param min lower bound (inclusive).
 * @param max upper bound (inclusive).
 * @return A random floating point number in the range [ min , max ].
 */
f64
math_randomf2_64
(   f64 min
,   f64 max
);

#endif  // MATH_RANDOM64_H