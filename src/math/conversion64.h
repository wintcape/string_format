/**
 * @author Matthew Weissel
 * @file math/conversion64.h
 * @brief 64-bit measurement conversion rates.
 */
#ifndef MATH_CONVERSION_64_H
#define MATH_CONVERSION_64_H

#include "common.h"

#include "math/trig64.h"

// Time conversion.
#define S_TO_NS_64 ( 1000.0 * 1000.0 * 1000.0 ) /** @brief Conversion factor (seconds to nanoseconds). */
#define S_TO_US_64 ( 1000.0 * 1000.0 )          /** @brief Conversion factor (seconds to microseconds). */
#define S_TO_MS_64 1000.0                       /** @brief Conversion factor (seconds to milliseconds). */
#define MS_TO_S_64 0.001                        /** @brief Conversion factor (milliseconds to seconds). */

// Angle conversion.
#define DEG_TO_RAD_64 ( PI_64 / 180.0 ) /** @brief Conversion factor (degrees to radians). */
#define RAD_TO_DEG_64 ( 180.0 / PI_64 ) /** @brief Conversion factor (radians to degrees). */

/**
 * @brief Degree to radian conversion function.
 * 
 * @param deg An angle in degrees.
 * @return deg converted to radians.
 */
INLINE
f64
radians64
(   f64 deg
)
{
    return DEG_TO_RAD_64 * deg;
}

/**
 * @brief Radian to degree conversion function.
 * 
 * @param rad An angle in radians.
 * @return rad converted to degrees.
 */
INLINE
f64
degrees64
(   f64 rad
)
{
    return RAD_TO_DEG_64 * rad;
}

/**
 * @brief Converts elapsed time into appropriate units (for display purposes).
 * 
 * @param elapsed Elapsed time (in seconds).
 * @param hours Output buffer for hours. Must be non-zero.
 * @param minutes Output buffer for minutes. Must be non-zero.
 * @param seconds Output buffer for seconds. Must be non-zero.
 * @param fractional Output buffer for leftover fractional. Must be non-zero.
 */
INLINE
void
display_time
(   const f64   elapsed
,   u64*        hours
,   u64*        minutes
,   u64*        seconds
,   f64*        fractional
)
{
    *hours = ( ( u64 ) elapsed ) / ( 60 * 60 );
    *minutes = ( ( ( u64 ) elapsed ) / 60 ) % 60;
    *seconds = ( ( u64 ) elapsed ) % 60;
    *fractional = elapsed - ( ( f64 ) *seconds );
}

#endif  // MATH_CONVERSION_64_H