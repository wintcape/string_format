/**
 * @author Matthew Weissel (mweissel3@gatech.edu)
 * @file math/math.c
 * @brief Implementation of the math header.
 * (see math/math.h for additional details)
 */
#include "math/math.h"

#include "core/string.h"

#include "platform/platform.h"

/**
 * @brief Undefines preprocessor bindings which may cause name conflicts with
 * the standard libc headers.
 */
#undef nan
#undef finite
#undef abs
#undef floor
#undef ceiling
#undef pow
#undef sqrt
#undef exp
#undef ln
#undef log
#undef sin
#undef cos
#undef tan
#undef asin
#undef acos
#undef atan
#undef sinh
#undef cosh
#undef tanh
#undef random
#undef random2
#undef randomf
#undef randomf2
#undef nan64
#undef finite64
#undef abs64
#undef floor64
#undef ceiling64
#undef pow64
#undef sqrt64
#undef exp64
#undef ln64
#undef log64
#undef sin64
#undef cos64
#undef tan64
#undef asin64
#undef acos64
#undef atan64
#undef sinh64
#undef cosh64
#undef tanh64
#undef random64
#undef random64_2
#undef randomf64
#undef randomf64_2

// Standard libc dependencies.
#include <math.h>
#include <stdlib.h>

// ( see random ).
static bool random_seeded = false;

////////////////////////////////////////////////////////////////////////////////
// Begin 32-bit math.

bool
math_nan
(   f32 x
)
{
    return isnan ( x );
}

bool
math_finite
(   f32 x
)
{
    return !isinf ( x );
}

f32
math_abs
(   f32 x
)
{
    return fabsf ( x );
}

f32
math_floor
(   f32 x
)
{
    return floorf ( x );
}

f32
math_ceiling
(   f32 x
)
{
    return ceilf ( x );
}

f32
math_pow
(   f32 x
,   f32 y
)
{
    return powf ( x , y );
}

f32
math_sqrt
(   f32 x
)
{
    return sqrtf ( x );
}

f32
math_exp
(   f32 x
)
{
    return expf ( x );
}

f32
math_ln
(   f32 x
)
{
    return logf ( x );
}

f32
math_log
(   f32 x
)
{
    return log10f ( x );
}

f32
math_sin
(   f32 x
)
{
    return sinf ( x );
}

f32
math_cos
(   f32 x
)
{
    return cosf ( x );
}

f32
math_tan
(   f32 x
)
{
    return tanf ( x );
}

f32
math_asin
(   f32 x
)
{
    return asinf ( x );
}

f32
math_acos
(   f32 x
)
{
    return acosf ( x );
}

f32
math_atan
(   f32 x
)
{
    return atanf ( x );
}

f32
math_sinh
(   f32 x
)
{
    return sinhf ( x );
}

f32
math_cosh
(   f32 x
)
{
    return coshf ( x );
}

f32
math_tanh
(   f32 x
)
{
    return tanhf ( x );
}

i32
math_random
( void )
{
    if ( !random_seeded )
    {
        srand ( ( u32 ) platform_absolute_time () );
        random_seeded = true;
    }
    return rand ();
}

i32
math_random2
(   i32 min
,   i32 max
)
{
    return ( math_random () % ( max - min + 1 ) ) + min;
}

f32
math_randomf
( void )
{
    return math_randomf2 ( 0.0F , 1.0F );
}

f32
math_randomf2
(   f32 min
,   f32 max
)
{
    return min + ( f32 ) math_random () / ( ( ( f32 ) RAND_MAX ) / ( max - min ) );
}

// End 32-bit math.
////////////////////////////////////////////////////////////////////////////////
// Begin 64-bit math.

bool
math_nan_64
(   f64 x
)
{
    return isnan ( x );
}

bool
math_finite_64
(   f64 x
)
{
    return !isinf ( x );
}

f64
math_abs_64
(   f64 x
)
{
    return fabs ( x );
}

f64
math_floor_64
(   f64 x
)
{
    return floor ( x );
}

f64
math_ceiling_64
(   f64 x
)
{
    return ceil ( x );
}

f64
math_pow_64
(   f64 x
,   f64 y
)
{
    return pow ( x , y );
}

f64
math_sqrt_64
(   f64 x
)
{
    return sqrt ( x );
}

f64
math_exp_64
(   f64 x
)
{
    return exp ( x );
}

f64
math_ln_64
(   f64 x
)
{
    return log ( x );
}

f64
math_log_64
(   f64 x
)
{
    return log10 ( x );
}

f64
math_sin_64
(   f64 x
)
{
    return sin ( x );
}

f64
math_cos_64
(   f64 x
)
{
    return cos ( x );
}

f64
math_tan_64
(   f64 x
)
{
    return tan ( x );
}

f64
math_asin_64
(   f64 x
)
{
    return asin ( x );
}

f64
math_acos_64
(   f64 x
)
{
    return acos ( x );
}

f64
math_atan_64
(   f64 x
)
{
    return atan ( x );
}

f64
math_sinh_64
(   f64 x
)
{
    return sinh ( x );
}

f64
math_cosh_64
(   f64 x
)
{
    return cosh ( x );
}

f64
math_tanh_64
(   f64 x
)
{
    return tanh ( x );
}

i64
math_random_64
( void )
{
    const u64 lo = math_random () & 0xFFFFFFFF;
    const u64 hi = math_random () & 0xFFFFFFFF;
    return lo | ( hi << 32 );
}

i64
math_random2_64
(   i64 min
,   i64 max
)
{
    return ( math_random_64 () % ( max - min + 1 ) ) + min;
}

f64
math_randomf_64
( void )
{
    return math_randomf2_64 ( 0.0F , 1.0F );
}

f64
math_randomf2_64
(   f64 min
,   f64 max
)
{
    return min + ( f64 ) math_random_64 () / ( ( ( f64 ) RAND_MAX ) / ( max - min ) );
}

// End 64-bit math.
////////////////////////////////////////////////////////////////////////////////