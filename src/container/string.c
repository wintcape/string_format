/**
 * @file container/string.c
 * @brief Implementation of the container/string header.
 * (see container/string.h for additional details)
 */
#include "container/string.h"

#include "core/logger.h"
#include "math/math.h"
#include "platform/memory.h"

string_t*
__string_create
(   ARRAY_FIELD initial_capacity
)
{
    if ( !initial_capacity )
    {
        LOGERROR ( "__string_create: Value of initial_capacity argument must be non-zero." );
        return 0;
    }
    
    char* string = array_create ( char , initial_capacity );
    _array_field_set ( string , ARRAY_FIELD_LENGTH , 1 );
    return string;//                                 ^ terminator
}

string_t*
__string_copy
(   const char* src
,   const u64   src_length
)
{//                               v terminator
    const u64 size = src_length + 1;
    char* string = array_create ( char , size );
    _array_field_set ( string , ARRAY_FIELD_LENGTH , size );
    memory_copy ( string , src , src_length );
    return string;
}

void
string_destroy
(   string_t* string
)
{
    if ( !string )
    {
        return;
    }

    array_destroy ( string );
}

u64
string_length
(   const string_t* string
)
{
    return array_length ( string ) - 1;
}

string_t*
__string_append
(   string_t*   string
,   const char* src
,   const u64   src_length
)
{
    const u64 old_size = array_length ( string );
    const u64 new_size = old_size + src_length;
    const u64 new_length = string_length ( string ) + src_length;
    const u64 stride = array_stride ( string );

    if ( new_size >= array_capacity ( string ) )
    {
        string = array_resize ( string , new_size );
    }

    const u64 dst = ( ( u64 ) string );
    memory_copy ( ( void* )( dst + string_length ( string ) * stride )
                , src
                , src_length * stride
                );
    memory_clear ( ( void* )( dst + new_length * stride ) , stride );
    _array_field_set ( string , ARRAY_FIELD_LENGTH , new_size );

    return string;
}

string_t*
__string_prepend
(   string_t*   string
,   const char* src
,   const u64   src_length
)
{
    return __string_insert ( string , 0 , src , src_length );
}

string_t*
__string_insert
(   string_t*   string
,   u64         index
,   const char* src
,   const u64   src_length
)
{
    const u64 old_length = string_length ( string );
    const u64 old_size = array_length ( string );
    const u64 new_size = old_size + src_length;
    const u64 stride = array_stride ( string );
    
    if ( index > old_length )
    {
        LOGERROR ( "__string_insert called with out of bounds index: %i (index) > %i (string length)."
                 , index , string_length ( string )
                 );
        return string;
    }

    if ( new_size >= array_capacity ( string ) )
    {
        string = array_resize ( string , new_size );
    }

    const u64 dst = ( ( u64 ) string );
    memory_move ( ( void* )( dst + ( index + src_length ) * stride )
                , ( void* )( dst + index * stride )
                , ( old_length - index ) * stride
                );
    memory_copy ( ( void* )( dst + index * stride )
                , src
                , src_length * stride
                );
    memory_clear ( ( void* )( dst + ( old_length + src_length ) * stride )
                 , stride
                 ); // Append terminator.
    _array_field_set ( string , ARRAY_FIELD_LENGTH , new_size );

    return string;
}

string_t*
__string_remove
(   string_t*   string
,   u64         index
,   u64         count
)
{
    const u64 old_length = string_length ( string );
    const u64 old_size = array_length ( string );
    const u64 new_size = old_size - count;
    const u64 stride = array_stride ( string );

    if ( index + count > old_length )
    {
        LOGERROR ( "__string_remove called with illegal index or count: (index %i + count %i) %i > %i (string length)."
                 , index , count , index + count , old_length
                 );
        return string;
    }

    const u64 src = ( ( u64 ) string );
    memory_move ( ( void* )( src + index * stride )
                , ( void* )( src + ( index + count ) * stride )
                , ( old_length - index - count ) * stride
                );
    memory_clear ( string + ( new_size - 1 ) * stride , stride ); // Append terminator.
    _array_field_set ( string , ARRAY_FIELD_LENGTH , new_size );

    return string;
}

string_t*
__string_clear
(   string_t* string
)
{
    string[ 0 ] = 0; // Append terminator.
    _array_field_set ( string , ARRAY_FIELD_LENGTH , 1 );
    return string;
}

string_t*
__string_trim
(   string_t* string
)
{
    const u64 length = string_length ( string );
    u64 i;
    
    // Compute index of first non-whitespace character.
    for ( i = 0; i < length && whitespace ( string[ i ] ); ++i );
    char* const from = string + i;

    // Whitespace-only case.
    if ( i == length )
    {
        return string_clear ( string );
    }

    // Compute index of final non-whitespace character.
    for ( i = length; i && whitespace ( string[ i - 1 ] ); --i );
    char* const to = string + i;

    // Copy memory range in-place.
    const u64 size = MAX ( 0 , to - from );
    memory_move ( string , from , size );
    string[ size ] = 0; // Append terminator.

    _array_field_set ( string , ARRAY_FIELD_LENGTH , size + 1 );
    
    return string;
}

string_t*
__string_replace
(   string_t*   string
,   const char* remove
,   const u64   remove_length
,   const char* replace
,   const u64   replace_length
)
{
    // CASE: Substring to remove and replacement substring are equivalent.
    if ( string_equal ( remove , remove_length , replace , replace_length ) )
    {
        return string;
    }

    // CASE: Substring to remove is empty.
    if ( !remove_length )
    {
        // CASE: Replacement substring is also empty.
        if ( !replace_length )
        {
            return string;
        }
        
        // Duplicate the replacement substring once for each character in the
        // original string.
        const u64 count = string_length ( string );
        string_clear ( string );
        for ( u64 i = 0; i < count; ++i )
        {
            string_append ( string , replace , replace_length );
        }
    }

    // CASE: Substring to remove and replacement substring are the same length.
    else if ( remove_length == replace_length )
    {
        u64 index = 0;
        u64 index_;
        while ( string_contains ( string + index
                                , string_length ( string ) - index
                                , remove
                                , remove_length
                                , false
                                , &index_
                                ))
        {
            index += index_;
            memory_copy ( string + index , replace , replace_length );
            index += replace_length;
        }
    }

    // CASE: Substring to remove and replacement substring differ in length.
    else
    {
        u64 index = 0;
        u64 index_;
        while ( string_contains ( string + index
                                , string_length ( string ) - index
                                , remove
                                , remove_length
                                , false
                                , &index_
                                ))
        {
            index += index_;
            string_remove ( string , index , remove_length );
            string_insert ( string , index , replace , replace_length );
            index += replace_length;
        }
    }

    return string;
}

string_t*
__string_strip_ansi
(   string_t* string
)
{
    u64 i = 0;
    for (;;)
    {
        const u64 length = string_length ( string );
        if ( !length || i >=  length - 1 )
        {
            break;
        }

        if ( string[ i ] != '\033' || string[ i + 1 ]  != '[' )
        {
            i += 1;
            continue;
        }

        u64 j = i + 2;
        for (;;)
        {
            if ( j >= string_length ( string ) )
            {
                i += 1;
                break;
            }

            if ( string[ j ] == 'm' )
            {
                string_remove ( string , i , j - i + 1 );
                break;
            }

            if ( !digit ( string[ j ] ) && string[ j ] != ';' )
            {
                i += 1;
                break;
            }

            j += 1;
        }
    }

    return string;
}