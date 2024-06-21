/**
 * @file math/float.h
 * @brief Defines miscellaneous floating point constants and operations.
 */
#ifndef MATH_FLOAT_H
#define MATH_FLOAT_H

#include "common.h"

// Global constants.
#define INFINITY        ( 1.0F / 0.0F )         /** @brief Placeholder value for floating point number overflow. */
#define FLOAT_EPSILON   1.192092896E-7F         /** @brief Smallest possible non-zero floating point number. */
#define EULERS_NUMBER   2.71828182845904523536F /** @brief Euler's number: e.*/
#define SQRT2           1.41421356237309504880F /** @brief sqrt(2). */
#define SQRT3           1.73205080756887729352F /** @brief sqrt(3). */
#define DIV_SQRT2       0.70710678118654752440F /** @brief 1 / sqrt(2). */
#define DIV_SQRT3       0.57735026918962576450F /** @brief 1 / sqrt(3). */

/**
 * @brief Defines an alias for each floating point function. This resolves a
 * name conflict with those provided by the standard libc headers.
 */
#define mix(x,y,t) math_mix ( (x) , (y) , (t) ) /** @brief Defines an alias for the mix function. This resolves a name conflict with the mix function provided by the standard libc headers. */
#define nan(x)     math_nan ( x )               /** @brief Defines an alias for the nan test predicate function. This resolves a name conflict with the nan test predicate function provided by the standard libc headers. */
#define finite(x)  math_finite ( x )            /** @brief Defines an alias for the overflow test predicat function. This resolves a name conflict with the overflow test predicate function provided by the standard libc headers. */
#define abs(x)     math_abs ( x )               /** @brief Defines an alias for the absolute value function. This resolves a name conflict with the absolute value function provided by the standard libc headers. */
#define floor(x)   math_floor ( x )             /** @brief Defines an alias for the floor function. This resolves a name conflict with the floor function provided by the standard libc headers. */
#define ceiling(x) math_ceiling ( x )           /** @brief Defines an alias for the ceiling function. This resolves a name conflict with the ceiling function provided by the standard libc headers. */
#define pow(x,y)   math_pow ( (x) , (y) )       /** @brief Defines an alias for the exponentiation function. This resolves a name conflict with the exponentiation function included by the standard libc headers. */
#define sqrt(x)    math_sqrt ( x )              /** @brief Defines an alias for the square root function. This resolves a name conflict with the square root function included by the standard libc headers. */
#define exp(x)     math_exp ( x )               /** @brief Defines an alias for the exponential function. This resolves a name conflict with the exponential function included by the standard libc headers. */
#define ln(x)      math_ln ( x )                /** @brief Defines an alias for the natural logarithmic function. This resolves a name conflict with the natural logarithmic function included by the standard libc headers. */
#define log(x)     math_log ( x )               /** @brief Defines an alias for the base-10 logarithmic function. This resolves a name conflict with the base-10 logarithmic function included by the standard libc headers. */

/**
 * @brief Interpolates two floating point values using the provided weight.
 * 
 * @param x A floating point number.
 * @param y A floating point number.
 * @param t Weight. Its value must be in the range [ 0.0 , 1.0 ].
 * @return Interpolation between x and y weighted by t.
 */
INLINE
f32
math_mix
(   f32 x
,   f32 y
,   f32 t
)
{
    return ( 1.0F - t ) * x + t * y;
}

/**
 * @brief Not a number? Y/N
 * 
 * @param x A floating point number.
 * @return true if x is NaN; false otherwise.
 */
bool
math_nan
(   f32 x
);

/**
 * @brief Overflow? Y/N
 * 
 * @param x A floating point number.
 * @return true if x is verifiably finite; false otherwise.
 */
bool
math_finite
(   f32 x
);

/**
 * @brief Absolute value function.
 * 
 * @param x A floating point number.
 * @return abs(x)
 */
f32
math_abs
(   f32 x
);

/**
 * @brief Floor function.
 * 
 * @param x A floating point number.
 * @return floor(x)
 */
f32
math_floor
(   f32 x
);

/**
 * @brief Ceiling function.
 * 
 * @param x A floating point number.
 * @return ceiling(x)
 */
f32
math_ceiling
(   f32 x
);

/**
 * @brief Exponentiation function.
 * 
 * @param x Base.
 * @param y Exponent.
 * @return x^y
 */
f32
math_pow
(   f32 x
,   f32 y
);

/**
 * @brief Square root function.
 * 
 * @param x A floating point number.
 * @return sqrt(x)
 */
f32
math_sqrt
(   f32 x
);

/**
 * @brief Exponential function.
 * 
 * @param x A floating point number.
 * @return e^x
 */
f32
math_exp
(   f32 x
);

/**
 * @brief Natural logarithmic function.
 * 
 * @param x A floating point number.
 * @return ln(x)
 */
f32
math_ln
(   f32 x
);

/**
 * @brief Logarithmic function (base-10).
 * 
 * @param x A floating point number.
 * @return log(x)
 */
f32
math_log
(   f32 x
);

#endif  // MATH_FLOAT_H