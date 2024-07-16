/**
 * @author Matthew Weissel
 * @file math/conversion.h
 * @brief Measurement conversion rates.
 */
#ifndef MATH_CONVERSION_H
#define MATH_CONVERSION_H

#include "common.h"

#include "math/trig.h"

// Time conversion.
#define S_TO_NS ( 1000.0F * 1000.0F * 1000.0F ) /** @brief Conversion factor (seconds to nanoseconds). */
#define S_TO_US ( 1000.0F * 1000.0F )           /** @brief Conversion factor (seconds to microseconds). */
#define S_TO_MS 1000.0F                         /** @brief Conversion factor (seconds to milliseconds). */
#define MS_TO_S 0.001F                          /** @brief Conversion factor (milliseconds to seconds). */

// Angle conversion.
#define DEG_TO_RAD ( PI / 180.0F ) /** @brief Conversion factor (degrees to radians). */
#define RAD_TO_DEG ( 180.0F / PI ) /** @brief Conversion factor (radians to degrees). */

/**
 * @brief Degree to radian conversion function.
 * 
 * @param deg An angle in degrees.
 * @return deg converted to radians.
 */
INLINE
f32
radians
(   f32 deg
)
{
    return DEG_TO_RAD * deg;
}

/**
 * @brief Radian to degree conversion function.
 * 
 * @param rad An angle in radians.
 * @return rad converted to degrees.
 */
INLINE
f32
degrees
(   f32 rad
)
{
    return RAD_TO_DEG * rad;
}

#endif  // MATH_CONVERSION_H