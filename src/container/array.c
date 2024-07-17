/**
 * @file container/array.c
 * @brief Implementation of the container/array header.
 * (see container/array.h for additional details)
 */
#include "container/array.h"

#include "core/logger.h"
#include "math/math.h"
#include "platform/memory.h"

array_t*
_array_create
(   ARRAY_FIELD initial_capacity
,   ARRAY_FIELD stride
)
{
    if ( !initial_capacity || !stride )
    {
        if ( !initial_capacity )
        {
            LOGERROR ( "_array_create: Value of initial_capacity argument must be non-zero." );
        }
        if ( !stride )
        {
            LOGERROR ( "_array_create: Value of stride argument must be non-zero." );
        }
        return 0;
    }
    
    const u64 header_size = ARRAY_FIELD_COUNT * sizeof ( u64 );
    const u64 content_size = initial_capacity * stride;
    const u64 size = header_size + content_size;

    u64* array = memory_allocate ( size /* , MEMORY_TAG_ARRAY */ );
    memory_clear ( array , size );

    array[ ARRAY_FIELD_CAPACITY ] = initial_capacity;
    array[ ARRAY_FIELD_LENGTH ]   = 0;
    array[ ARRAY_FIELD_STRIDE ]   = stride;

    return array + ARRAY_FIELD_COUNT;
}

void
_array_destroy
(   array_t* array
)
{
    if ( !array )
    {
        return;
    }
    memory_free ( ( ( u64* ) array ) - ARRAY_FIELD_COUNT
                // , array_size ( array )
                // , MEMORY_TAG_ARRAY
                );
}

array_t*
_array_create_from
(   const void* array_
,   ARRAY_FIELD length
,   ARRAY_FIELD stride
)
{
    const u64 capacity = length * stride;
    void* array = _array_create ( capacity , stride );
    memory_copy ( array , array_ , capacity );
    _array_field_set ( array , ARRAY_FIELD_LENGTH , length );
    return array;
}

array_t*
__array_copy
(   const array_t* src
)
{
    const u64 length = array_length ( src );
    const u64 stride = array_stride ( src );
    void* copy = _array_create ( length , stride );
    array_copy ( src , length , stride , copy );
    _array_field_set ( copy , ARRAY_FIELD_LENGTH , length );
    return copy;
}

u64
_array_field_get
(   const array_t*  array
,   ARRAY_FIELD     field
)
{
    const u64* header = ( ( u64* ) array ) - ARRAY_FIELD_COUNT;
    return header[ field ];
}

void
_array_field_set
(   array_t*    array
,   ARRAY_FIELD field
,   u64         value
)
{
    u64* header = ( ( u64* ) array ) - ARRAY_FIELD_COUNT;
    header[ field ] = value;
}

u64
_array_size
(   const array_t* array
)
{
    u64* header = ( ( u64* ) array ) - ARRAY_FIELD_COUNT;
    const u64 header_size = ARRAY_FIELD_COUNT * sizeof ( u64 );
    const u64 content_size = header[ ARRAY_FIELD_STRIDE ]
                           * header[ ARRAY_FIELD_CAPACITY ]
                           ;
    return header_size + content_size;
}

array_t*
_array_resize
(   array_t*    old_array
,   ARRAY_FIELD minimum_capacity
)
{
    if ( minimum_capacity == array_capacity ( old_array ) )
    {
        return old_array;
    }

    const u64 length = MIN ( array_length ( old_array ) , minimum_capacity );
    const u64 stride = array_stride ( old_array );

    void* new_array = _array_create ( minimum_capacity , stride );
    memory_copy ( new_array , old_array , length * stride );
    _array_field_set ( new_array , ARRAY_FIELD_LENGTH , length );
    _array_destroy ( old_array );
    return new_array;
}

array_t*
_array_push
(   array_t*    array
,   const void* src
)
{
    const u64 length = array_length ( array );
    const u64 stride = array_stride ( array );

    if ( length >= array_capacity ( array ) )
    {
        array = array_resize ( array , length );
    }

    const u64 dst = ( ( u64 ) array );
    memory_copy ( ( void* )( dst + length * stride ) , src , stride );
    _array_field_set ( array , ARRAY_FIELD_LENGTH , length + 1 );
    return array;
}

bool
_array_pop
(   array_t*    array
,   void*       dst
)
{
    if ( !array_length ( array ) )
    {
        LOGERROR ( "_array_pop: Array is empty.\n" );
        return false;
    }

    const u64 length = array_length ( array ) - 1;
    const u64 stride = array_stride ( array );

    const u64 src = ( ( u64 ) array );
    if ( dst )
    {
        memory_copy ( dst , ( void* )( src + length * stride ) , stride );
    }
    _array_field_set ( array , ARRAY_FIELD_LENGTH , length );
    return true;
}

array_t*
_array_insert
(   array_t*    array
,   u64         index
,   const void* src
)
{
    const u64 length = array_length ( array );
    const u64 stride = array_stride ( array );
    
    if ( index > length )
    {
        LOGERROR ( "_array_insert: Called with out of bounds index: %i (index) > %i (array length)."
                   , index , length
                   );
        return array;
    }
    
    if ( length >= array_capacity ( array ) )
    {
        array = array_resize ( array , length );
    }
    
    const u64 dst = ( ( u64 ) array );
    memory_move ( ( void* )( dst + ( index + 1 ) * stride )
                , ( void* )( dst + index * stride )
                , ( length - index ) * stride
                );
    memory_copy ( ( void* )( dst + index * stride ) , src , stride );
    _array_field_set ( array , ARRAY_FIELD_LENGTH , length + 1 );
    return array;
}

array_t*
_array_remove
(   array_t*    array
,   u64         index
,   void*       dst
)
{
    if ( !array_length ( array ) )
    {
        LOGERROR ( "_array_remove: Array is empty.\n" );
        return array;
    }

    const u64 length = array_length ( array ) - 1;
    const u64 stride = array_stride ( array );
    
    if ( index > length )
    {
        LOGERROR ( "_array_remove: Called with out of bounds index: %i (index) >= %i (array length)."
                 , index , length + 1
                 );
        return array;
    }

    const u64 src = ( ( u64 ) array );
    if ( dst )
    {
        memory_copy ( dst , ( void* )( src + index * stride ) , stride );
    }
    memory_move ( ( void* )( src + index * stride )
                , ( void* )( src + ( index + 1 ) * stride )
                , ( length - index ) * stride
                );
    _array_field_set ( array , ARRAY_FIELD_LENGTH , length );
    return array;
}