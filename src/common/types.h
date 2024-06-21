/**
 * @file common/types.h
 * @brief Common type definitions.
 */
#ifndef TYPES_H
#define TYPES_H

// Type definitions for numeric types. These are hardware-specific.
typedef unsigned char           u8;  /** @brief Integer (unsigned, 8-bit). */
typedef unsigned short int      u16; /** @brief Integer (unsigned, 16-bit). */
typedef unsigned int            u32; /** @brief Integer (unsigned, 32-bit). */
typedef unsigned long long int  u64; /** @brief Integer (unsigned, 64-bit). */
typedef signed char             i8;  /** @brief Integer (signed, 8-bit). */
typedef signed short int        i16; /** @brief Integer (signed, 16-bit). */
typedef signed int              i32; /** @brief Integer (signed, 32-bit). */
typedef signed long long int    i64; /** @brief Integer (signed, 64-bit). */
typedef float                   f32; /** @brief Floating point number (32-bit). */
typedef double                  f64; /** @brief Floating point number (64-bit). */

/** @brief Type and instance definitions for generic boolean type. */
typedef _Bool bool;
#define true    ( ( bool ) 1 )
#define false   ( ( bool ) 0 )

/** @brief Type definition for a 32-bit padded boolean type. */
typedef u32 b32;

/**
 * @brief Type definition for a generic comparator function.
 * 
 * @param a Address of a value to compare.
 * @param b Address of a value to compare.
 * @return 0 if a == b; positive value if a < b; negative value if a > b.
 */
typedef i32
( *comparator_function_t )
(   const void* a
,   const void* b
);

// Compile-time assertions.
#include "common/static_assert.h"

// Verify all hardware-specific types have the expected size.
STATIC_ASSERT ( sizeof( u8 )  == 1 , "Expected u8 to be 1 byte." );
STATIC_ASSERT ( sizeof( u16 ) == 2 , "Expected u16 to be 2 bytes." );
STATIC_ASSERT ( sizeof( u32 ) == 4 , "Expected u32 to be 4 bytes." );
STATIC_ASSERT ( sizeof( u64 ) == 8 , "Expected u64 to be 8 bytes." );
STATIC_ASSERT ( sizeof( i8 )  == 1 , "Expected i8 to be 1 byte." );
STATIC_ASSERT ( sizeof( i16 ) == 2 , "Expected i16 to be 2 bytes." );
STATIC_ASSERT ( sizeof( i32 ) == 4 , "Expected i32 to be 4 bytes." );
STATIC_ASSERT ( sizeof( i64 ) == 8 , "Expected i64 to be 8 bytes." );
STATIC_ASSERT ( sizeof( f32 ) == 4 , "Expected f32 to be 4 bytes." );
STATIC_ASSERT ( sizeof( f64 ) == 8 , "Expected f64 to be 8 bytes." );

#endif  // TYPES_H