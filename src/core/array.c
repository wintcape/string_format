/**
 * @file core/array.c
 * @brief Implementation of the core/array header.
 * (see core/array.h for additional details)
 */
#include "core/array.h"

#include "math/math.h"
#include "platform/platform.h"
#include "platform/memory.h"

void*
array_copy
(   const void* src
,   u64         array_length
,   u64         array_stride
,   void*       dst
)
{
    return memory_copy ( dst , src , array_length * array_stride );
}

void*
array_reverse
(   void*       array
,   const u64   array_length
,   const u64   array_stride
,   void*       swap_
)
{
    if ( !array_stride || array_length < 2 )
    {
        return array;
    }

    void* swap;
    if ( !swap_ )
    {
        swap = memory_allocate ( array_stride );
    }
    else
    {
        swap = swap_;
    }

    const u64 src = ( ( u64 ) array );
    u64 i;
    u64 j;
    for ( i = 0 , j = array_length - 1; i < j; ++i , --j )
    {
        memory_copy ( swap
                    , ( void* )( src + i * array_stride )
                    , array_stride
                    );
        memory_copy ( ( void* )( src + i * array_stride )
                    , ( void* )( src + j * array_stride )
                    , array_stride
                    );
        memory_copy ( ( void* )( src + j * array_stride )
                    , swap
                    , array_stride
                    );
    }

    if ( !swap_ )
    {
        memory_free ( swap );
    }

    return array;
}

void*
array_shuffle
(   void*   array
,   u64     array_length
,   u64     array_stride
,   void*   swap_
)
{
    if ( !array_stride || array_length < 2 )
    {
        return array;
    }

    void* swap;
    if ( !swap_ )
    {
        swap = memory_allocate ( array_stride );
    }
    else
    {
        swap = swap_;
    }

    const u64 src = ( ( u64 ) array );
    for ( u64 i = 0; i < array_length - 1; ++i )
    {
        u64 j = random2 ( 0 , i );
        memory_copy ( swap
                    , ( void* )( src + i * array_stride )
                    , array_stride
                    );
        memory_copy ( ( void* )( src + i * array_stride )
                    , ( void* )( src + j * array_stride )
                    , array_stride
                    );
        memory_copy ( ( void* )( src + j * array_stride )
                    , swap
                    , array_stride
                    );
    }

    if ( !swap_ )
    {
        memory_free ( swap );
    }

    return array;
}

void*
array_sort
(   void*                   array
,   u64                     array_length
,   u64                     array_stride
,   comparator_function_t   comparator
)
{
    platform_array_sort ( array , array_stride , array_length , comparator );
    return array;
}