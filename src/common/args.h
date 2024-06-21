/**
 * @file common/args.h
 * @brief Defines a preprocessor binding for handling functions which may accept
 * a variable number of arguments.
 */
#ifndef ARGS_H
#define ARGS_H

#include "common/types.h"

/** @brief Type definition for an element in a variadic argument list. */
typedef u64 arg_t;

/** @brief Type definition for a variadic argument list. */
typedef struct
{
    u64     arg_count;
    arg_t*  args;
}
args_t;

/**
 * @brief Preprocessor binding to pass a variable number of arguments to a
 * function using the __VA_ARGS__ preprocessor directive.
 * 
 * Functions which accept this argument list should be defined with a
 * corresponding parameter of the type args_t.
 * 
 * Under GCC and Clang, this will trip the compiler warning -Wint-conversion.
 * The warning may be disabled before using the macro and reenabled afterwards
 * if compiling with -Werror (see common/warn.h).
 * 
 * LIMITATIONS :
 * 
 * Currently doesn't support f32, and f64 must be passed by address, not value;
 * this is due to C's implicit flooring behavior when casting float types to int
 * types.
 * 
 * String literals must be passed by address, not value.
 *
 * EXAMPLE USAGE :
 * 
 *  |  i32 _my_function ( u8 a , u8 b , args_t args );
 *  |
 *  |  #define my_function(a,b,...)                               \
 *  |      ({                                                     \
 *  |          DISABLE_WARNING ( -Wint-conversion )               \
 *  |          _my_function ( (a) , (b) , ARGS ( __VA_ARGS__ ) ); \
 *  |          REENABLE_WARNING ()                                \
 *  |      })
 */
#define ARGS(...)                                                   \
    (( args_t ){ .arg_count = sizeof ( ( arg_t[] ){ __VA_ARGS__ } ) \
                            / sizeof ( arg_t )                      \
               , .args = ( arg_t[] ){ __VA_ARGS__ }                 \
               })

#endif  // ARGS_H
