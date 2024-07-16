/**
 * @author Matthew Weissel
 * @file math/random.h
 * @brief Random number generator functions.
 */
#ifndef MATH_RANDOM_H
#define MATH_RANDOM_H

#include "common.h"

/**
 * @brief Defines an alias for each random function. This resolves a name
 * conflict with the random functions included by the standard libc headers.
 */
#define random()          math_random ()                  /** @brief Defines an alias for the random function. This resolves a name conflict with the random function provided by the standard libc headers. */
#define random2(min,max)  math_random2 ( (min) , (max) )  /** @brief Defines an alias for the random2 function. */
#define randomf()         math_randomf ()                 /** @brief Defines an alias for the randomf function. */
#define randomf2(min,max) math_randomf2 ( (min) , (max) ) /** @brief Defines an alias for the randomf2 function. */

/**
 * @brief Generates a random integer.
 * 
 * @return A random integer.
 */
i32
math_random
( void );

/**
 * @brief Generates a random integer in the specified range.
 * 
 * @param min lower bound (inclusive).
 * @param max upper bound (inclusive).
 * @return A random integer in the range [ min , max ].
 */
i32
math_random2
(   i32 min
,   i32 max
);

/**
 * @brief Generates a random floating point number.
 * 
 * @return A random floating point number in the range [ 0.0 , 1.0 ].
 */
f32
math_randomf
( void );

/**
 * @brief Generates a random floating point number in the specified range.
 * 
 * @param min lower bound (inclusive).
 * @param max upper bound (inclusive).
 * @return A random floating point number in the range [ min , max ].
 */
f32
math_randomf2
(   f32 min
,   f32 max
);

#endif  // MATH_RANDOM_H