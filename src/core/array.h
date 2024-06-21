/**
 * @file core/array.h
 * @brief Defines common fixed-length array operations. For arrays with
 * mutable length, see container/array.h.
 */
#ifndef ARRAY_UTIL_H
#define ARRAY_UTIL_H

#include "common.h"

/**
 * @brief Copies an array. O(n).
 * 
 * @param src The array to copy. Must be non-zero.
 * @param array_length The number of elements in the array.
 * @param array_stride The size of each array element in bytes.
 * @param dst Output buffer. Must be non-zero.
 * @return dst.
 */
void*
array_copy
(   const void* src
,   u64         array_length
,   u64         array_stride
,   void*       dst
);

/**
 * @brief Reverses an array. O(n).
 * 
 * @param array The array to reverse. Must be non-zero.
 * @param array_length The number of elements in the array.
 * @param array_stride The size of each array element in bytes.
 * @param swap Optional pre-allocated swap buffer. Pass 0 to use implicit memory
 * allocation.
 * @return The array after reversal.
 */
void*
array_reverse
(   void*       array
,   const u64   array_length
,   const u64   array_stride
,   void*       swap
);

/**
 * @brief Shuffles the elements of an array. O(n).
 * 
 * @param array The array to shuffle. Must be non-zero.
 * @param array_length The number of elements in the array.
 * @param array_stride The size of each array element in bytes.
 * @param swap Optional pre-allocated swap buffer. Pass 0 to use implicit memory
 * allocation.
 * @return The array with its elements shuffled.
 */
void*
array_shuffle
(   void*   array
,   u64     array_length
,   u64     array_stride
,   void*   swap
);

/**
 * @brief Sorts an array in-place.
 * 
 * Current implementation uses quicksort algorithm.
 * AVERAGE CASE TIME COMPLEXITY : O(n log(n))
 * WORST CASE TIME COMPLEXITY   : O(n²)
 * 
 * @param array The array to sort. Must be non-zero.
 * @param array_length The number of elements in the array.
 * @param array_stride The size of each array element in bytes.
 * @param comparator A function which compares two array elements.
 * Must be non-zero.
 * @return The array with all elements sorted according to the comparator.
 */
void*
array_sort
(   void*                   array
,   u64                     array_length
,   u64                     array_stride
,   comparator_function_t   comparator
);

#endif  // ARRAY_UTIL_H