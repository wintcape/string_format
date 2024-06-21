/**
 * @file container/array.h
 * @brief Provides an interface for a resizable array data structure.
 */
#ifndef ARRAY_H
#define ARRAY_H

#include "core/array.h"

/** @brief Type declaration for a resizable array. */
typedef void array_t;

/** @brief Type and instance definitions for array fields. */
typedef enum
{
    ARRAY_FIELD_CAPACITY
,   ARRAY_FIELD_LENGTH
,   ARRAY_FIELD_STRIDE

,   ARRAY_FIELD_COUNT
}
ARRAY_FIELD;

/** @brief Array default capacity. */
#define ARRAY_DEFAULT_CAPACITY 10

/** @brief Array growth factor. */
#define ARRAY_GROWTH_FACTOR 2

/**
 * @brief Allocates memory for a resizable array.
 * 
 * Use array_create to explicitly specify an initial capacity, or
 * array_create_new to use a default.
 * 
 * Uses dynamic memory allocation. Call array_destroy to free.
 * 
 * @param initial_capacity The initial capacity. Must be non-zero.
 * @param stride The fixed element size in bytes. Must be non-zero.
 * @return An empty resizable array.
 */
array_t*
_array_create
(   ARRAY_FIELD initial_capacity
,   ARRAY_FIELD stride
);

/** @param type C data type of the array. */
#define array_create(type,initial_capacity) \
    _array_create ( (initial_capacity) , sizeof ( type ) )

/** @param type C data type of the array. */
#define array_create_new(type) \
    _array_create ( ARRAY_DEFAULT_CAPACITY , sizeof ( type ) )

/**
 * @brief Creates a resizable array by copying an existing fixed-length array.
 * O(n).
 *
 * Uses dynamic memory allocation. Call array_destroy to free.
 * 
 * @param array The fixed-length array to copy. Must be non-zero.
 * @param length The number of elements contained by array.
 * @param stride The fixed element size in bytes.
 * @return A resizable copy of array.
*/
array_t*
_array_create_from
(   const void* array
,   ARRAY_FIELD length
,   ARRAY_FIELD stride
);

/** @param type C data type of the array. */
#define array_create_from(type,array,length) \
    _array_create_from ( (array) , (length) , sizeof ( type ) )

/**
 * @brief Frees the memory used by a resizable array.
 * 
 * @param array The resizable array to free.
 */
void
_array_destroy
(   array_t* array
);

#define array_destroy(array) \
    _array_destroy ( array )

/**
 * @brief Obtains the value of a resizable array field. O(1).
 * 
 * @param array The resizable array to query. Must be non-zero.
 * @param field The field to read.
 * @return The value of the resizable array field.
 */
u64
_array_field_get
(   const array_t*  array
,   ARRAY_FIELD     field
);

/** @brief Query array field: capacity. */
#define array_capacity(array) \
    _array_field_get ( (array) , ARRAY_FIELD_CAPACITY )

/** @brief Query array field: length. */
#define array_length(array) \
    _array_field_get ( (array) , ARRAY_FIELD_LENGTH )

/** @brief Query array field: stride. */
#define array_stride(array) \
    _array_field_get ( (array) , ARRAY_FIELD_STRIDE )

/**
 * @brief Sets the value of a resizable array field. O(1).
 * 
 * @param array The resizable array to mutate. Must be non-zero.
 * @param field The field to set.
 * @param value The value to set.
 */
void
_array_field_set
(   array_t*    array
,   ARRAY_FIELD field
,   u64         value
);

/**
 * @brief Computes the current size in bytes of a resizable array data
 * structure. O(1).
 * 
 * @param array The resizable array to query. Must be non-zero.
 * @return The size in bytes of the resizable array data structure.
 */
u64
_array_size
(   const array_t* array
);

#define array_size(array) \
    _array_size ( array )

/**
 * @brief Resizes an existing resizable array. O(n).
 * 
 * @param array The resizable array to resize. Must be non-zero.
 * @param minimum_capacity The minimum number of elements the new array is
 * required to hold. If the value of this parameter is less than the current
 * array length, the array will be automatically shrunk and data loss will
 * occur.
 * @return The array after resizing (possibly with new address).
 */
array_t*
_array_resize
(   array_t*    array
,   ARRAY_FIELD minimum_capacity
);

/** @brief Alias for calling array_resize with ARRAY_GROWTH_FACTOR. */
#define array_resize(array,minimum_capacity) \
    _array_resize ( (array) , ARRAY_GROWTH_FACTOR * minimum_capacity )

/**
 * @brief Appends an element to a resizable array. Amortized O(1).
 * 
 * Use array_push to append a literal value to the array; use _array_push to
 * pass the address of a value to append.
 * 
 * @param array The resizable array to append to. Must be non-zero.
 * @param src The handle to the value to append. Must be non-zero.
 * @return The array (possibly with new address).
 */
array_t*
_array_push
(   array_t*    array
,   const void* src
);

#define array_push(array,value)                  \
    ({                                           \
        __typeof__ ( (value) ) tmp = (value);    \
       (array) = _array_push ( (array) , &tmp ); \
    })

/**
 * @brief Inserts an element into a resizable array at a specified index. O(n).
 * 
 * Use array_insert to insert a literal value into the array; use _array_insert
 * to pass a handle to the value to insert.
 * 
 * @param array The resizable array to append to. Must be non-zero.
 * @param index The index to insert at.
 * @param src The handle to the value to insert. Must be non-zero.
 * @return The array (possibly with new address).
 */
array_t*
_array_insert
(   array_t*    array
,   u64         index
,   const void* src
);

#define array_insert(array,index,value)                      \
    ({                                                       \
        __typeof__ ( (value) ) tmp = (value);                \
       (array) = _array_insert ( (array) , (index) , &tmp ); \
    })

/**
 * @brief Removes an element from a resizable array at a specified index. O(n).
 * 
 * @param array The resizable array to mutate. Must be non-zero.
 * @param index The index of the element to remove.
 * @param dst Output buffer to store the element that was removed. Pass 0 to
 * retrieve nothing.
 * @return The array.
 */
array_t*
_array_remove
(   array_t*    array
,   u64         index
,   void*       dst
);

#define array_remove(array,index,dst) \
    _array_remove ( (array) , (index) , (dst) )


/**
 * @brief Removes the tail of an array. O(1).
 * 
 * @param array The resizable array to remove from. Must be non-zero.
 * @param dst Output buffer to store the element that was removed. Pass 0 to
 * retrieve nothing.
 * @return true on success; false if array empty.
 */
bool
_array_pop
(   array_t*    array
,   void*       dst
);

#define array_pop(array,dst) \
    _array_pop ( (array) , (dst) )


/**
 * @brief Copies a resizable array. O(n).
 * 
 * Uses dynamic memory allocation. Call array_destroy to free.
 * 
 * @param array The resizable array to copy.
 * @return A copy of the array.
 */
array_t*
__array_copy
(   const array_t* array
);

// Extra underscore avoids a name conflict with array_copy from core/array.h.
#define _array_copy(array) \
    __array_copy ( array )

/**
 * @brief Alias for calling array_reverse on a resizable array.
 * (see core/array.h)
 */
#define _array_reverse(array,swap)         \
    array_reverse ( (array)                \
                  , array_length ( array ) \
                  , array_stride ( array ) \
                  , (swap)                 \
                  )

/**
 * @brief Alias for calling array_shuffle on a resizable array.
 * (see core/array.h)
 */
#define _array_shuffle(array,swap)         \
    array_shuffle ( (array)                \
                  , array_length ( array ) \
                  , array_stride ( array ) \
                  , (swap)                 \
                  )

/**
 * @brief Alias for calling array_sort on a resizable array.
 * (see core/array.h)
 */
#define _array_sort(array,comparator)   \
    array_sort ( (array)                \
               , array_length ( array ) \
               , array_stride ( array ) \
               , (comparator)           \
               )

#endif  // ARRAY_H