/**
 * @author Matthew Weissel
 * @file math/trig64.h
 * @brief Defines 64-bit trigonometric functions.
 */
#ifndef MATH_TRIGONOMETRY64_H
#define MATH_TRIGONOMETRY64_H

#include "common.h"

// Global constants.
#define PI_64              3.14159265358979323846 /** @brief π */
#define TWO_PI_64          ( 2.0 * PI_64 )        /** @brief 2 π */
#define FOUR_PI_64         ( 4.0 * PI_64 )        /** @brief 4 π */
#define HALF_PI_64         ( 0.5 * PI_64 )        /** @brief π / 2 */
#define QUARTER_PI_64      ( 0.25 * PI_64 )       /** @brief π / 4 */
#define DIV_PI_64          ( 1.0 / PI_64 )        /** @brief 1 / π */
#define DIV_TWO_PI_64      ( 1.0 / TWO_PI_64 )    /** @brief 1 / 2 π */

/**
 * @brief Defines an alias for each trigonometric function. This avoids function
 * signatures which cause name conflicts with those provided by the standard
 * libc headers.
 */
#define sin64(x)  math_sin_64 ( x )  /** @brief Defines an alias for the sine function. This resolves a name conflict with the sine function provided by the standard libc headers. */
#define cos64(x)  math_cos_64 ( x )  /** @brief Defines an alias for the cosine function. This resolves a name conflict with the cosine function provided by the standard libc headers. */
#define tan64(x)  math_tan_64 ( x )  /** @brief Defines an alias for the tangent function. This resolves a name conflict with the tangent function provided by the standard libc headers. */
#define asin64(x) math_asin_64 ( x ) /** @brief Defines an alias for the sine inverse function. This resolves a name conflict with the sine inverse function provided by the standard libc headers. */
#define acos64(x) math_acos_64 ( x ) /** @brief Defines an alias for the cosine inverse function. This resolves a name conflict with the cosine inverse function provided by the standard libc headers. */
#define atan64(x) math_atan_64 ( x ) /** @brief Defines an alias for the tangent inverse function. This resolves a name conflict with the tangent inverse function provided by the standard libc headers. */
#define sinh64(x) math_sinh_64 ( x ) /** @brief Defines an alias for the hyperbolic sine function. This resolves a name conflict with the hyperbolic sine function provided by the standard libc headers. */
#define cosh64(x) math_cosh_64 ( x ) /** @brief Defines an alias for the hyperbolic cosine function. This resolves a name conflict with the hyperbolic cosine function provided by the standard libc headers. */
#define tanh64(x) math_tanh_64 ( x ) /** @brief Defines an alias for the hyperbolic tangent function. This resolves a name conflict with the hyperbolic tangent function provided by the standard libc headers. */

/**
 * @brief Sine function.
 * 
 * @param x A floating-point number.
 * @return sin(x)
 */
f64
math_sin_64
(   f64 x
);

/**
 * @brief Cosine function.
 * 
 * @param x A floating-point number.
 * @return cos(x)
 */
f64
math_cos_64
(   f64 x
);

/**
 * @brief Tangent function.
 * 
 * @param x A floating-point number.
 * @return tan(x)
 */
f64
math_tan_64
(   f64 x
);

/**
 * @brief Sine inverse function.
 * 
 * @param x A floating-point number.
 * @return arcsin(x)
 */
f64
math_asin_64
(   f64 x
);

/**
 * @brief Cosine inverse function.
 * 
 * @param x A floating-point number.
 * @return arccos(x)
 */
f64
math_acos_64
(   f64 x
);

/**
 * @brief Tangent inverse function.
 * 
 * @param x A floating-point number.
 * @return arctan(x)
 */
f64
math_atan_64
(   f64 x
);

/**
 * @brief Hyperbolic sine function.
 * 
 * @param x A floating-point number.
 * @return sinh(x)
 */
f64
math_sinh_64
(   f64 x
);

/**
 * @brief Hyperbolic cosine function.
 * 
 * @param x A floating-point number.
 * @return cosh(x)
 */
f64
math_cosh_64
(   f64 x
);

/**
 * @brief Hyperbolic tangent function.
 * 
 * @param x A floating-point number.
 * @return tanh(x)
 */
f64
math_tanh_64
(   f64 x
);

#endif  // MATH_TRIGONOMETRY64_H