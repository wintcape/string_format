/**
 * @file core/string.c
 * @brief Implementation of the core/string header.
 * (see core/string.h for additional details)
 */
#include "core/string.h"

#include "core/logger.h"
#include "math/math.h"
#include "platform/platform.h"
#include "platform/memory.h"

/**
 * @brief Primary implementation of string_contains (see string_contains).
 * 
 * Search begins from the start of the string.
 * 
 * @param search The string to search.
 * @param search_length The number of characters in search.
 * @param find The string to find.
 * @param find_length The number of characters in find.
 * @param index Output buffer to hold the index in search at which find was
 * found.
 * @return true if search contains find; false otherwise.
 */
bool
__string_contains
(   const char* search
,   const u64   search_length
,   const char* find
,   const u64   find_length
,   u64*        index
);

/**
 * @brief Primary implementation of string_contains (see string_contains).
 * 
 * Search begins from the end of the string.
 * 
 * @param search The string to search.
 * @param search_length The number of characters in search.
 * @param find The string to find.
 * @param find_length The number of characters in find.
 * @param index Output buffer to hold the index in search at which find was
 * found.
 * @return true if search contains find; false otherwise.
 */
bool
__string_contains_reverse
(   const char* search
,   const u64   search_length
,   const char* find
,   const u64   find_length
,   u64*        index
);

/**
 * @brief Primary implementation of string_i64 and string_u64
 * (see string_i64 and string_u64).
 * 
 * After return, the string at dst will be written in least-significant-bit
 * first order. Since the standard method for printing Arabic-numeral integers
 * is in most-significant-bit first order, the output of this function should be
 * reversed for the purpose of display.
 * 
 * @param value A 64-bit value.
 * @param radix Integer radix in the range [2..36] (inclusive).
 * @param dst Output buffer for string.
 * @return The number of characters written to dst.
 */
u64
_string_u64
(   u64     value
,   u8      radix
,   char*   dst
);

u64
_string_length
(   const char* string
)
{
    return platform_string_length ( string );
}

u64
_string_length_clamped
(   const char* string
,   const u64   limit
)
{
    return platform_string_length_clamped ( string , limit );
}

bool
string_equal
(   const char* s1
,   const u64   s1_length
,   const char* s2
,   const u64   s2_length
)
{
    return ( s1_length == s2_length )
        && ( s1 == s2 || memory_equal ( s1 , s2 , s1_length ) )
        ;
}

bool
string_empty
(   const char* string
)
{
    return string && !( *string );
}

bool
string_contains
(   const char* search
,   const u64   search_length
,   const char* find
,   const u64   find_length
,   bool        reverse
,   u64*        index
)
{
    if ( find_length > search_length )
    {
        return false;
    }
    if ( reverse )
    {
        return __string_contains_reverse ( search , search_length
                                         , find , find_length
                                         , index
                                         );
    }
    return __string_contains ( search , search_length
                             , find , find_length
                             , index
                             );
}

char*
string_reverse
(   char*       string
,   const u64   string_length
)
{
    if ( string_length < 2 )
    {
        return string;
    }
    
    u64 i;
    u64 j;
    char swap;
    for ( i = 0 , j = string_length - 1; i < j; ++i , --j )
    {
        swap = string[ i ];
        string[ i ] = string[ j ];
        string[ j ] = swap;
    }
    return string;
}

u64
string_i64
(   i64     value
,   u8      radix
,   char*   dst
)
{
    if ( radix < STRING_INTEGER_MIN_RADIX || radix > STRING_INTEGER_MAX_RADIX )
    {
        PRINT ( "string_i64: Illegal value for radix argument: %u. Clamping to range [%u..%u].\n"
              , radix
              , STRING_INTEGER_MIN_RADIX
              , STRING_INTEGER_MAX_RADIX
              );
        radix = CLAMP ( radix
                      , STRING_INTEGER_MIN_RADIX
                      , STRING_INTEGER_MAX_RADIX
                      );
    }

    const bool negative = radix == 10 && value < 0;
    if ( negative )
    {
        *dst = '-';
        value = -value;
    }

    u64 length = _string_u64 ( value , radix , dst + negative );
    string_reverse ( dst + negative , length );
    length += negative;
    dst[ length ] = 0; // Append terminator.
    return length;
}

u64
string_u64
(   u64     value
,   u8      radix
,   char*   dst
)
{
    if ( radix < STRING_INTEGER_MIN_RADIX || radix > STRING_INTEGER_MAX_RADIX )
    {
        PRINT ( "string_u64: Illegal value for radix argument: %u. Clamping to range [%u..%u].\n"
              , radix
              , STRING_INTEGER_MIN_RADIX
              , STRING_INTEGER_MAX_RADIX
              );
        radix = CLAMP ( radix
                      , STRING_INTEGER_MIN_RADIX
                      , STRING_INTEGER_MAX_RADIX
                      );
    }

    const u64 length = _string_u64 ( value , radix , dst );
    string_reverse ( dst , length );
    dst[ length ] = 0; // Append terminator.
    return length;
}

u64
string_f64
(   f64     value
,   u8      precision
,   bool    abbreviated
,   char*   dst
)
{
    if ( nan64 ( value ) )
    {
        const char* src = "NaN";
        const u64 src_length = _string_length ( src );
        memory_copy ( dst , src , src_length );
        return src_length;
    }
    if ( !finite64 ( value ) )
    {
        if ( value < 0 )
        {
            const char* src = "-Infinity";
            const u64 src_length = _string_length ( src );
            memory_copy ( dst , src , src_length );
            return src_length;
        }
        else
        {
            const char* src = "Infinity";
            const u64 src_length = _string_length ( src );
            memory_copy ( dst , src , src_length );
            return src_length;
        }
    }

    if ( precision > STRING_FLOAT_MAX_PRECISION )
    {
        PRINT ( "string_f64: Illegal value for precision argument: %u. Clamping to range [%u..%u]."
              , precision
              , 0
              , STRING_FLOAT_MAX_PRECISION
              );
        precision = STRING_FLOAT_MAX_PRECISION;
    }
    
    // Force abbreviation if the integral part of the number is too large or too
    // small to be casted to a 64-bit signed integer.
    if ( value <= -9223372036854775809.00 || value >= 9223372036854775808.00 )
    {
        abbreviated = true;
    }

    u64 length = 0;

    // Abbreviated notation.
    if ( abbreviated && value )
    {
        const u8 radix = 10;
        const i64 exponent = value ? floor64 ( log64 ( abs64 ( value ) ) ) : 0;
        const f64 mantissa = value / pow64 ( radix , exponent );

        // Mantissa portion.
        length += string_f64 ( mantissa , precision , false , dst );

        // Strip the point if required precision is zero.
        if ( !precision )
        {
            u64 index;
            if ( string_contains ( dst , length
                                 , string_char ( '.' ) , 1
                                 , true , &index
                                 ))
            {
                dst[ index ] = 0;
                length -= 1;
            }
        }

        // Append separator.
        dst[ length ] = 'E';
        length += 1;

        // Exponent portion.
        length += string_i64 ( exponent , radix , dst + length );
    }

    // Standard base-10 fractional notation.
    else
    {
        const u8 radix = 10;
        const i64 integer = value;
        const f64 fractional = value - integer;

        // Integral portion.
        length += string_i64 ( integer , radix , dst );

        // Append point.
        dst[ length ] = '.';
        length += 1;

        if ( precision )
        {
            // Fractional portion.
            char fractional_string[ STRING_FLOAT_MAX_PRECISION ];
            const u8 fractional_string_length = string_u64 ( ( ( fractional < 0 ) ? -fractional
                                                                                  : fractional
                                                             ) * pow64 ( radix , precision )
                                                           , radix
                                                           , fractional_string
                                                           );
            for ( u8 i = fractional_string_length; i < precision; ++i )
            {
                dst[ length ] = '0'; // Pad with leading zeroes (if needed).
                length += 1;
            }
            memory_copy ( dst + length
                        , fractional_string
                        , fractional_string_length
                        );
            length += fractional_string_length;
        }
    }

    dst[ length ] = 0; // Append terminator.
    return length;
}

const char*
string_bytesize
(   u64     size
,   f64*    amount
)
{
    if ( size >= GiB ( 1 ) )
    {
        *amount = ( ( f64 ) size ) / GiB ( 1 );
        return "GiB";
    }
    if ( size >= MiB ( 1 ) )
    {
        *amount = ( ( f64 ) size ) / MiB ( 1 );
        return "MiB";
    }
    if ( size >= KiB ( 1 ) )
    {
        *amount = ( ( f64 ) size ) / KiB ( 1 );
        return "KiB";
    }
    *amount = ( f64 ) size;
    return "B";
}

bool
to_i64
(   const char* string
,   u64         string_length
,   i64*        value_
)
{
    u64 value;
    u64 i;
    u64 j;

    // Compute index of first non-whitespace character.
    for ( i = 0; i < string_length && whitespace ( string[ i ] ); ++i );

    // Whitespace-only case.
    if ( i == string_length )
    {
        LOGERROR ( "to_i64: Input string must be a valid i64 value (radix 10)." );
        return false;
    }

    // Parse sign, if present.
    bool negative = false;
    if ( string[ i ] == '-' )
    {
        negative = true;
        i += 1;
    }
    else if ( string[ i ] == '+' )
    {
        i += 1;
    }

    // Sign-only case.
    if ( i == string_length )
    {
        LOGERROR ( "to_i64: Input string must be a valid i64 value (radix 10)." );
        return false;
    }

    // Compute index of first character which is not a leading zero.
    for ( ; i < string_length && string[ i ] == '0'; ++i );

    // Zero-only case.
    if ( i == string_length )
    {
        *value_ = 0;
        return true;
    }

    // Compute index of final non-whitespace character.
    for ( j = string_length; j && whitespace ( string[ j - 1 ] ); --j );

    // Update length.
    string_length = MIN ( j - i , 20U );
    
    // Main parsing loop.
    value = 0;
    for ( j = 0; j < string_length; ++j )
    {
        if ( digit ( string[ i + j ] ) )
        {
            value = 10 * value + to_digit ( string[ i + j ] );
        }
        else
        {
            LOGERROR ( "to_i64: Input string must be a valid i64 value (radix 10)." );
            return false;
        }
    }

    *value_ = negative ? -value : value;
    return true;
}

bool
to_u64
(   const char* string
,   u64         string_length
,   u64*        value_
)
{
    u64 value;
    u64 i;
    u64 j;

    // Compute index of first non-whitespace character.
    for ( i = 0; i < string_length && whitespace ( string[ i ] ); ++i );

    // Whitespace-only case.
    if ( i == string_length )
    {
        LOGERROR ( "to_u64: Input string must be a valid u64 value (radix 10)." );
        return false;
    }

    // Compute index of first character which is not a leading zero.
    for ( ; i < string_length && string[ i ] == '0'; ++i );

    // Zero-only case.
    if ( i == string_length )
    {
        *value_ = 0;
        return true;
    }

    // Compute index of final non-whitespace character.
    for ( j = string_length; j && whitespace ( string[ j - 1 ] ); --j );

    // Update length.
    string_length = MIN ( j - i , 20U );
    
    // Main parsing loop.
    value = 0;
    for ( j = 0; j < string_length; ++j )
    {
        if ( digit ( string[ i + j ] ) )
        {
            value = 10 * value + to_digit ( string[ i + j ] );
        }
        else
        {
            LOGERROR ( "to_u64: Input string must be a valid u64 value (radix 10)." );
            return false;
        }
    }

    *value_ = value;
    return true;
}

bool
to_f64
(   const char* string
,   u64         string_length
,   f64*        value_
)
{
    const char* infinity = "infinity";
    const u64 infinity_length = _string_length ( infinity );
    const char* inf = "inf";
    const u64 inf_length = _string_length ( inf );
    const char* nan = "nan";
    const u64 nan_length = _string_length ( nan );
    char string_[ 9 ];

    f64 value;
    u64 i;
    u64 j;

    // Compute index of first non-whitespace character.
    for ( i = 0; i < string_length && whitespace ( string[ i ] ); ++i );

    // Whitespace-only case.
    if ( i == string_length )
    {
        LOGERROR ( "to_f64: Input string must be a valid f64 value (radix 10)." );
        return false;
    }

    // Parse sign, if present.
    bool negative = false;
    if ( string[ i ] == '-' )
    {
        negative = true;
        i += 1;
    }
    else if ( string[ i ] == '+' )
    {
        i += 1;
    }

    // Sign-only case.
    if ( i == string_length )
    {
        LOGERROR ( "to_f64: Input string must be a valid f64 value (radix 10)." );
        return false;
    }

    // Compute index of final non-whitespace character.
    for ( j = string_length; j && whitespace ( string[ j - 1 ] ); --j );

    // Update length.
    string_length = MIN ( j - i , 64U );

    // Infinity / NaN ? Y/N
    if (   string_length == nan_length
        || string_length == inf_length
        || string_length == infinity_length
       )
    {
        // Ignore case.
        u64 string_length_ = MIN ( 9U , string_length );
        memory_copy ( string_ , string , string_length_ );
        for ( u64 k = 0; k < string_length_; ++k )
        {
            string_[ k ] = to_lowercase ( string[ i + k ] );
        }

        if ( string_equal ( string_ , string_length_
                          , nan , nan_length
                          ))
        {
            *value_ = 1.0 / 0.0; // NaN
            return true;
        }
        else if (   string_equal ( string_ , string_length_ , inf , inf_length )
                 || string_equal ( string_ , string_length_ , infinity , infinity_length )
                )
        {
            *value_ = negative ? -INFINITY_64 : INFINITY_64;
            return true;
        }
    }

    // Compute index of first character which is not a leading zero.
    for ( ; i < j && string[ i ] == '0'; ++i );

    // Zero-only case.
    if ( ( i == j ) || ( ( i == j - 1 ) && string[ i ] == '.' ) )
    {
        *value_ = 0;
        return true;
    }

    // Remove trailing zeroes after decimal point, if present.
    u64 point_index = string_length;
    if ( string_contains ( string + i , string_length
                         , string_char ( '.' ) , 1
                         , false
                         , &point_index
                         ))
    {
        for ( ; string[ i + j - 1 ] == '0'; --j );
        string_length = MIN ( j - i , 64U );
    }

    // CASE: Too large, should be in abbreviated notation.
    // TODO: Handle this properly.
    const u64 integral_length = point_index;
    if ( integral_length > 19 )
    {
        LOGERROR ( "to_f64: Input string must be a valid f64 value (radix 10)." );
        return false;
    }
    
    // Parse integral portion.
    value = 0.0;
    for ( j = 0; j < integral_length; ++j )
    {
        if ( digit ( string[ i + j ] ) )
        {
            value = 10 * value + to_digit ( string[ i + j ] );
        }
        else
        {
            LOGERROR ( "to_f64: Input string must be a valid f64 value (radix 10)." );
            return false;
        }
    }
    
    // Parse fractional portion.
    f64 fractional = 0.0;
    for ( j += 1; j < string_length; ++j )
    {
        if ( digit ( string[ i + j ] ) )
        {
            const u64 base = 10;
            f64 scale = 1.0;
            for ( u64 exponent = integral_length; exponent < j; ++exponent )
            {
                scale /= base;
            }
            fractional += scale * to_digit ( string[ i + j ] );
        }
        else
        {
            LOGERROR ( "to_f64: Input string must be a valid f64 value (radix 10)." );
            return false;
        }
    }
    value += fractional;

    // Apply sign, if needed.
    if ( negative )
    {
        value = -value;
    }

    *value_ = value;
    return true;
}

char*
string_allocate
(   u64 content_size
)
{
    const u64 header_size = sizeof ( u64 );
    const u64 size = header_size + content_size;
    char* string = memory_allocate ( size /* , MEMORY_TAG_STRING */ );
    *( ( u64* ) string ) = size;
    return ( char* )( ( ( u64 ) string ) + header_size );
}

char*
string_allocate_from
(   const char* string
)
{
    const u64 length = _string_length ( string );
    char* copy = string_allocate ( length + 1 );
    memory_copy ( copy , string , length );
    return copy;
}

void
string_free
(   void* string
)
{
    if ( !string )
    {
        return;
    }
    const u64 header_size = sizeof ( u64 );
    string = ( void* )( ( ( u64 ) string ) - header_size );
    memory_free ( string
                // , *( ( u64* ) string )
                // , MEMORY_TAG_STRING
                );
}

bool
__string_contains
(   const char* search
,   const u64   search_length
,   const char* find
,   const u64   find_length
,   u64*        index
)
{
    if ( !find_length )
    {
        if ( index )
        {
            *index = 0;
        }
        return true;
    }
    for ( u64 i = 0; i <= search_length - find_length; ++i )
    {
        if ( search[ i ] != *find )
        {
            continue;
        }
        if ( memory_equal ( search + i + 1 , find + 1 , find_length - 1 ) )
        {
            if ( index )
            {
                *index = i;
            }
            return true;
        }
    }
    return false;
}

bool
__string_contains_reverse
(   const char* search
,   const u64   search_length
,   const char* find
,   const u64   find_length
,   u64*        index
)
{
    if ( !find_length )
    {
        if ( index )
        {
            *index = search_length - 1;
        }
        return true;
    }
    for ( u64 i = search_length - find_length + 1; i; --i )
    {
        if ( search[ i - 1 ] != *find )
        {
            continue;
        }
        if ( memory_equal ( search + i , find + 1 , find_length - 1 ) )
        {
            if ( index )
            {
                *index = i - 1;
            }
            return true;
        }
    }
    return false;
}

u64
_string_u64
(   u64     value
,   u8      radix
,   char*   dst
)
{
    char* i = dst;
    do
    {
        const u8 digit = value % radix;
        value /= radix;
        if ( digit < 10 )
        {
            *i = '0' + digit;
        }
        else
        {
            *i = 'A' + digit - 10;
        }
        i += 1;
    }
    while ( value );
    return i - dst;
}