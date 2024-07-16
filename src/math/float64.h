/**
 * @file math/float64.h
 * @brief Defines miscellaneous 64-bit floating point constants and operations.
 */
#ifndef MATH_FLOAT64_H
#define MATH_FLOAT64_H

#include "common.h"

// Global constants.
#define INFINITY_64        ( 1.0 / 0.0 )          /** @brief Placeholder value for floating point number overflow. */
#define FLOAT_EPSILON_64   1.192092896E-7         /** @brief Smallest possible non-zero floating point number. */
#define EULERS_NUMBER_64   2.71828182845904523536 /** @brief Euler's number: e.*/
#define SQRT2_64           1.41421356237309504880 /** @brief sqrt(2). */
#define SQRT3_64           1.73205080756887729352 /** @brief sqrt(3). */
#define DIV_SQRT2_64       0.70710678118654752440 /** @brief 1 / sqrt(2). */
#define DIV_SQRT3_64       0.57735026918962576450 /** @brief 1 / sqrt(3). */

/**
 * @brief Defines an alias for each floating point function. This resolves a
 * name conflict with those provided by the standard libc headers.
 */
#define mix64(x,y,t) math_mix_64 ( (x) , (y) , (t) ) /** @brief Defines an alias for the mix function. This resolves a name conflict with the mix function provided by the standard libc headers. */
#define nan64(x)     math_nan_64 ( x )               /** @brief Defines an alias for the nan test predicate function. This resolves a name conflict with the nan test predicate function provided by the standard libc headers. */
#define finite64(x)  math_finite_64 ( x )            /** @brief Defines an alias for the overflow test predicat function. This resolves a name conflict with the overflow test predicate function provided by the standard libc headers. */
#define abs64(x)     math_abs_64 ( x )               /** @brief Defines an alias for the absolute value function. This resolves a name conflict with the absolute value function provided by the standard libc headers. */
#define floor64(x)   math_floor_64 ( x )             /** @brief Defines an alias for the floor function. This resolves a name conflict with the floor function provided by the standard libc headers. */
#define ceiling64(x) math_ceiling_64 ( x )           /** @brief Defines an alias for the ceiling function. This resolves a name conflict with the ceiling function provided by the standard libc headers. */
#define pow64(x,y)   math_pow_64 ( (x) , (y) )       /** @brief Defines an alias for the exponentiation function. This resolves a name conflict with the exponentiation function included by the standard libc headers. */
#define sqrt64(x)    math_sqrt_64 ( x )              /** @brief Defines an alias for the square root function. This resolves a name conflict with the square root function included by the standard libc headers. */
#define exp64(x)     math_exp_64 ( x )               /** @brief Defines an alias for the exponential function. This resolves a name conflict with the exponential function included by the standard libc headers. */
#define ln64(x)      math_ln_64 ( x )                /** @brief Defines an alias for the natural logarithmic function. This resolves a name conflict with the natural logarithmic function included by the standard libc headers. */
#define log64(x)     math_log_64 ( x )               /** @brief Defines an alias for the base-10 logarithmic function. This resolves a name conflict with the base-10 logarithmic function included by the standard libc headers. */

/**
 * @brief Interpolates two floating point values using the provided weight.
 * 
 * @param x A floating point number.
 * @param y A floating point number.
 * @param t Weight. Its value must be in the range [ 0.0 , 1.0 ].
 * @return Interpolation between x and y weighted by t.
 */
INLINE
f64
math_mix_64
(   f64 x
,   f64 y
,   f64 t
)
{
    return ( 1.0 - t ) * x + t * y;
}

/**
 * @brief Not a number? Y/N
 * 
 * @param x A floating point number.
 * @return true if x is NaN; false otherwise.
 */
bool
math_nan_64
(   f64 x
);

/**
 * @brief Overflow? Y/N
 * 
 * @param x A floating point number.
 * @return true if x is verifiably finite; false otherwise.
 */
bool
math_finite_64
(   f64 x
);

/**
 * @brief Absolute value function.
 * 
 * @param x A floating point number.
 * @return abs(x)
 */
f64
math_abs_64
(   f64 x
);

/**
 * @brief Floor function.
 * 
 * @param x A floating point number.
 * @return floor(x)
 */
f64
math_floor_64
(   f64 x
);

/**
 * @brief Ceiling function.
 * 
 * @param x A floating point number.
 * @return ceiling(x)
 */
f64
math_ceiling_64
(   f64 x
);

/**
 * @brief Exponentiation function.
 * 
 * @param x Base.
 * @param y Exponent.
 * @return x^y
 */
f64
math_pow_64
(   f64 x
,   f64 y
);

/**
 * @brief Square root function.
 * 
 * @param x A floating point number.
 * @return sqrt(x)
 */
f64
math_sqrt_64
(   f64 x
);

/**
 * @brief Exponential function.
 * 
 * @param x A floating point number.
 * @return e^x
 */
f64
math_exp_64
(   f64 x
);

/**
 * @brief Natural logarithmic function.
 * 
 * @param x A floating point number.
 * @return ln(x)
 */
f64
math_ln_64
(   f64 x
);

/**
 * @brief Logarithmic function (base-10).
 * 
 * @param x A floating point number.
 * @return log(x)
 */
f64
math_log_64
(   f64 x
);

#endif  // MATH_FLOAT64_H