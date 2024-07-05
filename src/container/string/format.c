/**
 * @file container/string/format.c
 * @brief Implementation of the container/string/format header.
 * (see container/string/format.h for additional details)
 */
#include "container/string/format.h"
#include "container/string.h"

#include "container/array.h"
#include "core/logger.h"
#include "math/math.h"
#include "platform/memory.h"

/** @brief Type definition for a container to hold string token information. */
typedef struct
{
    const char* value;
    u64         length;
}
string_format_token_t;

/** @brief Format specifier token lookup table. */
static const
string_format_token_t
format_specifiers[ STRING_FORMAT_SPECIFIER_COUNT ] =
    {
        { .value = STRING_FORMAT_SPECIFIER_TOKEN_IGNORE
        , .length = sizeof ( STRING_FORMAT_SPECIFIER_TOKEN_IGNORE ) - 1 
        }
    ,   { .value = STRING_FORMAT_SPECIFIER_TOKEN_RAW
        , .length = sizeof ( STRING_FORMAT_SPECIFIER_TOKEN_RAW ) - 1 
        }
    ,   { .value = STRING_FORMAT_SPECIFIER_TOKEN_INTEGER
        , .length = sizeof ( STRING_FORMAT_SPECIFIER_TOKEN_INTEGER ) - 1 
        }
    ,   { .value = STRING_FORMAT_SPECIFIER_TOKEN_FLOATING_POINT
        , .length = sizeof ( STRING_FORMAT_SPECIFIER_TOKEN_FLOATING_POINT ) - 1 
        }
    ,   { .value = STRING_FORMAT_SPECIFIER_TOKEN_FLOATING_POINT_SHOW_FRACTIONAL
        , .length = sizeof ( STRING_FORMAT_SPECIFIER_TOKEN_FLOATING_POINT_SHOW_FRACTIONAL ) - 1 
        }
    ,   { .value = STRING_FORMAT_SPECIFIER_TOKEN_FLOATING_POINT_ABBREVIATED
        , .length = sizeof ( STRING_FORMAT_SPECIFIER_TOKEN_FLOATING_POINT_ABBREVIATED ) - 1 
        }
    ,   { .value = STRING_FORMAT_SPECIFIER_TOKEN_FLOATING_POINT_FRACTIONAL_ONLY
        , .length = sizeof ( STRING_FORMAT_SPECIFIER_TOKEN_FLOATING_POINT_FRACTIONAL_ONLY ) - 1 
        }
    ,   { .value = STRING_FORMAT_SPECIFIER_TOKEN_ADDRESS
        , .length = sizeof ( STRING_FORMAT_SPECIFIER_TOKEN_ADDRESS ) - 1 
        }
    ,   { .value = STRING_FORMAT_SPECIFIER_TOKEN_CHARACTER
        , .length = sizeof ( STRING_FORMAT_SPECIFIER_TOKEN_CHARACTER ) - 1 
        }
    ,   { .value = STRING_FORMAT_SPECIFIER_TOKEN_STRING
        , .length = sizeof ( STRING_FORMAT_SPECIFIER_TOKEN_STRING ) - 1 
        }
    ,   { .value = STRING_FORMAT_SPECIFIER_TOKEN_RESIZABLE_STRING
        , .length = sizeof ( STRING_FORMAT_SPECIFIER_TOKEN_RESIZABLE_STRING ) - 1 
        }
    ,   { .value = STRING_FORMAT_SPECIFIER_TOKEN_FILE_INFO
        , .length = sizeof ( STRING_FORMAT_SPECIFIER_TOKEN_FILE_INFO ) - 1 
        }
    };

/** @brief Format modifier token lookup table. */
static const
string_format_token_t
format_modifiers[ STRING_FORMAT_MODIFIER_COUNT ] =
    {
        { .value = STRING_FORMAT_MODIFIER_TOKEN_PAD
        , .length = sizeof ( STRING_FORMAT_MODIFIER_TOKEN_PAD ) - 1
        }
    ,   { .value = STRING_FORMAT_MODIFIER_TOKEN_PAD_MINIMUM
        , .length = sizeof ( STRING_FORMAT_MODIFIER_TOKEN_PAD_MINIMUM ) - 1
        }
    ,   { .value = STRING_FORMAT_MODIFIER_TOKEN_SHOW_SIGN
        , .length = sizeof ( STRING_FORMAT_MODIFIER_TOKEN_SHOW_SIGN ) - 1
        }
    ,   { .value = STRING_FORMAT_MODIFIER_TOKEN_HIDE_SIGN
        , .length = sizeof ( STRING_FORMAT_MODIFIER_TOKEN_HIDE_SIGN ) - 1
        }
    ,   { .value = STRING_FORMAT_MODIFIER_TOKEN_FIX_PRECISION
        , .length = sizeof ( STRING_FORMAT_MODIFIER_TOKEN_FIX_PRECISION ) - 1
        }
    ,   { .value = STRING_FORMAT_MODIFIER_TOKEN_RADIX
        , .length = sizeof ( STRING_FORMAT_MODIFIER_TOKEN_RADIX ) - 1
        }
    ,   { .value = STRING_FORMAT_MODIFIER_TOKEN_ARRAY
        , .length = sizeof ( STRING_FORMAT_MODIFIER_TOKEN_ARRAY ) - 1
        }
    ,   { .value = STRING_FORMAT_MODIFIER_TOKEN_RESIZABLE_ARRAY
        , .length = sizeof ( STRING_FORMAT_MODIFIER_TOKEN_RESIZABLE_ARRAY ) - 1
        }
    ,   { .value = STRING_FORMAT_MODIFIER_TOKEN_SLICE_BEGIN
        , .length = sizeof ( STRING_FORMAT_MODIFIER_TOKEN_SLICE_BEGIN ) - 1
        }
    };

/** @brief Type and instance definitions for a string padding tag. */
typedef enum
{
    STRING_FORMAT_PADDING_NONE
,   STRING_FORMAT_PADDING_LEFT
,   STRING_FORMAT_PADDING_RIGHT
}
STRING_FORMAT_PADDING;

/** @brief Type definition for a container to hold string padding configuration info. */
typedef struct
{
    STRING_FORMAT_PADDING   tag;
    bool                    fixed;
    u64                     width;

    bool                    wildcard_value;
    string_format_token_t   value;

    bool                    nested;
    string_format_token_t   format;
}
string_format_padding_t;

/** @brief Maximum pad width. */
#define STRING_FORMAT_MAX_PAD_WIDTH 65535

/** @brief Type and instance definitions for a sign tag. */
typedef enum
{
    STRING_FORMAT_SIGN_NONE
,   STRING_FORMAT_SIGN_SHOW
,   STRING_FORMAT_SIGN_HIDE
}
STRING_FORMAT_SIGN;

/** @brief Type definition for a container to hold sign configuration info. */
typedef struct
{
    STRING_FORMAT_SIGN tag;
}
string_format_sign_t;

/** @brief Type definition for a container to hold fix-precision configuration info. */
typedef struct
{
    bool    tag;
    u8      precision;
}
string_format_fix_precision_t;

/** @brief Default floating point decimal precision. */
#define STRING_FORMAT_FLOAT_DEFAULT_PRECISION 6

/** @brief Type definition for a container to hold radix configuration info. */
typedef struct
{
    u8 radix;
}
string_format_radix_t;

/** @brief Default numeric radix. */
#define STRING_FORMAT_DEFAULT_RADIX 10

/** @brief Type definition for a container to hold an index range. */
typedef struct
{
    u64     from;
    u64     to;
}
string_format_range_t;

/** @brief Type and instance definitions for a collection tag. */
typedef enum
{
    STRING_FORMAT_COLLECTION_NONE
,   STRING_FORMAT_COLLECTION_STRING
,   STRING_FORMAT_COLLECTION_RESIZABLE_STRING
,   STRING_FORMAT_COLLECTION_ARRAY
,   STRING_FORMAT_COLLECTION_RESIZABLE_ARRAY
}
STRING_FORMAT_COLLECTION;

/** @brief Type definition for a container to hold string info. */
typedef struct
{
    char*   string;
    u64     length;
}
string_format_string_t;

/** @brief Type definition for a container to hold array info. */
typedef struct
{
    void*   array;
    u64     length;
    u64     stride;
}
string_format_array_t;

/** @brief Type definition for a container to hold collection info. */
typedef struct
{
    STRING_FORMAT_COLLECTION    tag;
    union
    {
        string_format_string_t  string;
        string_format_array_t   array;
    };

    bool                        sliced;
    string_format_range_t       slice;
}
string_format_collection_t;

/** @brief Type definition for a container to hold format specifier info. */
typedef struct
{
    STRING_FORMAT_SPECIFIER         tag;
    u64                             length;

    bool                            modifiers[ STRING_FORMAT_MODIFIER_COUNT ];
    string_format_padding_t         padding;
    string_format_sign_t            sign;
    string_format_radix_t           radix;
    string_format_fix_precision_t   fix_precision;
    string_format_collection_t      collection;
}
string_format_specifier_t;

/** @brief Type definition for internal state. */
typedef struct
{
    const char* format;
    u64         format_length;

    args_t      args;
    arg_t*      next_arg;
    u64         args_remaining;

    const char* copy_start;
    const char* copy_end;

    string_t*   string;
}
state_t;

/** @brief Defines next copy size. */
#define STRING_FORMAT_COPY_SIZE(state) \
    MAX ( 0 , (state)->copy_end - (state)->copy_start )

/** @brief Defines the read limit for the format string. */
#define STRING_FORMAT_READ_LIMIT(state) \
    ( (state)->format + (state)->format_length )

/**
 * @brief Variant of memory_equal which performs a bound check prior to
 * string comparison.
 *
 * @param s1 A string. Must be non-zero.
 * @param s2 A string. Must be non-zero.
 * @param size The number of bytes to compare.
 * @param limit If the sum of s1 and size exceeds this value, the test will
 * fail.
 * @return true if strings are equal, or size exceeds limit; false otherwise.
 */
INLINE
bool
_memory_equal
(   const void* s1
,   const void* s2
,   u64         size
,   const void* limit
)
{
    if ( ( ( void* )( ( ( u64 ) s1 ) + size ) ) > limit )
    {
        return false;
    }
    return memory_equal ( s1 , s2 , size );
}

/**
 * @brief Variant of _string_format which starts processing a format string
 * given an initial working state (see _string_format).
 * 
 * @param state Internal state parameters.
 * @return The formatted string.
 */
string_t*
__string_format
(   state_t* state
);

/**
 * @brief Advances to the next argument in the variadic argument list.
 * 
 * @param state Internal state arguments.
 */
void
_string_format_consume_next_argument
(   state_t* state
);

/**
 * @brief Validates a format specifier token.
 * 
 * @param state Internal state arguments.
 * @param read Current read head.
 * @param format_specifier Output buffer for format specifier
 * (possibly invalidated).
 * @return Format specifier string length.
 */
void
_string_format_validate_format_specifier
(   state_t*                    state
,   const char*                 read
,   string_format_specifier_t*  format_specifier
);

// Implementation of _string_format_validate_format_specifier
// ( see _string_format_validate_format_specifier ).
void _string_format_validate_format_specifier_ignore ( state_t* state , const char** read , string_format_specifier_t* format_specifier );
void _string_format_validate_format_specifier_raw ( state_t* state , const char** read , string_format_specifier_t* format_specifier );
void _string_format_validate_format_specifier_integer ( state_t* state , const char** read , string_format_specifier_t* format_specifier );
void _string_format_validate_format_specifier_floating_point ( state_t* state , const char** read , string_format_specifier_t* format_specifier );
void _string_format_validate_format_specifier_floating_point_show_fractional ( state_t* state , const char** read , string_format_specifier_t* format_specifier );
void _string_format_validate_format_specifier_floating_point_abbreviated ( state_t* state , const char** read , string_format_specifier_t* format_specifier );
void _string_format_validate_format_specifier_floating_point_fractional_only ( state_t* state , const char** read , string_format_specifier_t* format_specifier );
void _string_format_validate_format_specifier_address ( state_t* state , const char** read , string_format_specifier_t* format_specifier );
void _string_format_validate_format_specifier_character ( state_t* state , const char** read , string_format_specifier_t* format_specifier );
void _string_format_validate_format_specifier_string ( state_t* state , const char** read , string_format_specifier_t* format_specifier );
void _string_format_validate_format_specifier_resizable_string ( state_t* state , const char** read , string_format_specifier_t* format_specifier );
void _string_format_validate_format_specifier_file_info ( state_t* state , const char** read , string_format_specifier_t* format_specifier );
void _string_format_validate_format_modifier_pad ( state_t* state , const char** read , const bool fixed , string_format_specifier_t* format_specifier );
void _string_format_validate_format_modifier_sign ( state_t* state , const char** read , STRING_FORMAT_SIGN sign, string_format_specifier_t* format_specifier );
void _string_format_validate_format_modifier_fix_precision ( state_t* state , const char** read , string_format_specifier_t* format_specifier );
void _string_format_validate_format_modifier_radix ( state_t* state , const char** read , string_format_specifier_t* format_specifier );
void _string_format_validate_format_modifier_array ( state_t* state , const char** read , string_format_specifier_t* format_specifier );
void _string_format_validate_format_modifier_resizable_array ( state_t* state , const char** read , string_format_specifier_t* format_specifier );
void _string_format_validate_format_modifier_slice ( state_t* state , const char** read , string_format_specifier_t* format_specifier );

/**
 * @brief Parses and prints the next argument according to the current format
 * specifier and modifier(s).
 * 
 * @param state Internal state arguments.
 * @param format_specifier A format specifier.
 * @param arg The argument to parse, explicitly casted to the applicable type.
 */
void
_string_format_parse_next_argument
(   state_t*                            state
,   const string_format_specifier_t*    format_specifier
);

// Implementation of _string_format_parse_next_argument
// ( see _string_format_parse_next_argument ).
u64 _string_format_parse_argument_ignore ( state_t* state , const string_format_specifier_t* format_specifier , const arg_t arg );
u64 _string_format_parse_argument_raw ( state_t* state , const string_format_specifier_t* format_specifier , const u64 arg );
u64 _string_format_parse_argument_integer ( state_t* state , const string_format_specifier_t* format_specifier , const i64 arg );
u64 _string_format_parse_argument_floating_point ( state_t* state , const string_format_specifier_t* format_specifier , const f64* arg );
u64 _string_format_parse_argument_floating_point_show_fractional ( state_t* state , const string_format_specifier_t* format_specifier , const f64* arg );
u64 _string_format_parse_argument_floating_point_abbreviated ( state_t* state , const string_format_specifier_t* format_specifier , const f64* arg );
u64 _string_format_parse_argument_floating_point_fractional_only ( state_t* state , const string_format_specifier_t* format_specifier , const f64* arg );
u64 _string_format_parse_argument_address ( state_t* state , const string_format_specifier_t* format_specifier , const void* arg );
u64 _string_format_parse_argument_character ( state_t* state , const string_format_specifier_t* format_specifier , const char arg );
u64 _string_format_parse_argument_file_info ( state_t* state , const string_format_specifier_t* format_specifier , file_t* arg );
u64 _string_format_parse_argument_string ( state_t* state , const string_format_specifier_t* format_specifier , const char* arg );
u64 _string_format_parse_argument_array ( state_t* state , const string_format_specifier_t* format_specifier , const void* arg );

/**
 * @brief Wrapper for string_append that respects the left- and right- padding
 * format modifiers, if they are set.
 * 
 * @param string The handle to a resizable string to append to.
 * @param src The string to append.
 * @param src_length The number of characters contained by src.
 * @param format_specifier A format specifier.
 * @return The number of characters written to the string being constructed.
 */
u64
_string_format_append
(   string_t**                          string
,   const char*                         src
,   const u64                           src_length
,   const string_format_specifier_t*    format_specifier
);

string_t*
_string_format
(   const char* format
,   args_t      args
)
{
    if ( !format || ( args.arg_count && !args.args ) )
    {
        if ( !format )
        {
            LOGERROR ( "_string_format: Missing argument: format." );
        }
        if ( args.arg_count && !args.args )
        {
            LOGERROR ( "_string_format: Invalid argument: args. List is null, but count indicates it should contain %u element%s."
                     , args.arg_count
                     , ( args.arg_count > 1 ) ? "s" : ""
                     );
        }
        return string_create_from ( "" );
    }

    state_t state;
    state.format = format;
    state.format_length = _string_length ( state.format );
    state.args = args;
    state.next_arg = state.args.args;
    state.args_remaining = state.args.arg_count;
    state.string = _string_create ( state.format_length + 1 );
    return __string_format ( &state );
}

string_t*
__string_format
(   state_t* state
)
{
    const char* format_specifier_token = STRING_FORMAT_SPECIFIER_TOKEN_ID;
    const u64 format_specifier_token_length = _string_length ( format_specifier_token );
    
    // Parse the format string.
    const char* read = state->format;
    state->copy_start = read;
    for (;;)
    {
        // Out of bounds? Y/N
        if ( read >= STRING_FORMAT_READ_LIMIT ( state ) )
        {
            break;
        }

        // Read until next format specifier reached.
        if ( read[ 0 ] != format_specifier_token[ 0 ] )
        {
            read += 1;
            continue;
        }
        if ( !_memory_equal ( read + 1
                            , format_specifier_token + 1
                            , format_specifier_token_length - 1
                            , STRING_FORMAT_READ_LIMIT ( state )
                            ))
        {
            read += format_specifier_token_length;
            continue;
        }

        // Validate format specifier.
        string_format_specifier_t format_specifier;
        _string_format_validate_format_specifier ( state
                                                 , read + format_specifier_token_length
                                                 , &format_specifier
                                                 );

        // Valid? Y/N                      
        if ( format_specifier.tag == STRING_FORMAT_SPECIFIER_INVALID )
        {
            if ( state->args_remaining )
            {
                _string_format_consume_next_argument ( state );
            }

            // Padding requested? Y/N
            if ( format_specifier.padding.tag != STRING_FORMAT_PADDING_NONE )
            {
                // CASE: The invalid format specifier is a compound padding
                //       format modifier. It acts on arbitrary substrings of the
                //       format string; thus, the entire relevant portion of the
                //       substring should be skipped, rather than just the
                //       format specifier token itself. The length of the
                //       relevant section may be read from the format specifier
                //       length (even though the specifier was invalidated,
                //       _string_format_validate_format_specifier still writes
                //       the length for this case to be handled as shown below).
                if ( format_specifier.padding.nested )
                {
                    read += format_specifier.length;
                }

                // CASE: The invalid format specifier is a multi-character
                //       padding format modifier. It may contain tokens
                //       identical to valid format specifiers; thus, the entire
                //       relevant portion of the substring should be skipped,
                //       rather than just the format specifier token itself. The
                //       length of the relevant section may be read from the
                //       format specifier length (even though the specifier was
                //       invalidated, _string_format_validate_format_specifier
                //       still writes the length for this case to be handled as
                //       shown below).
                else if ( !format_specifier.padding.wildcard_value )
                {
                    read += MAX ( format_specifier.length
                                , format_specifier_token_length
                                );
                }
            }

            // CASE: The invalid format specifier is standalone; thus, only the
            //       format specifier token should be consumed.
            else
            {
                read += format_specifier_token_length;
            }

            continue;
        }

        // Copy from the write position to the current read position.
        state->copy_end = read;
        string_append ( state->string
                      , state->copy_start
                      , STRING_FORMAT_COPY_SIZE ( state )
                      );

        // Ignore? Y/N
        if ( format_specifier.tag == STRING_FORMAT_SPECIFIER_IGNORE )
        {
            // Update the read and write position.
            read += 2 * format_specifier_token_length;
            state->copy_start = read;

            // Append the token.
            string_append ( state->string
                          , STRING_FORMAT_SPECIFIER_TOKEN_IGNORE
                          , format_specifier_token_length
                          );

            continue;
        }

        // Parse and print the next argument according to the format specifier.
        _string_format_parse_next_argument ( state , &format_specifier );

        // Update the read and write position.
        read += format_specifier.length;
        state->copy_start = read;
    }
    
    // Copy from the write position to the end of the format string.
    state->copy_end = STRING_FORMAT_READ_LIMIT ( state );
    string_append ( state->string
                  , state->copy_start
                  , STRING_FORMAT_COPY_SIZE ( state )
                  );
    return state->string;
}

void
_string_format_consume_next_argument
(   state_t* state
)
{
    state->next_arg += 1;
    state->args_remaining -= 1;
}

void
_string_format_validate_format_specifier
(   state_t*                    state
,   const char*                 read
,   string_format_specifier_t*  format_specifier
)
{
    // Out of bounds? Y/N
    if ( read >= STRING_FORMAT_READ_LIMIT ( state ) )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    const char* const read_ = read;

    // Defaults.
    format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID + 1;
    memory_clear ( format_specifier->modifiers //           ^^^ Avoids value matching against STRING_FORMAT_SPECIFIER_INVALID.
                 , sizeof ( bool ) * STRING_FORMAT_MODIFIER_COUNT
                 );
    format_specifier->length = 0;
    format_specifier->padding.tag = STRING_FORMAT_PADDING_NONE;
    format_specifier->padding.width = 0;
    format_specifier->padding.wildcard_value = false;
    format_specifier->padding.value.value = 0;
    format_specifier->padding.value.length = 0;
    format_specifier->padding.nested = false;
    format_specifier->padding.format.value = 0;
    format_specifier->padding.format.length = 0;
    format_specifier->sign.tag = STRING_FORMAT_SIGN_NONE;
    format_specifier->radix.radix = STRING_FORMAT_DEFAULT_RADIX;
    format_specifier->fix_precision.tag = false;
    format_specifier->fix_precision.precision = STRING_FORMAT_FLOAT_DEFAULT_PRECISION;
    format_specifier->collection.tag = STRING_FORMAT_COLLECTION_NONE;
    format_specifier->collection.sliced = false;
    format_specifier->collection.slice.from = 0;
    format_specifier->collection.slice.to = 0;

    // STAGE 1: "Default" case. Determine if the token is an exact match to a
    //          format specifier, i.e. it has no modifiers.

    // Match against format specifier tokens.
    for ( STRING_FORMAT_SPECIFIER i = 0
        ; i < STRING_FORMAT_SPECIFIER_COUNT
        ; ++i
        )
    {
        // CASE: No match.
        if ( read[ 0 ] != format_specifiers[ i ].value[ 0 ] )
        {
            continue;
        }
        if ( !_memory_equal ( read + 1
                            , format_specifiers[ i ].value + 1
                            , format_specifiers[ i ].length - 1
                            , STRING_FORMAT_READ_LIMIT ( state )
                            ))
        {
            continue;
        }

        // Handle special cases: differentiate between format specifiers
        // which have any number of matching characters at the start of the
        // token.
        if ( i == STRING_FORMAT_SPECIFIER_FLOATING_POINT )
        {
            if ( _memory_equal ( read
                               , format_specifiers[ STRING_FORMAT_SPECIFIER_FILE_INFO ].value
                               , format_specifiers[ STRING_FORMAT_SPECIFIER_FILE_INFO ].length
                               , STRING_FORMAT_READ_LIMIT ( state )
                               ))
            {
                _string_format_validate_format_specifier_file_info ( state , &read , format_specifier );
                format_specifier->length = read - read_ + 1;
                return;
            }
        }

        // CASE: Match.
        switch ( i )
        {
            case STRING_FORMAT_SPECIFIER_IGNORE:                         _string_format_validate_format_specifier_ignore ( state , &read , format_specifier )                         ;break;
            case STRING_FORMAT_SPECIFIER_RAW:                            _string_format_validate_format_specifier_raw ( state , &read , format_specifier )                            ;break;
            case STRING_FORMAT_SPECIFIER_INTEGER:                        _string_format_validate_format_specifier_integer ( state , &read , format_specifier )                        ;break;
            case STRING_FORMAT_SPECIFIER_FLOATING_POINT:                 _string_format_validate_format_specifier_floating_point ( state , &read , format_specifier )                 ;break;
            case STRING_FORMAT_SPECIFIER_FLOATING_POINT_SHOW_FRACTIONAL: _string_format_validate_format_specifier_floating_point_show_fractional ( state , &read , format_specifier ) ;break;
            case STRING_FORMAT_SPECIFIER_FLOATING_POINT_ABBREVIATED:     _string_format_validate_format_specifier_floating_point_abbreviated ( state , &read , format_specifier )     ;break;
            case STRING_FORMAT_SPECIFIER_FLOATING_POINT_FRACTIONAL_ONLY: _string_format_validate_format_specifier_floating_point_fractional_only ( state , &read , format_specifier ) ;break;
            case STRING_FORMAT_SPECIFIER_ADDRESS:                        _string_format_validate_format_specifier_address ( state , &read , format_specifier )                        ;break;
            case STRING_FORMAT_SPECIFIER_CHARACTER:                      _string_format_validate_format_specifier_character ( state , &read , format_specifier )                      ;break;
            case STRING_FORMAT_SPECIFIER_STRING:                         _string_format_validate_format_specifier_string ( state , &read , format_specifier )                         ;break;
            case STRING_FORMAT_SPECIFIER_RESIZABLE_STRING:               _string_format_validate_format_specifier_resizable_string ( state , &read , format_specifier )               ;break;
            case STRING_FORMAT_SPECIFIER_FILE_INFO:                      _string_format_validate_format_specifier_file_info ( state , &read , format_specifier )                      ;break;
            default:                                                                                                                                                                   break;
        }

        // Validation complete.
        format_specifier->length = read - read_ + 1;
        return;
    }

    // Out of bounds? Y/N
    if ( read >= STRING_FORMAT_READ_LIMIT ( state ) )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    // STAGE 2: Process up to one of each format modifier.
    //          (This is fine, even though some are mutually-exclusive.)

    for ( STRING_FORMAT_MODIFIER i = 0; i < STRING_FORMAT_MODIFIER_COUNT; ++i )
    {
        // Match against the format modifier tokens.
        for ( STRING_FORMAT_MODIFIER j = 0; j < STRING_FORMAT_MODIFIER_COUNT; ++j )
        {
            // CASE: No match.
            if ( read[ 0 ] != format_modifiers[ j ].value[ 0 ] )
            {
                continue;
            }
            if ( !_memory_equal ( read + 1
                                , format_modifiers[ j ].value + 1
                                , format_modifiers[ j ].length - 1
                                , STRING_FORMAT_READ_LIMIT ( state )
                                ))
            {
                continue;
            }

            // CASE: Match.
            bool k = false;
            switch ( j )
            {
                case STRING_FORMAT_MODIFIER_PAD_LEFT:        _string_format_validate_format_modifier_pad ( state , &read , true , format_specifier )                     ;k = true;break;
                case STRING_FORMAT_MODIFIER_PAD_RIGHT:       _string_format_validate_format_modifier_pad ( state , &read , false , format_specifier )                    ;k = true;break;
                case STRING_FORMAT_MODIFIER_SHOW_SIGN:       _string_format_validate_format_modifier_sign ( state , &read , STRING_FORMAT_SIGN_SHOW , format_specifier ) ;k = true;break;
                case STRING_FORMAT_MODIFIER_HIDE_SIGN:       _string_format_validate_format_modifier_sign ( state , &read , STRING_FORMAT_SIGN_HIDE , format_specifier ) ;k = true;break;
                case STRING_FORMAT_MODIFIER_FIX_PRECISION:   _string_format_validate_format_modifier_fix_precision ( state , &read , format_specifier )                  ;k = true;break;
                case STRING_FORMAT_MODIFIER_RADIX:           _string_format_validate_format_modifier_radix ( state , &read , format_specifier )                          ;k = true;break;
                case STRING_FORMAT_MODIFIER_ARRAY:           _string_format_validate_format_modifier_array ( state , &read , format_specifier )                          ;k = true;break;
                case STRING_FORMAT_MODIFIER_RESIZABLE_ARRAY: _string_format_validate_format_modifier_resizable_array ( state , &read , format_specifier )                ;k = true;break;
                case STRING_FORMAT_MODIFIER_SLICE:           _string_format_validate_format_modifier_slice ( state , &read , format_specifier )                          ;k = true;break;
                default:                                                                                                                                                           break;
            }
            if ( k )
            {
                break;
            }
        }

        // CASE: Compound padding modifier. These act on arbitrary substrings
        //       of the format string and need to be validated immediately.
        //       If one is invalidated, its length still needs to be written so
        //       that the entire nested substring may be skipped.
        if ( format_specifier->padding.nested )
        {
            format_specifier->length = read - read_ + 1;
            if ( format_specifier->tag != STRING_FORMAT_SPECIFIER_INVALID )
            {
                // This can be anything except STRING_FORMAT_SPECIFIER_INVALID
                // or STRING_FORMAT_SPECIFIER_IGNORE.
                format_specifier->tag = STRING_FORMAT_SPECIFIER_STRING;
                return;
            }
        }

        // Invalid? Y/N
        if ( format_specifier->tag == STRING_FORMAT_SPECIFIER_INVALID )
        {
            return;
        }

        // Out of bounds? Y/N
        if ( read >= STRING_FORMAT_READ_LIMIT ( state ) )
        {
            break;
        }
        
        // Match against format specifier tokens.
        for ( STRING_FORMAT_SPECIFIER k = 0
            ; k < STRING_FORMAT_SPECIFIER_COUNT
            ; ++k
            )
        {
            // CASE: No match.
            if ( read[ 0 ] != format_specifiers[ k ].value[ 0 ] )
            {
                continue;
            }
            if ( !_memory_equal ( read + 1
                                , format_specifiers[ k ].value + 1
                                , format_specifiers[ k ].length - 1
                                , STRING_FORMAT_READ_LIMIT ( state )
                                ))
            {
                continue;
            }

            // Handle special cases: differentiate between format specifiers
            // which have any number of matching characters at the start of the
            // token.
            if ( k == STRING_FORMAT_SPECIFIER_FLOATING_POINT )
            {
                if ( _memory_equal ( read
                                   , format_specifiers[ STRING_FORMAT_SPECIFIER_FILE_INFO ].value
                                   , format_specifiers[ STRING_FORMAT_SPECIFIER_FILE_INFO ].length
                                   , STRING_FORMAT_READ_LIMIT ( state )
                                   ))
                {
                    _string_format_validate_format_specifier_file_info ( state , &read , format_specifier );
                    format_specifier->length = read - read_ + 1;
                    return;
                }
            }

            // CASE: Match.
            switch ( k )
            {
                // CASE: Ignore.
                case STRING_FORMAT_SPECIFIER_IGNORE:
                {
                    // Invalidate (modifiers not permitted).
                    format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
                    return;
                }

                case STRING_FORMAT_SPECIFIER_RAW:                            _string_format_validate_format_specifier_raw ( state , &read , format_specifier )                            ;break;
                case STRING_FORMAT_SPECIFIER_INTEGER:                        _string_format_validate_format_specifier_integer ( state , &read , format_specifier )                        ;break;
                case STRING_FORMAT_SPECIFIER_FLOATING_POINT:                 _string_format_validate_format_specifier_floating_point ( state , &read , format_specifier )                 ;break;
                case STRING_FORMAT_SPECIFIER_FLOATING_POINT_SHOW_FRACTIONAL: _string_format_validate_format_specifier_floating_point_show_fractional ( state , &read , format_specifier ) ;break;
                case STRING_FORMAT_SPECIFIER_FLOATING_POINT_ABBREVIATED:     _string_format_validate_format_specifier_floating_point_abbreviated ( state , &read , format_specifier )     ;break;
                case STRING_FORMAT_SPECIFIER_FLOATING_POINT_FRACTIONAL_ONLY: _string_format_validate_format_specifier_floating_point_fractional_only ( state , &read , format_specifier ) ;break;
                case STRING_FORMAT_SPECIFIER_ADDRESS:                        _string_format_validate_format_specifier_address ( state , &read , format_specifier )                        ;break;
                case STRING_FORMAT_SPECIFIER_CHARACTER:                      _string_format_validate_format_specifier_character ( state , &read , format_specifier )                      ;break;
                case STRING_FORMAT_SPECIFIER_STRING:                         _string_format_validate_format_specifier_string ( state , &read , format_specifier )                         ;break;
                case STRING_FORMAT_SPECIFIER_RESIZABLE_STRING:               _string_format_validate_format_specifier_resizable_string ( state , &read , format_specifier )               ;break;
                case STRING_FORMAT_SPECIFIER_FILE_INFO:                      _string_format_validate_format_specifier_file_info ( state , &read , format_specifier )                      ;break;
                default:                                                                                                                                                                   break;
            }

            // Validation complete.
            format_specifier->length = read - read_ + 1;
            return;
        }
    }

    // STAGE 3: The token did not terminate with a format specifier.

    // Invalidate the format specifier.
    format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
}

void
_string_format_validate_format_specifier_ignore
(   state_t*                    state
,   const char**                read
,   string_format_specifier_t*  format_specifier
)
{
    format_specifier->tag = STRING_FORMAT_SPECIFIER_IGNORE;
    *read += format_specifiers[ STRING_FORMAT_SPECIFIER_IGNORE ].length;
}

void
_string_format_validate_format_specifier_raw
(   state_t*                    state
,   const char**                read
,   string_format_specifier_t*  format_specifier
)
{
    if ( !state->args_remaining )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }
    format_specifier->tag = STRING_FORMAT_SPECIFIER_RAW;
    *read += format_specifiers[ STRING_FORMAT_SPECIFIER_RAW ].length;
}

void
_string_format_validate_format_specifier_integer
(   state_t*                    state
,   const char**                read
,   string_format_specifier_t*  format_specifier
)
{
    if ( !state->args_remaining )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }
    format_specifier->tag = STRING_FORMAT_SPECIFIER_INTEGER;
    *read += format_specifiers[ STRING_FORMAT_SPECIFIER_INTEGER ].length;
}

void
_string_format_validate_format_specifier_floating_point
(   state_t*                    state
,   const char**                read
,   string_format_specifier_t*  format_specifier
)
{
    if ( !state->args_remaining )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }
    format_specifier->tag = STRING_FORMAT_SPECIFIER_FLOATING_POINT;
    *read += format_specifiers[ STRING_FORMAT_SPECIFIER_FLOATING_POINT ].length;
}

void
_string_format_validate_format_specifier_floating_point_show_fractional
(   state_t*                    state
,   const char**                read
,   string_format_specifier_t*  format_specifier
)
{
    if ( !state->args_remaining )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }
    format_specifier->tag = STRING_FORMAT_SPECIFIER_FLOATING_POINT_SHOW_FRACTIONAL;
    *read += format_specifiers[ STRING_FORMAT_SPECIFIER_FLOATING_POINT_SHOW_FRACTIONAL ].length;
}

void
_string_format_validate_format_specifier_floating_point_abbreviated
(   state_t*                    state
,   const char**                read
,   string_format_specifier_t*  format_specifier
)
{
    if ( !state->args_remaining )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }
    format_specifier->tag = STRING_FORMAT_SPECIFIER_FLOATING_POINT_ABBREVIATED;
    *read += format_specifiers[ STRING_FORMAT_SPECIFIER_FLOATING_POINT_ABBREVIATED ].length;
}

void
_string_format_validate_format_specifier_floating_point_fractional_only
(   state_t*                    state
,   const char**                read
,   string_format_specifier_t*  format_specifier
)
{
    if ( !state->args_remaining )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }
    format_specifier->tag = STRING_FORMAT_SPECIFIER_FLOATING_POINT_FRACTIONAL_ONLY;
    *read += format_specifiers[ STRING_FORMAT_SPECIFIER_FLOATING_POINT_FRACTIONAL_ONLY ].length;
}

void
_string_format_validate_format_specifier_address
(   state_t*                    state
,   const char**                read
,   string_format_specifier_t*  format_specifier
)
{
    if ( !state->args_remaining )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }
    format_specifier->tag = STRING_FORMAT_SPECIFIER_ADDRESS;
    *read += format_specifiers[ STRING_FORMAT_SPECIFIER_ADDRESS ].length;
}

void
_string_format_validate_format_specifier_character
(   state_t*                    state
,   const char**                read
,   string_format_specifier_t*  format_specifier
)
{
    if ( !state->args_remaining )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }
    format_specifier->tag = STRING_FORMAT_SPECIFIER_CHARACTER;
    *read += format_specifiers[ STRING_FORMAT_SPECIFIER_CHARACTER ].length;
}

void
_string_format_validate_format_specifier_string
(   state_t*                    state
,   const char**                read
,   string_format_specifier_t*  format_specifier
)
{
    if ( !state->args_remaining )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    // Fill out collection info by peeking the corresponding argument.
    if ( format_specifier->collection.tag == STRING_FORMAT_COLLECTION_NONE )
    {
        format_specifier->collection.tag = STRING_FORMAT_COLLECTION_STRING;
        format_specifier->collection.string.string = *( ( char** )( state->next_arg ) );
        format_specifier->collection.string.length = format_specifier->collection.string.string
                                                ? _string_length ( format_specifier->collection.string.string )
                                                : 0
                                                ;

        // Slice? Y/N
        if ( format_specifier->collection.sliced )
        {
            // The upper slice index may now be validated since the string length was
            // retrieved.
            if ( format_specifier->collection.slice.to != ( ( u64 )( -1 ) ) )
            {
                if ( format_specifier->collection.slice.to > format_specifier->collection.string.length )
                {
                    format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
                    return;
                }
            }
            else
            {
                format_specifier->collection.slice.to = format_specifier->collection.string.length;
            }
        }
    }

    // Validation complete.

    format_specifier->tag = STRING_FORMAT_SPECIFIER_STRING;
    *read += format_specifiers[ STRING_FORMAT_SPECIFIER_STRING ].length;
}

void
_string_format_validate_format_specifier_resizable_string
(   state_t*                    state
,   const char**                read
,   string_format_specifier_t*  format_specifier
)
{
    if ( !state->args_remaining )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    // Fill out collection info by peeking the corresponding argument.
    if ( format_specifier->collection.tag == STRING_FORMAT_COLLECTION_NONE )
    {
        format_specifier->collection.tag = STRING_FORMAT_COLLECTION_RESIZABLE_STRING;
        format_specifier->collection.string.string = *( ( string_t** )( state->next_arg ) );
        format_specifier->collection.string.length = format_specifier->collection.string.string
                                                ? string_length ( format_specifier->collection.string.string )
                                                : 0
                                                ;

        // Slice? Y/N
        if ( format_specifier->collection.sliced )
        {
            // The upper slice index may now be validated since the string length was
            // retrieved.
            if ( format_specifier->collection.slice.to != ( ( u64 )( -1 ) ) )
            {
                if ( format_specifier->collection.slice.to > format_specifier->collection.string.length )
                {
                    format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
                    return;
                }
            }
            else
            {
                format_specifier->collection.slice.to = format_specifier->collection.string.length;
            }
        }
    }

    // Validation complete.

    format_specifier->tag = STRING_FORMAT_SPECIFIER_RESIZABLE_STRING;
    *read += format_specifiers[ STRING_FORMAT_SPECIFIER_RESIZABLE_STRING ].length;
}

void
_string_format_validate_format_specifier_file_info
(   state_t*                    state
,   const char**                read
,   string_format_specifier_t*  format_specifier
)
{
    if ( !state->args_remaining )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }
    format_specifier->tag = STRING_FORMAT_SPECIFIER_FILE_INFO;
    *read += format_specifiers[ STRING_FORMAT_SPECIFIER_FILE_INFO ].length;
}

void
_string_format_validate_format_modifier_pad
(   state_t*                    state
,   const char**                read
,   const bool                  fixed
,   string_format_specifier_t*  format_specifier
)
{
    // Already present? Y/N
    if (   format_specifier->modifiers[ STRING_FORMAT_MODIFIER_PAD_LEFT ]
        || format_specifier->modifiers[ STRING_FORMAT_MODIFIER_PAD_RIGHT ]
       )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    *read += 1; // TODO: Account for multi-character tokens.

    // Out of bounds? Y/N
    if ( *read + 3 >= STRING_FORMAT_READ_LIMIT ( state ) )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    STRING_FORMAT_MODIFIER modifier;

    // STAGE 1: Parse alignment.

    // CASE: Pad left.
    if ( _memory_equal ( *read
                       , "l"
                       , sizeof ( "l" ) - 1
                       , STRING_FORMAT_READ_LIMIT ( state )
                       ))
    {
        format_specifier->padding.tag = STRING_FORMAT_PADDING_LEFT;
        modifier = STRING_FORMAT_MODIFIER_PAD_LEFT;
        *read += sizeof ( "l" ) - 1;
    }

    // CASE: Pad right.
    else if ( _memory_equal ( *read
                            , "r"
                            , sizeof ( "r" ) - 1
                            , STRING_FORMAT_READ_LIMIT ( state )
                            ))
    {
        format_specifier->padding.tag = STRING_FORMAT_PADDING_RIGHT;
        modifier = STRING_FORMAT_MODIFIER_PAD_RIGHT;
        *read += sizeof ( "r" ) - 1;
    }

    // CASE: Wildcard (from argument).
    else if ( _memory_equal ( *read
                            , "?"
                            , sizeof ( "?" ) - 1
                            , STRING_FORMAT_READ_LIMIT ( state )
                            ))
    {
        // Arguments remaining? Y/N
        if ( !state->args_remaining )
        {
            format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
            return;
        }

        // Retrieve the value of the next argument in the variadic argument list.
        // Expects a string matching either "l" or "r".
        char* alignment = *( ( char** )( state->next_arg ) );

        // Consume the argument that was just parsed.
        _string_format_consume_next_argument ( state );

        *read += sizeof ( "?" ) - 1;

        // CASE: Pad left.
        if ( _memory_equal ( alignment
                           , "l"
                           , sizeof ( "l" ) - 1
                           , STRING_FORMAT_READ_LIMIT ( state )
                           ))
        {
            format_specifier->padding.tag = STRING_FORMAT_PADDING_LEFT;
            modifier = STRING_FORMAT_MODIFIER_PAD_LEFT;
        }

        // CASE: Pad right.
        else if ( _memory_equal ( alignment
                                , "r"
                                , sizeof ( "r" ) - 1
                                , STRING_FORMAT_READ_LIMIT ( state )
                                ))
        {
            format_specifier->padding.tag = STRING_FORMAT_PADDING_RIGHT;
            modifier = STRING_FORMAT_MODIFIER_PAD_RIGHT;
        }
        
        // CASE: Other (invalid).
        else
        {
            format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
            return;
        }
    }

    // CASE: Other (invalid).
    else
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }
    
    // STAGE 2: Parse pad string.

    // Possible escape? Y/N
    if ( **read == '\\' )
    {
        format_specifier->padding.value.length = 1;
        *read += 1;

        // CASE: Escape single-character wildcard (i.e. `?`).
        if ( _memory_equal ( *read
                           , "?"
                           , sizeof ( "?" ) - 1
                           , STRING_FORMAT_READ_LIMIT ( state )
                           ))
        {
            format_specifier->padding.value.value = "?";
            *read += sizeof ( "?" ) - 1;
        }

        // CASE: Escape multi-character delimiter (i.e. `'`).
        else if ( _memory_equal ( *read
                                , "'"
                                , sizeof ( "'" ) - 1
                                , STRING_FORMAT_READ_LIMIT ( state )
                                ))
        {
            format_specifier->padding.value.value = "'";
            *read += sizeof ( "'" ) - 1;
        }

        // CASE: No escape (i.e. `\`).
        else
        {
            format_specifier->padding.value.value = "\\";
        }
    }

    // CASE: Wildcard (single character from argument).
    else if ( _memory_equal ( *read
                            , "?"
                            , sizeof ( "?" ) - 1
                            , STRING_FORMAT_READ_LIMIT ( state )
                            ))
    {
        // Arguments remaining? Y/N
        if ( !state->args_remaining )
        {
            format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
            return;
        }

        // Retrieve the value of the next argument in the variadic argument list.
        // Expects an ASCII character.
        format_specifier->padding.value.value = ( ( char* )( state->next_arg ) );
        format_specifier->padding.value.length = 1;
        format_specifier->padding.wildcard_value = true;

        // Consume the argument that was just parsed.
        _string_format_consume_next_argument ( state );

        *read += sizeof ( "?" ) - 1;
    }

    // Multi-character padding? Y/N
    else if ( _memory_equal ( *read
                            , "'"
                            , sizeof ( "'" ) - 1
                            , STRING_FORMAT_READ_LIMIT ( state )
                            ))
    {
        *read += sizeof ( "'" ) - 1;

        // Out of bounds? Y/N
        if (  *read
            + ( sizeof ( "'" ) - 1 )
            + 1 
            >= STRING_FORMAT_READ_LIMIT ( state )
           )
        {
            format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
            return;
        }

        // CASE: Multi-character wildcard (null-terminated string from argument).
        if ( _memory_equal ( *read
                           , "?'"
                           , sizeof ( "?'" ) - 1
                           , STRING_FORMAT_READ_LIMIT ( state )
                           ))
        {
            // Arguments remaining? Y/N
            if ( !state->args_remaining )
            {
                format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
                return;
            }

            // Retrieve the value of the next argument in the variadic argument list.
            // Expects a null-terminated string.
            format_specifier->padding.value.value = *( ( char** )( state->next_arg ) );
            format_specifier->padding.value.length = format_specifier->padding.value.value
                                                   ? _string_length ( format_specifier->padding.value.value )
                                                   : 0
                                                   ;
            format_specifier->padding.wildcard_value = true;

            // Consume the argument that was just parsed.
            _string_format_consume_next_argument ( state );

            *read += sizeof ( "?'" ) - 1;
        }

        // CASE: Escape multi-character wildcard (i.e. `'?'`).
        else if ( _memory_equal ( *read
                                , "\\?'"
                                , sizeof ( "\\?'" ) - 1
                                , STRING_FORMAT_READ_LIMIT ( state )
                                ))
        {
            format_specifier->padding.value.value = "?";
            format_specifier->padding.value.length = 1;
            *read += sizeof ( "\\?'" ) - 1;
        }

        else
        {
            format_specifier->padding.value.value = *read;

            // Validate that the padding string terminates.
            while (   format_specifier->padding.value.length < STRING_FORMAT_MAX_PAD_WIDTH
                   && *read < STRING_FORMAT_READ_LIMIT ( state )
                  )
            {
                // Terminator encountered? Y/N
                if ( **read == '\'' )
                {
                    {
                        // Escape? Y/N
                        if ( *( *read - 1 ) != '\\' )
                        {
                            break;
                        }
                    }
                }

                *read += 1;
            }

            format_specifier->padding.value.length = *read
                                                   - format_specifier->padding.value.value
                                                   ;
            *read += sizeof ( "'" ) - 1;
        }
    }

    // CASE: Single character from format string.
    else
    {
        format_specifier->padding.value.value = *read;
        format_specifier->padding.value.length = 1;
        *read += 1;
    }

    // Out of bounds? Y/N
    if ( *read >= STRING_FORMAT_READ_LIMIT ( state ) )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    // Invalidate if the padding string is empty.
    if ( !format_specifier->padding.value.length )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    // STAGE 3: Parse width.

    // CASE: Wildcard (from argument).
    if ( _memory_equal ( *read
                       , "?"
                       , sizeof ( "?" ) - 1
                       , STRING_FORMAT_READ_LIMIT ( state )
                       ))
    {
        // Arguments remaining? Y/N
        if ( !state->args_remaining )
        {
            format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
            return;
        }

        // Retrieve the value of the next argument in the variadic argument list.
        // Expects an unsigned integer.
        format_specifier->padding.width = MIN ( *( ( u64* )( state->next_arg ) )
                                              , ( u64 ) STRING_FORMAT_MAX_PAD_WIDTH
                                              );

        // Consume the argument that was just parsed.
        _string_format_consume_next_argument ( state );

        *read += sizeof ( "?" ) - 1;
    }

    // CASE: From format string.
    else
    {
        // Invalidate if there is not at least one digit present.
        if ( !digit ( **read ) )
        {
            format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
            return;
        }

        // Parse the high-order digit.
        format_specifier->padding.width = to_digit ( **read );
        *read += 1;

        // Parse the low-order digits, if present.
        while (   format_specifier->padding.width < STRING_FORMAT_MAX_PAD_WIDTH
               && *read < STRING_FORMAT_READ_LIMIT ( state )
              )
        {
            if ( !digit ( **read ) )
            {
                break;
            }
            format_specifier->padding.width = 10
                                            * format_specifier->padding.width
                                            + to_digit ( **read )
                                            ;
            *read += 1;
        }
    }

    // Invalidate if the pad width is zero.
    if ( !format_specifier->padding.width )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    // STAGE 4: Parse the format substring, if present.

    // Out of bounds? Y/N
    if ( *read >= STRING_FORMAT_READ_LIMIT ( state ) )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    // CASE: No nesting braces present.
    //       (Validation complete).
    if ( **read != '{' )
    {
        format_specifier->padding.fixed = fixed;
        format_specifier->modifiers[ modifier ] = true;
        return;
    }
    *read += 1;

    // Out of bounds? Y/N
    if ( *read >= STRING_FORMAT_READ_LIMIT ( state ) )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    // CASE: Nesting brace(s) present.
    //       Stop validating when the terminating brace is reached.
    format_specifier->padding.nested = true;
    format_specifier->padding.format.value = *read;
    u64 depth = 1;
    while ( depth )
    {
        // Increment depth for each opening brace.
        if ( **read == '{' && *( *read - 1 ) != '\\' )
        {
            depth += 1;
        }

        // Decrement depth for each closing brace.
        else if ( **read == '}' && *( *read - 1 ) != '\\' )
        {
            depth -= 1;
        }

        // Out of bounds? Y/N
        *read += 1;
        if ( *read >= STRING_FORMAT_READ_LIMIT ( state ) )
        {
            break;
        }
    }

    // Invalidate if depth is non-zero (i.e. unbalanced braces).
    if ( depth )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    // Validation complete.

    format_specifier->padding.format.length = *read
                                            - format_specifier->padding.format.value
                                            - 1
                                            ;
    format_specifier->padding.fixed = fixed;
    format_specifier->modifiers[ modifier ] = true;
}

void
_string_format_validate_format_modifier_sign
(   state_t*                    state
,   const char**                read
,   STRING_FORMAT_SIGN          sign
,   string_format_specifier_t*  format_specifier
)
{
    if ( sign == STRING_FORMAT_SIGN_NONE )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    // Already present? Y/N
    if (   format_specifier->modifiers[ STRING_FORMAT_MODIFIER_HIDE_SIGN ]
        || format_specifier->modifiers[ STRING_FORMAT_MODIFIER_SHOW_SIGN ]
       )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    // CASE: Hide sign.
    if ( sign == STRING_FORMAT_SIGN_HIDE )
    {
        format_specifier->modifiers[ STRING_FORMAT_MODIFIER_HIDE_SIGN ] = true;
        *read += format_modifiers[ STRING_FORMAT_MODIFIER_HIDE_SIGN ].length;
    }

    // CASE: Show sign.
    else
    {
        format_specifier->modifiers[ STRING_FORMAT_MODIFIER_SHOW_SIGN ] = true;
        *read += format_modifiers[ STRING_FORMAT_MODIFIER_SHOW_SIGN ].length;
    }

    // Validation complete.

    format_specifier->sign.tag = sign;
}

void
_string_format_validate_format_modifier_fix_precision
(   state_t*                    state
,   const char**                read
,   string_format_specifier_t*  format_specifier
)
{
    // Already present? Y/N
    if ( format_specifier->modifiers[ STRING_FORMAT_MODIFIER_FIX_PRECISION ] )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    *read += format_modifiers[ STRING_FORMAT_MODIFIER_FIX_PRECISION ].length;

    // Out of bounds? Y/N
    if ( *read >= STRING_FORMAT_READ_LIMIT ( state ) )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    // CASE: Explicit precision requested.
    if ( digit ( **read ) )
    {
        // Parse the high-order digit.
        format_specifier->fix_precision.tag = true;
        format_specifier->fix_precision.precision = to_digit ( **read );
        *read += 1;

        // Out of bounds? Y/N
        if ( *read >= STRING_FORMAT_READ_LIMIT ( state ) )
        {
            format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
            return;
        }

        // Parse the low-order digit, if present.
        if ( digit ( **read ) )
        {
            format_specifier->fix_precision.precision = 10
                                                      * format_specifier->fix_precision.precision
                                                      + to_digit ( **read )
                                                      ;
            *read += 1;
        }

        // Invalidate if the requested precision is greater than the maximum
        // allowed by string_f64 (see core/string.h).
        if ( format_specifier->fix_precision.precision > STRING_FLOAT_MAX_PRECISION )
        {
            format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
            return;
        }
    }

    // CASE: Wildcard (from argument).
    else if ( _memory_equal ( *read
                            , "?"
                            , sizeof ( "?" ) - 1
                            , STRING_FORMAT_READ_LIMIT ( state )
                            ))
    {
        // Arguments remaining? Y/N
        if ( !state->args_remaining )
        {
            format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
            return;
        }

        // Retrieve the value of the next argument in the variadic argument list.
        // Expects an unsigned integer.
        format_specifier->fix_precision.precision = *( ( u64* )( state->next_arg ) );

        // Consume the argument that was just parsed.
        _string_format_consume_next_argument ( state );

        *read += sizeof ( "?" ) - 1;

        // Invalidate if the requested precision is greater than the maximum
        // allowed by string_f64 (see core/string.h).
        if ( format_specifier->fix_precision.precision > STRING_FLOAT_MAX_PRECISION )
        {
            format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
            return;
        }
    }

    // CASE: No explicit precision requested (use default).
    else
    {
        format_specifier->fix_precision.tag = false;
        format_specifier->fix_precision.precision = STRING_FORMAT_FLOAT_DEFAULT_PRECISION;
    }

    // Validation complete.

    format_specifier->modifiers[ STRING_FORMAT_MODIFIER_FIX_PRECISION ] = true;
}

void
_string_format_validate_format_modifier_radix
(   state_t*                    state
,   const char**                read
,   string_format_specifier_t*  format_specifier
)
{
    // Already present? Y/N
    if ( format_specifier->modifiers[ STRING_FORMAT_MODIFIER_RADIX ] )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    *read += format_modifiers[ STRING_FORMAT_MODIFIER_RADIX ].length;

    // Out of bounds? Y/N
    if ( *read >= STRING_FORMAT_READ_LIMIT ( state ) )
    {
        return;
    }

    // CASE: Wildcard (from argument).
    if ( _memory_equal ( *read
                       , "?"
                       , sizeof ( "?" ) - 1
                       , STRING_FORMAT_READ_LIMIT ( state )
                       ))
    {
        // Arguments remaining? Y/N
        if ( !state->args_remaining )
        {
            format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
            return;
        }

        // Retrieve the value of the next argument in the variadic argument list.
        // Expects an unsigned integer.
        format_specifier->radix.radix = *( ( u64* )( state->next_arg ) );

        // Consume the argument that was just parsed.
        _string_format_consume_next_argument ( state );

        *read += sizeof ( "?" ) - 1;
    }

    // CASE: From format string.
    else
    {
        // Invalidate if there is not at least one digit present.
        if ( !digit ( **read ) )
        {
            format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
            return;
        }

        // Parse the high-order digit.
        format_specifier->radix.radix = to_digit ( **read );
        *read += 1;

        // Out of bounds? Y/N
        if ( *read >= STRING_FORMAT_READ_LIMIT ( state ) )
        {
            format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
            return;
        }

        // Parse the low-order digit, if present.
        if ( digit ( **read ) )
        {
            format_specifier->radix.radix = 10
                                          * format_specifier->radix.radix
                                          + to_digit ( **read )
                                          ;
            *read += 1;
        }
    }

    // Invalidate if the requested radix does not lie within the bounds
    // allowed by string_i64 (see core/string.h).
    if (   format_specifier->radix.radix < STRING_INTEGER_MIN_RADIX
        || format_specifier->radix.radix > STRING_INTEGER_MAX_RADIX
       )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    // Validation complete.

    format_specifier->modifiers[ STRING_FORMAT_MODIFIER_RADIX ] = true;
}

void
_string_format_validate_format_modifier_array
(   state_t*                    state
,   const char**                read
,   string_format_specifier_t*  format_specifier
)
{
    // Already present? Y/N
    if (   format_specifier->modifiers[ STRING_FORMAT_MODIFIER_ARRAY ]
        || format_specifier->modifiers[ STRING_FORMAT_MODIFIER_RESIZABLE_ARRAY ]
       )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    *read += format_modifiers[ STRING_FORMAT_MODIFIER_ARRAY ].length;

    // Enough arguments present? Y/N
    if ( state->args_remaining < 3 )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    // Fill out collection info by peeking the corresponding arguments.
    format_specifier->collection.tag = STRING_FORMAT_COLLECTION_ARRAY;
    format_specifier->collection.array.array = *( ( void** )( state->next_arg ) );
    format_specifier->collection.array.length = state->next_arg[ 1 ];
    format_specifier->collection.array.stride = state->next_arg[ 2 ];

    // Slice? Y/N
    if ( format_specifier->collection.sliced )
    {
        // The upper slice index may now be validated since the array length was
        // retrieved.
        if ( format_specifier->collection.slice.to != ( ( u64 )( -1 ) ) )
        {
            if ( format_specifier->collection.slice.to > format_specifier->collection.array.length )
            {
                format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
                return;
            }
        }
        else
        {
            format_specifier->collection.slice.to = format_specifier->collection.array.length;
        }
    }

    // Validation complete.
    
    format_specifier->modifiers[ STRING_FORMAT_MODIFIER_ARRAY ] = true;
}

void
_string_format_validate_format_modifier_resizable_array
(   state_t*                    state
,   const char**                read
,   string_format_specifier_t*  format_specifier
)
{
    // Already present? Y/N
    if (   format_specifier->modifiers[ STRING_FORMAT_MODIFIER_ARRAY ]
        || format_specifier->modifiers[ STRING_FORMAT_MODIFIER_RESIZABLE_ARRAY ]
       )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    *read += format_modifiers[ STRING_FORMAT_MODIFIER_RESIZABLE_ARRAY ].length;

    // Enough arguments present? Y/N
    if ( !state->args_remaining )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    // Fill out collection info by peeking the corresponding arguments.
    format_specifier->collection.tag = STRING_FORMAT_COLLECTION_RESIZABLE_ARRAY;
    format_specifier->collection.array.array = *( ( array_t** )( state->next_arg ) );
    format_specifier->collection.array.length = format_specifier->collection.array.array
                                              ? array_length ( format_specifier->collection.array.array )
                                              : 0
                                              ;
    format_specifier->collection.array.stride = format_specifier->collection.array.array
                                              ? array_stride ( format_specifier->collection.array.array )
                                              : 0
                                              ;

    // Slice? Y/N
    if ( format_specifier->collection.sliced )
    {
        // The upper slice index may now be validated since the array length was
        // retrieved.
        if ( format_specifier->collection.slice.to != ( ( u64 )( -1 ) ) )
        {
            if ( format_specifier->collection.slice.to > format_specifier->collection.array.length )
            {
                format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
                return;
            }
        }
        else
        {
            format_specifier->collection.slice.to = format_specifier->collection.array.length;
        }
    }

    // Validation complete.

    format_specifier->modifiers[ STRING_FORMAT_MODIFIER_RESIZABLE_ARRAY ] = true;
}

void
_string_format_validate_format_modifier_slice
(   state_t*                    state
,   const char**                read
,   string_format_specifier_t*  format_specifier
)
{
    // Already present? Y/N
    if ( format_specifier->modifiers[ STRING_FORMAT_MODIFIER_SLICE ] )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    *read += format_modifiers[ STRING_FORMAT_MODIFIER_SLICE ].length;

    // Out of bounds? Y/N
    if (  *read
        + ( sizeof ( STRING_FORMAT_MODIFIER_TOKEN_SLICE_END ) - 1 )
        + 1
        >= STRING_FORMAT_READ_LIMIT ( state )
       )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    // Enough arguments present? Y/N
    if ( !state->args_remaining )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    // STAGE 1: Parse 'from' index.
 
    if ( !digit ( **read ) )
    {
        // CASE: No index provided (use first).
        if ( _memory_equal ( *read
                           , STRING_FORMAT_MODIFIER_TOKEN_SLICE_INDEX_SEPARATOR
                           , sizeof ( STRING_FORMAT_MODIFIER_TOKEN_SLICE_INDEX_SEPARATOR ) - 1
                           , STRING_FORMAT_READ_LIMIT ( state )
                           ))
        {
            format_specifier->collection.slice.from = 0;
            *read += sizeof ( STRING_FORMAT_MODIFIER_TOKEN_SLICE_INDEX_SEPARATOR ) - 1;
        }

        // CASE: Wildcard (from argument).
        else if ( _memory_equal ( *read
                                , "?" STRING_FORMAT_MODIFIER_TOKEN_SLICE_INDEX_SEPARATOR
                                , sizeof ( "?" STRING_FORMAT_MODIFIER_TOKEN_SLICE_INDEX_SEPARATOR ) - 1
                                , STRING_FORMAT_READ_LIMIT ( state )
                                ))
        {
            // Arguments remaining? Y/N
            if ( !state->args_remaining )
            {
                format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
                return;
            }

            // Retrieve the value of the next argument in the variadic argument list.
            // Expects an unsigned integer.
            format_specifier->collection.slice.from = *( ( u64* )( state->next_arg ) );

            // Consume the argument that was just parsed.
            _string_format_consume_next_argument ( state );

            *read += sizeof ( "?" STRING_FORMAT_MODIFIER_TOKEN_SLICE_INDEX_SEPARATOR ) - 1;
        }

        // CASE: Other non-numeric character (invalid).
        else
        {
            format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
            return;
        }
    }
    else
    {
        // CASE: Explicit index provided.
        do
        {
            // CASE: Continue.
            if ( digit ( **read ) )
            {
                format_specifier->collection.slice.from = 10
                                                        * format_specifier->collection.slice.from
                                                        + to_digit ( **read )
                                                        ;
                *read += 1;
            }

            // CASE: Done (index separator provided).
            else if ( _memory_equal ( *read
                                    , STRING_FORMAT_MODIFIER_TOKEN_SLICE_INDEX_SEPARATOR
                                    , sizeof ( STRING_FORMAT_MODIFIER_TOKEN_SLICE_INDEX_SEPARATOR ) - 1
                                    , STRING_FORMAT_READ_LIMIT ( state )
                                    ))
            {
                *read += sizeof ( STRING_FORMAT_MODIFIER_TOKEN_SLICE_INDEX_SEPARATOR ) - 1;
                break;
            }

            // CASE: Done (no index separator provided).
            else if ( _memory_equal ( *read
                                    , STRING_FORMAT_MODIFIER_TOKEN_SLICE_END
                                    , sizeof ( STRING_FORMAT_MODIFIER_TOKEN_SLICE_END ) - 1
                                    , STRING_FORMAT_READ_LIMIT ( state )
                                    ))
            {
                break;
            }

            // CASE: Done (invalid).
            else
            {
                format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
                return;
            }
        }
        while ( *read < STRING_FORMAT_READ_LIMIT ( state ) );
    }

    // Out of bounds? Y/N
    if ( *read >= STRING_FORMAT_READ_LIMIT ( state ) )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    // STAGE 2: Parse 'to' index.

    if ( !digit ( **read ) )
    {
        // CASE: No index provided.
        if ( _memory_equal ( *read
                           , STRING_FORMAT_MODIFIER_TOKEN_SLICE_END
                           , sizeof ( STRING_FORMAT_MODIFIER_TOKEN_SLICE_END ) - 1
                           , STRING_FORMAT_READ_LIMIT ( state )
                           ))
        {
            // CASE: Index separator provided (use last).
            if ( _memory_equal ( *read - ( sizeof ( STRING_FORMAT_MODIFIER_TOKEN_SLICE_INDEX_SEPARATOR ) - 1 )
                               , STRING_FORMAT_MODIFIER_TOKEN_SLICE_INDEX_SEPARATOR
                               , sizeof ( STRING_FORMAT_MODIFIER_TOKEN_SLICE_INDEX_SEPARATOR ) - 1
                               , STRING_FORMAT_READ_LIMIT ( state )
                               ))
            {
                // Uses -1 as a placeholder value to indicate that the 
                // collection length must be written once known during the
                // parsing stage.
                // TODO: Find a cleaner way of handling this.
                format_specifier->collection.slice.to = ( ( u64 )( -1 ) );
            }

            // CASE: No index separator provided (use single index range).
            else
            {
                format_specifier->collection.slice.to = format_specifier->collection.slice.from
                                                      + 1
                                                      ;
            }

            *read += sizeof ( STRING_FORMAT_MODIFIER_TOKEN_SLICE_END ) - 1;
        }

        // CASE: Wildcard (from argument).
        else if ( _memory_equal ( *read
                                , "?" STRING_FORMAT_MODIFIER_TOKEN_SLICE_END
                                , sizeof ( "?" STRING_FORMAT_MODIFIER_TOKEN_SLICE_END ) - 1
                                , STRING_FORMAT_READ_LIMIT ( state )
                                ))
        {
            // Arguments remaining? Y/N
            if ( !state->args_remaining )
            {
                format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
                return;
            }

            // Retrieve the value of the next argument in the variadic argument list.
            // Expects an unsigned integer.
            format_specifier->collection.slice.to = *( ( u64* )( state->next_arg ) );

            // Consume the argument that was just parsed.
            _string_format_consume_next_argument ( state );

            *read += sizeof ( "?" STRING_FORMAT_MODIFIER_TOKEN_SLICE_END ) - 1;
        }

        // CASE: Other non-numeric character (invalid).
        else
        {
            format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
            return;
        }
    }
    else
    {
        // CASE: Explicit index provided.
        do
        {
            // CASE: Continue.
            if ( digit ( **read ) )
            {
                format_specifier->collection.slice.to = 10
                                                      * format_specifier->collection.slice.to
                                                      + to_digit ( **read )
                                                      ;
                *read += 1;
            }

            // CASE: Done (valid).
            else if ( _memory_equal ( *read
                                    , STRING_FORMAT_MODIFIER_TOKEN_SLICE_END
                                    , sizeof ( STRING_FORMAT_MODIFIER_TOKEN_SLICE_END ) - 1
                                    , STRING_FORMAT_READ_LIMIT ( state )
                                    ))
            {
                *read += sizeof ( STRING_FORMAT_MODIFIER_TOKEN_SLICE_END ) - 1;
                break;
            }

            // CASE: Done (invalid).
            else
            {
                format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
                return;
            }
        }
        while ( *read < STRING_FORMAT_READ_LIMIT ( state ) );
    }

    // Out of bounds? Y/N
    if ( *read >= STRING_FORMAT_READ_LIMIT ( state ) )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    // Validate the provided range.
    if (  format_specifier->collection.slice.from
        > format_specifier->collection.slice.to
       )
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    // STAGE 3: Validate collection.
    // TODO: Handle this in a more extensible manner.
    
    // CASE: String (fixed-length).
    if ( _memory_equal ( *read
                       , format_specifiers[ STRING_FORMAT_SPECIFIER_STRING ].value
                       , format_specifiers[ STRING_FORMAT_SPECIFIER_STRING ].length
                       , STRING_FORMAT_READ_LIMIT ( state )
                       ))
    {}

    // CASE: String (resizable).
    else if ( _memory_equal ( *read
                            , format_specifiers[ STRING_FORMAT_SPECIFIER_RESIZABLE_STRING ].value
                            , format_specifiers[ STRING_FORMAT_SPECIFIER_RESIZABLE_STRING ].length
                            , STRING_FORMAT_READ_LIMIT ( state )
                            ))
    {}

    // CASE: Array (fixed-length).
    else if ( _memory_equal ( *read
                            , format_modifiers[ STRING_FORMAT_MODIFIER_ARRAY ].value
                            , format_modifiers[ STRING_FORMAT_MODIFIER_ARRAY ].length
                            , STRING_FORMAT_READ_LIMIT ( state )
                            ))
    {}

    // CASE: Array (resizable).
    else if ( _memory_equal ( *read
                            , format_modifiers[ STRING_FORMAT_MODIFIER_RESIZABLE_ARRAY ].value
                            , format_modifiers[ STRING_FORMAT_MODIFIER_RESIZABLE_ARRAY ].length
                            , STRING_FORMAT_READ_LIMIT ( state )
                            ))
    {}

    // CASE: Other (invalid).
    else
    {
        format_specifier->tag = STRING_FORMAT_SPECIFIER_INVALID;
        return;
    }

    // Validation complete.
    format_specifier->collection.sliced = true;
    format_specifier->modifiers[ STRING_FORMAT_MODIFIER_SLICE ] = true;
}

void
_string_format_parse_next_argument
(   state_t*                            state
,   const string_format_specifier_t*    format_specifier
)
{
    // CASE: Nested padding format modifier present?
    //       YES  =>  Parse and print the nested format string first
    //                (i.e. recursive case).
    //       NO   =>  Parse and print normally (i.e. base case).
    if ( format_specifier->padding.nested )
    {
        // Generate and append the nested format substring.
        string_t* string;
        state_t state_;
        state_.format = format_specifier->padding.format.value;
        state_.format_length = format_specifier->padding.format.length;
        state_.args = state->args;
        state_.next_arg = state->next_arg;
        state_.args_remaining = state->args_remaining;
        state_.string = _string_create ( state_.format_length + 1 );
        string = __string_format ( &state_ );
        _string_replace ( string , "\\{" , "{" ); // TODO: Okay, this is just plain lazy.
        _string_replace ( string , "\\}" , "}" ); // TODO: ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        _string_format_append ( &state->string
                              , string
                              , string_length ( string )
                              , format_specifier
                              );
        string_destroy ( string );

        // Update state.
        state->next_arg = state_.next_arg;
        state->args_remaining = state_.args_remaining;

        return;
    }

    // Retrieve the value of the next argument in the variadic argument list.
    const arg_t arg = *( state->next_arg );

    // CASE: Collection format modifier present?
    //       YES  =>  Parse and print according to collection type.
    //       NO   =>  Parse and print according to format specifier type.
    if ( format_specifier->collection.tag != STRING_FORMAT_COLLECTION_NONE )
    {
        // Parse and print the argument according to the container format
        // modifier.
        switch ( format_specifier->collection.tag )
        {
            case STRING_FORMAT_COLLECTION_STRING:
            case STRING_FORMAT_COLLECTION_RESIZABLE_STRING:
            {
                _string_format_parse_argument_string ( state
                                                     , format_specifier
                                                     , 0
                                                     );
                _string_format_consume_next_argument ( state );
            }
            break;

            case STRING_FORMAT_COLLECTION_ARRAY:
            {
                _string_format_parse_argument_array ( state
                                                    , format_specifier
                                                    , 0
                                                    );
                _string_format_consume_next_argument ( state );
                _string_format_consume_next_argument ( state );
                _string_format_consume_next_argument ( state );
            }
            break;

            case STRING_FORMAT_COLLECTION_RESIZABLE_ARRAY:
            {
                _string_format_parse_argument_array ( state
                                                    , format_specifier
                                                    , 0
                                                    );
                _string_format_consume_next_argument ( state );
            }
            break;

            default:
            {
                // Should not happen, but something was **supposed** to have
                // been processed here, so skip it.
                _string_format_consume_next_argument ( state );
            }
            break;
        }
    }
    else
    {
        // Parse and print the argument according to the format specifier.
        switch ( format_specifier->tag )
        {
            case STRING_FORMAT_SPECIFIER_RAW:                            _string_format_parse_argument_raw ( state , format_specifier , ( u64 ) arg )                             ;break;
            case STRING_FORMAT_SPECIFIER_CHARACTER:                      _string_format_parse_argument_character ( state , format_specifier , ( char ) arg )                      ;break;
            case STRING_FORMAT_SPECIFIER_INTEGER:                        _string_format_parse_argument_integer ( state , format_specifier , ( i64 ) arg )                         ;break;
            case STRING_FORMAT_SPECIFIER_FLOATING_POINT:                 _string_format_parse_argument_floating_point ( state , format_specifier , ( f64* ) arg )                 ;break;
            case STRING_FORMAT_SPECIFIER_FLOATING_POINT_SHOW_FRACTIONAL: _string_format_parse_argument_floating_point_show_fractional ( state , format_specifier , ( f64* ) arg ) ;break;
            case STRING_FORMAT_SPECIFIER_FLOATING_POINT_ABBREVIATED:     _string_format_parse_argument_floating_point_abbreviated ( state , format_specifier , ( f64* ) arg )     ;break;
            case STRING_FORMAT_SPECIFIER_FLOATING_POINT_FRACTIONAL_ONLY: _string_format_parse_argument_floating_point_fractional_only ( state , format_specifier , ( f64* ) arg ) ;break;
            case STRING_FORMAT_SPECIFIER_ADDRESS:                        _string_format_parse_argument_address ( state , format_specifier , ( void* ) arg )                       ;break;
            case STRING_FORMAT_SPECIFIER_FILE_INFO:                      _string_format_parse_argument_file_info ( state , format_specifier , ( file_t* ) arg )                   ;break;
            default:                                                                                                                                                               break;
        }

        // Consume the argument that was just printed.
        _string_format_consume_next_argument ( state );
    }
}

u64
_string_format_parse_argument_raw
(   state_t*                            state
,   const string_format_specifier_t*    format_specifier
,   const u64                           arg
)
{
    char string[ STRING_INTEGER_MAX_LENGTH + 1 ];
    const u8 radix = format_specifier->radix.radix;
    const u64 string_length = string_u64 ( arg , radix , string );
    return _string_format_append ( &state->string
                                 , string
                                 , string_length
                                 , format_specifier
                                 );
}

u64
_string_format_parse_argument_integer
(   state_t*                            state
,   const string_format_specifier_t*    format_specifier
,   const i64                           arg 
)
{
    char string[ STRING_INTEGER_MAX_LENGTH + 1 ];
    const u8 radix = format_specifier->radix.radix;
    const bool hide_sign = format_specifier->sign.tag == STRING_FORMAT_SIGN_HIDE
                        && radix == 10
                        && arg < 0
                        ;
    const bool show_sign = format_specifier->sign.tag == STRING_FORMAT_SIGN_SHOW
                        && radix == 10
                        && arg >= 0
                        ;
    u64 string_length;
    if ( hide_sign )
    {
        string_length = string_u64 ( -arg , radix , string );
    }
    else if ( show_sign )
    {
        string[ 0 ] = '+';
        string_length = string_u64 ( arg , radix , string + 1 ) + 1;
    }
    else
    {
        string_length = string_i64 ( arg , radix , string );
    }
    return _string_format_append ( &state->string
                                 , string
                                 , string_length
                                 , format_specifier
                                 );
}

u64
_string_format_parse_argument_floating_point
(   state_t*                            state
,   const string_format_specifier_t*    format_specifier
,   const f64*                          arg
)
{
    // Floating point values are passed by address (see common/args.h), so the
    // null case must be handled explicitly.
    if ( !arg )
    {
        return _string_format_append ( &state->string
                                     , ""
                                     , 0
                                     , format_specifier
                                     );
    }

    char string[ STRING_FLOAT_MAX_LENGTH + 1 ];
    f64 value = *arg;
    i64 integer = value;
    f64 fractional = value - integer;
    const bool hide_sign = format_specifier->sign.tag == STRING_FORMAT_SIGN_HIDE
                        && value < 0
                        ;
    const bool show_sign = format_specifier->sign.tag == STRING_FORMAT_SIGN_SHOW
                        && value >= 0
                        ;
    u64 string_length;

    if ( hide_sign )
    {
        string_length = string_f64 ( -value
                                   , format_specifier->fix_precision.precision
                                   , false
                                   , string
                                   );
    }
    else if ( show_sign )
    {
        string[ 0 ] = '+';
        string_length = string_f64 ( value
                                   , format_specifier->fix_precision.precision
                                   , false
                                   , string + 1
                                   ) + 1;
    }
    else
    {
        string_length = string_f64 ( value
                                   , format_specifier->fix_precision.precision
                                   , false
                                   , string
                                   );
    }

    if ( !fractional )
    {
        u64 index;
        if ( string_contains ( string , string_length
                             , string_char ( '.' ) , 1
                             , false
                             , &index
                             ))
        {
            string_length = index;
            string[ string_length ] = 0; // Append terminator.
        }
    }

    return _string_format_append ( &state->string
                                 , string
                                 , string_length
                                 , format_specifier
                                 );
}

u64
_string_format_parse_argument_floating_point_show_fractional
(   state_t*                            state
,   const string_format_specifier_t*    format_specifier
,   const f64*                          arg
)
{
    // Floating point values are passed by address (see common/args.h), so the
    // null case must be handled explicitly.
    if ( !arg )
    {
        return _string_format_append ( &state->string
                                     , ""
                                     , 0
                                     , format_specifier
                                     );
    }

    char string[ STRING_FLOAT_MAX_LENGTH + 1 ];
    f64 value = *arg;
    const bool hide_sign = format_specifier->sign.tag == STRING_FORMAT_SIGN_HIDE
                        && value < 0
                        ;
    const bool show_sign = format_specifier->sign.tag == STRING_FORMAT_SIGN_SHOW
                        && value >= 0
                        ;
    u64 string_length;

    if ( hide_sign )
    {
        string_length = string_f64 ( -value
                                   , format_specifier->fix_precision.precision
                                   , false
                                   , string
                                   );
    }
    else if ( show_sign )
    {
        string[ 0 ] = '+';
        string_length = string_f64 ( value
                                   , format_specifier->fix_precision.precision
                                   , false
                                   , string + 1
                                   ) + 1;
    }
    else
    {
        string_length = string_f64 ( value
                                   , format_specifier->fix_precision.precision
                                   , false
                                   , string
                                   );
    }

    return _string_format_append ( &state->string
                                 , string
                                 , string_length
                                 , format_specifier
                                 );
}

u64
_string_format_parse_argument_floating_point_abbreviated
(   state_t*                            state
,   const string_format_specifier_t*    format_specifier
,   const f64*                          arg
)
{
    // Floating point values are passed by address (see common/args.h), so the
    // null case must be handled explicitly.
    if ( !arg )
    {
        return _string_format_append ( &state->string
                                     , ""
                                     , 0
                                     , format_specifier
                                     );
    }

    char string[ STRING_FLOAT_MAX_LENGTH + 1 ];
    f64 value = *arg;
    const bool hide_sign = format_specifier->sign.tag == STRING_FORMAT_SIGN_HIDE
                        && value < 0
                        ;
    const bool show_sign = format_specifier->sign.tag == STRING_FORMAT_SIGN_SHOW
                        && value >= 0
                        ;
    u64 string_length;

    if ( hide_sign )
    {
        string_length = string_f64 ( -value
                                   , format_specifier->fix_precision.precision
                                   , true
                                   , string
                                   );
    }
    else if ( show_sign )
    {
        string[ 0 ] = '+';
        string_length = string_f64 ( value
                                   , format_specifier->fix_precision.precision
                                   , true
                                   , string + 1
                                   ) + 1;
    }
    else
    {
        string_length = string_f64 ( value
                                   , format_specifier->fix_precision.precision
                                   , true
                                   , string
                                   );
    }

    return _string_format_append ( &state->string
                                 , string
                                 , string_length
                                 , format_specifier
                                 );
}

u64
_string_format_parse_argument_floating_point_fractional_only
(   state_t*                            state
,   const string_format_specifier_t*    format_specifier
,   const f64*                          arg
)
{
    // Floating point values are passed by address (see common/args.h), so the
    // null case must be handled explicitly.
    if ( !arg )
    {
        return _string_format_append ( &state->string
                                     , ""
                                     , 0
                                     , format_specifier
                                     );
    }

    char string[ STRING_FLOAT_MAX_LENGTH + 1 ];
    f64 value = *arg;
    const bool hide_sign = format_specifier->sign.tag == STRING_FORMAT_SIGN_HIDE
                        && value < 0
                        ;
    const bool show_sign = format_specifier->sign.tag == STRING_FORMAT_SIGN_SHOW
                        && value >= 0
                        ;
    u64 string_length;

    if ( hide_sign )
    {
        string_length = string_f64 ( -value
                                   , format_specifier->fix_precision.precision
                                   , false
                                   , string
                                   );
    }
    else if ( show_sign )
    {
        string[ 0 ] = '+';
        string_length = string_f64 ( value
                                   , format_specifier->fix_precision.precision
                                   , false
                                   , string + 1
                                   ) + 1;
    }
    else
    {
        string_length = string_f64 ( value
                                   , format_specifier->fix_precision.precision
                                   , false
                                   , string
                                   );
    }

    u64 index;
    if ( string_contains ( string , string_length
                         , string_char ( '.' ) , 1
                         , false
                         , &index
                         ))
    {
        index += 1;
        string_length -= index;
        memory_move ( string
                    , string + index
                    , string_length
                    );
        string[ string_length ] = 0;
    }
    
    return _string_format_append ( &state->string
                                 , string
                                 , string_length
                                 , format_specifier
                                 );
}

u64
_string_format_parse_argument_address
(   state_t*                            state
,   const string_format_specifier_t*    format_specifier
,   const void*                         arg
)
{
    const char* prefix = "0x";
    char string[ STRING_INTEGER_MAX_LENGTH + 1 + 2 ];
    const u8 radix = 16;//                     ^^^ prefix string length
    memory_copy ( string , prefix , _string_length ( prefix ) );
    const u64 string_length = string_u64 ( ( u64 ) arg
                                         , radix
                                         , string + _string_length ( prefix )
                                         )
                            + _string_length ( prefix )
                            ;
    return _string_format_append ( &state->string
                                 , string
                                 , string_length
                                 , format_specifier
                                 );
}

u64
_string_format_parse_argument_character
(   state_t*                            state
,   const string_format_specifier_t*    format_specifier
,   const char                          arg
)
{
    if ( !whitespace ( arg ) && ( arg < 32 || arg > 126 ) )
    {
        return _string_format_append ( &state->string
                                     , ""
                                     , 0
                                     , format_specifier
                                     );
    }
    return _string_format_append ( &state->string
                                 , &arg
                                 , 1
                                 , format_specifier
                                 );
}

u64
_string_format_parse_argument_file_info
(   state_t*                            state
,   const string_format_specifier_t*    format_specifier
,   file_t*                             arg
)
{
    // Null argument or file handle? Y/N
    if ( !arg || !arg->handle )
    {
        return _string_format_append ( &state->string
                                     , ""
                                     , 0
                                     , format_specifier
                                     );
    }

    const u64 old_length = string_length ( state->string );

    // CASE: Invalid file? Y/N
    if ( !arg->valid )
    {
        const char* src = "\n\t{"
                          "\n\t   File info:  Invalid file!"
                          "\n\t   Mode:       -"
                          "\n\t   Size:       -"
                          "\n\t   Position:   -"
                          "\n\t}\n"
                          ;
        const u64 src_length = _string_length ( src );
        _string_format_append ( &state->string
                              , src
                              , src_length
                              , format_specifier
                              );
        return string_length ( state->string ) - old_length;
    }

    // Stringify file path.
    const char* path = file_path ( arg );

    // Stringify file mode.
    const FILE_MODE mode_ = file_mode ( arg );
    const char* mode;
    if ( ( mode_ & FILE_MODE_READ ) && ( mode_ & FILE_MODE_WRITE ) )
    {
        mode = "read, write";
    }
    else if ( ( mode_ & FILE_MODE_READ ) && !( mode_ & FILE_MODE_WRITE ) )
    {
        mode = "read-only";
    }
    else if ( !( mode_ & FILE_MODE_READ ) && ( mode_ & FILE_MODE_WRITE ) )
    {
        mode = "write-only";
    }
    else
    {
        mode = "Unknown";
    }

    // Stringify file bytesize.
    const u64 size = file_size ( arg );
    f64 size_amount;
    const char* size_unit = string_bytesize ( size , &size_amount );

    // Stringify file position.
    const u64 position = file_position_get ( arg );
    const f64 position_ratio = size ? 100.00 * ( ( ( f64 ) position ) / ( ( f64 ) size ) )
                                    :  0.0
                                    ;

    // Print the stringified file fields to a new string with pretty formatting,
    // append that string to the string being constructed, then delete it.
    string_t* formatted = string_format ( "\n\t{"
                                          "\n\t   File info:  %s"
                                          "\n\t   Mode:       %s"
                                          "\n\t   Size:       %.2f %s"
                                          "\n\t   Position:   %u (%.2f%%)"
                                          "\n\t}\n"
                                        , path
                                        , mode
                                        , &size_amount , size_unit
                                        , position , &position_ratio
                                        );
    _string_format_append ( &state->string
                          , formatted
                          , string_length ( formatted ) 
                          , format_specifier
                          );
    string_destroy ( formatted );

    return string_length ( state->string ) - old_length;
}

u64
_string_format_parse_argument_string
(   state_t*                            state
,   const string_format_specifier_t*    format_specifier
,   const char*                         arg
)
{
    u64 string_from;
    u64 string_to;
    if (   format_specifier->collection.sliced
        && (   format_specifier->collection.tag == STRING_FORMAT_COLLECTION_STRING
            || format_specifier->collection.tag == STRING_FORMAT_COLLECTION_RESIZABLE_STRING
           ))
    {
        string_from = format_specifier->collection.slice.from;
        string_to = format_specifier->collection.slice.to;
    }
    else
    {
        string_from = 0;
        string_to = format_specifier->collection.string.length;
    }
    return _string_format_append ( &state->string
                                 , format_specifier->collection.string.string + string_from
                                 , string_to - string_from
                                 , format_specifier
                                 );
}

u64
_string_format_parse_argument_array
(   state_t*                            state
,   const string_format_specifier_t*    format_specifier
,   const void*                         arg
)
{
    // Null argument? Y/N
    if ( !format_specifier->collection.array.array )
    {
        return _string_format_append ( &state->string
                                     , ""
                                     , 0
                                     , format_specifier
                                     );
    }

    const u64 old_length = string_length ( state->string );

    // Print array start token.
    _string_append ( state->string , "{ " );

    // Set start and end index (accounts for slice modifier).
    const u64 array_from = ( format_specifier->collection.sliced )
                         ? format_specifier->collection.slice.from
                         : 0
                         ;
    const u64 array_to = ( format_specifier->collection.sliced )
                       ? format_specifier->collection.slice.to
                       : format_specifier->collection.array.length
                       ;

    // Traverse the array.
    for ( u64 i = array_from; i < array_to; ++i )
    {
        // Retrieve the array element address.
        const void* element = ( const void* )( ( ( u64 ) format_specifier->collection.array.array )
                                             + i * format_specifier->collection.array.stride
                                             );
        
        // Attempt to parse and print the value at the address according to the
        // format specifier and array stride.
        switch ( format_specifier->tag )
        {
            case STRING_FORMAT_SPECIFIER_RAW:
            {
                u64 value;
                switch ( format_specifier->collection.array.stride )
                {
                    case sizeof ( u8 ):  value = *( ( u8* ) element )  ;break;
                    case sizeof ( u16 ): value = *( ( u16* ) element ) ;break;
                    case sizeof ( u32 ): value = *( ( u32* ) element ) ;break;
                    case sizeof ( u64 ): value = *( ( u64* ) element ) ;break;
                    default:             value = 0                     ;break;
                }
                _string_format_parse_argument_raw ( state , format_specifier , value );
            }
            break;

            case STRING_FORMAT_SPECIFIER_CHARACTER:
            {
                const char value = *( ( char* ) element );
                _string_append ( state->string , "`" );
                _string_format_parse_argument_character ( state , format_specifier , value );
                _string_append ( state->string , "`" );
            }
            break;

            case STRING_FORMAT_SPECIFIER_INTEGER:
            {
                i64 value;
                switch ( format_specifier->collection.array.stride )
                {
                    case sizeof ( i8 ):  value = *( ( i8* ) element )  ;break;
                    case sizeof ( i16 ): value = *( ( i16* ) element ) ;break;
                    case sizeof ( i32 ): value = *( ( i32* ) element ) ;break;
                    case sizeof ( i64 ): value = *( ( i64* ) element ) ;break;
                    default:             value = 0                     ;break;
                }
                _string_format_parse_argument_integer ( state , format_specifier , value );
            }
            break;

            case STRING_FORMAT_SPECIFIER_FLOATING_POINT:
            {
                f64 value;
                switch ( format_specifier->collection.array.stride )
                {
                    case sizeof ( f32 ): value = *( ( f32* ) element ) ;break;
                    case sizeof ( f64 ): value = *( ( f64* ) element ) ;break;
                    default:             value = 0                     ;break;
                }
                _string_format_parse_argument_floating_point ( state , format_specifier , &value );
            }
            break;

            case STRING_FORMAT_SPECIFIER_FLOATING_POINT_SHOW_FRACTIONAL:
            {
                f64 value;
                switch ( format_specifier->collection.array.stride )
                {
                    case sizeof ( f32 ): value = *( ( f32* ) element ) ;break;
                    case sizeof ( f64 ): value = *( ( f64* ) element ) ;break;
                    default:             value = 0                     ;break;
                }
                _string_format_parse_argument_floating_point_show_fractional ( state , format_specifier , &value );
            }
            break;

            case STRING_FORMAT_SPECIFIER_FLOATING_POINT_ABBREVIATED:
            {
                f64 value;
                switch ( format_specifier->collection.array.stride )
                {
                    case sizeof ( f32 ): value = *( ( f32* ) element ) ;break;
                    case sizeof ( f64 ): value = *( ( f64* ) element ) ;break;
                    default:             value = 0                     ;break;
                }
                _string_format_parse_argument_floating_point_abbreviated ( state , format_specifier , &value );
            }
            break;

            case STRING_FORMAT_SPECIFIER_FLOATING_POINT_FRACTIONAL_ONLY:
            {
                f64 value;
                switch ( format_specifier->collection.array.stride )
                {
                    case sizeof ( f32 ): value = *( ( f32* ) element ) ;break;
                    case sizeof ( f64 ): value = *( ( f64* ) element ) ;break;
                    default:             value = 0                     ;break;
                }
                _string_format_parse_argument_floating_point_fractional_only ( state , format_specifier , &value );
            }
            break;

            case STRING_FORMAT_SPECIFIER_ADDRESS:
            {
                void* value;
                switch ( format_specifier->collection.array.stride )
                {
                    case sizeof ( void* ): value = *( ( void** ) element );break;
                    default:               value = 0                      ;break;
                }
                _string_format_parse_argument_address ( state , format_specifier , value );
            }
            break;

            case STRING_FORMAT_SPECIFIER_STRING:
            {
                _string_append ( state->string , "`" );
                _string_format_parse_argument_string ( state , format_specifier , 0 );
                _string_append ( state->string , "`" );
            }
            break;

            case STRING_FORMAT_SPECIFIER_RESIZABLE_STRING:
            {
                _string_append ( state->string , "`" );
                _string_format_parse_argument_string ( state , format_specifier , 0 );
                _string_append ( state->string , "`" );
            }
            break;

            case STRING_FORMAT_SPECIFIER_FILE_INFO:
            {
                file_t* value;
                switch ( format_specifier->collection.array.stride )
                {
                    case sizeof ( file_t* ): value = *( ( file_t** ) element );break;
                    default:                 value = 0                        ;break;
                }
                _string_format_parse_argument_file_info ( state , format_specifier , value );
            }
            break;
            
            default:
            {}
            break;
        }

        // Print array element separator token.
        if ( i < array_to - 1 )
        {
            _string_append ( state->string , ", " );
        }
    }

    // Print array terminator token.
    _string_append ( state->string , " }" );

    return string_length ( state->string ) - old_length;
}

u64
_string_format_append
(   string_t**                          string
,   const char*                         src
,   const u64                           src_length
,   const string_format_specifier_t*    format_specifier
)
{
    // CASE: Explicit padding not required.
    if ( format_specifier->padding.tag == STRING_FORMAT_PADDING_NONE )
    {
        // Append the entire source string.
        string_append ( *string , src , src_length );
        return src_length;
    }

    // CASE: Content length exceeds specified pad width.
    if ( format_specifier->padding.width <= src_length )
    {
        // CASE: Fix-width printing requested.
        if ( format_specifier->padding.fixed )
        {
            // Truncate the source string.
            string_append ( *string
                          , src
                          , format_specifier->padding.width
                          );
            return format_specifier->padding.width;
        }

        // CASE: Min-width printing requested.
        else
        {
            // Append the entire source string.
            string_append ( *string , src , src_length );
            return src_length;
        }
    }

    // Compute the amount to pad.
    const u64 pad_width = format_specifier->padding.width - src_length;

    // CASE: Left padding required.
    if ( format_specifier->padding.tag == STRING_FORMAT_PADDING_LEFT )
    {
        u64 pad = pad_width;
        while ( pad )
        {
            u64 index = 0;

            // Handle multi-character padding strings with escaped `'`
            // characters.
            if ( !format_specifier->padding.wildcard_value )
            {
                u64 index_;
                while ( string_contains ( format_specifier->padding.value.value + index
                                        , format_specifier->padding.value.length - index
                                        , "\\'"
                                        , sizeof ( "\\'" ) - 1
                                        , false
                                        , &index_
                                        ))
                {
                    // Copy everything between the previous copy end and the
                    // multi-character escape token.
                    const u64 size = MIN ( pad , index_ );
                    string_append ( *string
                                  , format_specifier->padding.value.value + index
                                  , size
                                  );
                    
                    index += index_;
                    pad -= size;

                    // Maximum pad width reached? Y/N
                    if ( !pad )
                    {
                        break;
                    }

                    // Skip the `\` token and continue.
                    index += 1;
                }
            }

            // Copy everything between the previous copy end and the end of the
            // padding string.
            const u64 size = MIN ( pad
                                 , format_specifier->padding.value.length - index
                                 );
            string_append ( *string
                          , format_specifier->padding.value.value + index
                          , size
                          );

            pad -= size;
        }
    }

    // Append the entire source string.
    string_append ( *string , src , src_length );

    // CASE: Right padding required.
    if ( format_specifier->padding.tag == STRING_FORMAT_PADDING_RIGHT )
    {
        u64 pad = pad_width;
        while ( pad )
        {
            u64 index = 0;

            // Handle multi-character padding strings with escaped `'`
            // characters.
            if ( !format_specifier->padding.wildcard_value )
            {
                u64 index_;
                while ( string_contains ( format_specifier->padding.value.value + index
                                        , format_specifier->padding.value.length - index
                                        , "\\'"
                                        , sizeof ( "\\'" ) - 1
                                        , false
                                        , &index_
                                        ))
                {
                    // Copy everything between the previous copy end and the
                    // multi-character escape token.
                    const u64 size = MIN ( pad , index_ );
                    string_append ( *string
                                  , format_specifier->padding.value.value + index
                                  , size
                                  );
                    
                    index += index_;
                    pad -= size;

                    // Maximum pad width reached? Y/N
                    if ( !pad )
                    {
                        break;
                    }

                    // Skip the `\` token and continue.
                    index += 1;
                }
            }

            // Copy everything between the previous copy end and the end of the
            // padding string.
            const u64 size = MIN ( pad
                                 , format_specifier->padding.value.length - index
                                 );
            string_append ( *string
                          , format_specifier->padding.value.value + index
                          , size
                          );

            pad -= size;
        }
    }
    return format_specifier->padding.width;
}