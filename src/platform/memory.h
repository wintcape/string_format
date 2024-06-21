/**
 * @file platform/memory.h
 * @brief Provides an interface for memory allocation and management.
 */
#ifndef MEMORY_H
#define MEMORY_H

#include "common.h"

/**
 * @brief Allocates a block of memory.
 * 
 * @param size The number of bytes to allocate.
 * @return The allocated block.
 */
void*
memory_allocate
(   u64 size
);

/**
 * @brief Frees a block of memory.
 * 
 * @param memory The block to free. Must be non-zero.
 */
void
memory_free
(   void* memory
);

/**
 * @brief Clears a block of memory.
 * 
 * @param memory The block to clear. Must be non-zero.
 * @param size The block size in bytes.
 * @return memory.
 */
void*
memory_clear
(   void*   memory
,   u64     size
);

/**
 * @brief Sets a block of memory.
 * 
 * @param memory The block to set. Must be non-zero.
 * @param size The block size in bytes.
 * @return memory.
 */
void*
memory_set
(   void*   dst
,   i32     value
,   u64     size
);

/**
 * @brief Copies a specified number of bytes of memory from a source block to a
 * destination block.
 * 
 * The size of both blocks should be adequate for the specified size parameter.
 * The source and destination blocks may not overlap.
 * 
 * @param dst The destination block. Must be non-zero.
 * @param src The source block. Must be non-zero.
 * @param size The number of bytes to copy.
 * @return dst.
 */
void*
memory_copy
(   void*       dst
,   const void* src
,   u64         size
);

/**
 * @brief Moves a specified number of bytes of memory from a source block to a
 * destination block.
 * 
 * The size of both blocks should be adequate for the specified size parameter.
 * The source and destination blocks may overlap.
 * 
 * @param dst The destination block. Must be non-zero.
 * @param src The source block. Must be non-zero.
 * @param size The number of bytes to move.
 * @return dst.
 */
void*
memory_move
(   void*       dst
,   const void* src
,   u64         size
);

/**
 * @brief Fixed-length string equality test predicate.
 * 
 * @param s1 A string. Must be non-zero.
 * @param s2 A string. Must be non-zero.
 * @param size The number of bytes to compare.
 * @return true if strings are equal; false otherwise.
 */
bool
memory_equal
(   const void* s1
,   const void* s2
,   u64         size
);

#endif // MEMORY_H