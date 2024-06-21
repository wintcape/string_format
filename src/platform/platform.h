/**
 * @file platform/platform.h
 * @brief Provides an interface to abstract away the host platform layer.
 */
#ifndef PLATFORM_H
#define PLATFORM_H

#include "common.h"

////////////////////////////////////////////////////////////////////////////////
// Begin memory operations.

/**
 * @brief Platform-independent memory allocation function (see platform/memory.h).
 * 
 * @param size The number of bytes to allocate.
 */
void*
platform_memory_allocate
(   u64 size
);

/**
 * @brief Platform-independent memory free function (see platform/memory.h).
 * 
 * @param blk The block to free. Must be non-zero.
 */
void
platform_memory_free
(   void* blk
);

/**
 * @brief Queries the total amount of memory the host platform has made
 * available for allocation.
 * 
 * @return The total amount of memory the host platform has made available for
 * allocation (in bytes).
 */
u64
platform_total_available_memory
( void );

// End memory operations.
////////////////////////////////////////////////////////////////////////////////
// Begin string operations.

/**
 * @brief Platform-independent 'memory clear' function (see platform/memory.h).
 * 
 * @param blk The block to initialize. Must be non-zero.
 * @param size The number of bytes to set.
 * @return blk.
 */
void*
platform_memory_clear
(   void*   blk
,   u64     size
);

/**
 * @brief Platform-independent 'memory set' function (see platform/memory.h).
 * 
 * @param blk The block to set. Must be non-zero.
 * @param value The value to set.
 * @param size The number of bytes to set.
 * @return blk.
 */
void*
platform_memory_set
(   void*       blk
,   const i32   value
,   u64         size
);

/**
 * @brief Platform-independent 'memory copy' function (see platform/memory.h).
 * 
 * @param dst The destination block. Must be non-zero.
 * @param src The source block. Must be non-zero.
 * @param size The number of bytes to copy.
 * @return dst.
 */
void*
platform_memory_copy
(   void*       dst
,   const void* src
,   u64         size
);

/**
 * @brief Platform-independent 'memory move' function (see platform/memory.h).
 * 
 * @param dst The destination block. Must be non-zero.
 * @param src The source block. Must be non-zero.
 * @param size The number of bytes to move.
 * @return dst.
 */
void*
platform_memory_move
(   void*       dst
,   const void* src
,   u64         size
);

/**
 * @brief Platform-independent 'memory equal' function (see platform/memory.h).
 * 
 * @param s1 A string. Must be non-zero.
 * @param s2 A string. Must be non-zero.
 * @param size The number of bytes to compare.
 * @return true if strings are equal; false otherwise.
 */
bool
platform_memory_equal
(   const void* s1
,   const void* s2
,   u64         size
);

/**
 * @brief Platform-independent 'string length' function (see core/string.h).
 * 
 * @param string The null-terminated string to read. Must be non-zero.
 * @return The number of characters in string.
 */
u64
platform_string_length
(   const char* string
);

/**
 * @brief Clamped variant of platform_string_length for unsafe strings.
 * 
 * @param string The string to read. Must be non-zero.
 * @param limit The maximum number of characters to read from string.
 * @return The number of characters in string, or limit if string contains more
 * than limit characters.
 */
u64
platform_string_length_clamped
(   const char* string
,   u64         limit
);

// End string operations.
////////////////////////////////////////////////////////////////////////////////
// Begin array operations.

/**
 * @brief Platform-independent function to sort an array in-place.
 * 
 * Current implementation uses quicksort algorithm.
 * AVERAGE CASE TIME COMPLEXITY : O(n log(n))
 * WORST CASE TIME COMPLEXITY   : O(n²)
 * 
 * @param array The array to sort. Must be non-zero.
 * @param array_length The number of elements contained by the array.
 * @param array_stride The array stride. Must be non-zero.
 * @param comparator A function which compares two array elements.
 * Must be non-zero.
 */
void
platform_array_sort
(   void*                   array
,   u64                     array_length
,   u64                     array_stride
,   comparator_function_t   comparator
);

// End array operations.
////////////////////////////////////////////////////////////////////////////////
// Begin filesystem operations.

#include "platform/filesystem.h"

/**
 * @brief Platform-independent 'file exists' function
 * (see platform/filesystem.h).
 * 
 * @param path The filepath to test.
 * @param mode Mode flag.
 * @return true if file exists; false otherwise.
 */
bool
platform_file_exists
(   const char* path
,   FILE_MODE   mode
);

/**
 * @brief Platform-independent 'file open' function (see platform/filesystem.h).
 * 
 * @param path The filepath.
 * @param mode Mode flag.
 * @param file Output buffer for file handle.
 * @return true if file opened successfully; false otherwise.
 */
bool
platform_file_open
(   const char* path
,   FILE_MODE   mode
,   file_t*     file_
);

/**
 * @brief Platform-independent 'file close' function
 * (see platform/filesystem.h).
 * 
 * @param file Handle to the file to close.
 */
void
platform_file_close
(   file_t* file_
);

/**
 * @brief Platform-independent 'file update' function
 * (see platform/filesystem.h).
 * 
 * @param file Handle to the file to update.
 * @return true on success; false if both the file size and read-write position
 * were unchanged.
 */
bool
platform_file_update
(   file_t* file_
);

/**
 * @brief Platform-independent 'file path' function (see platform/filesystem.h).
 * 
 * @param file Handle to a file.
 * @return The filepath of file.
 */
const char*
platform_file_path
(   file_t* file_
);

/**
 * @brief Platform-independent 'file mode' function (see platform/filesystem.h).
 * 
 * @param file Handle to a file.
 * @return The mode of file.
 */
FILE_MODE
platform_file_mode
(   file_t* file_
);

/**
 * @brief Platform-independent 'file size' function (see platform/filesystem.h).
 * 
 * @param file Handle to a file.
 * @return The filesize of file in bytes.
 */
u64
platform_file_size
(   file_t* file_
);

/**
 * @brief Platform-independent 'file position get' function
 * (see platform/filesystem.h).
 * 
 * @param file Handle to a file.
 * @return The current read-write position within file.
 */
u64
platform_file_position_get
(   file_t* file_
);

/**
 * Platform-independent 'file position set' function
 * (see platform/filesystem.h).
 * 
 * @param file Handle to a file.
 * @param position The position to set.
 * @return true on success; false otherwise.
 */
bool
platform_file_position_set
(   file_t* file_
,   u64     position
);

/**
 * @brief Platform-independent 'file read' function (see platform/filesystem.h).
 * 
 * @param file Handle to the file to read.
 * @param size Number of bytes to read.
 * @param dst Output buffer for the content.
 * @param read Output buffer to hold number of bytes read.
 * @return true if file read into dst successfully; false otherwise.
 */
bool
platform_file_read
(   file_t* file_
,   u64     size
,   void*   dst
,   u64*    read
);

/**
 * @brief Platform-independent 'file read line' function
 * (see platform/filesystem.h).
 * 
 * Uses dynamic memory allocation. Call string_destroy to free.
 * 
 * @param file Handle to the file to read.
 * @param dst Output buffer to hold the handle to the resizable output string.
 * @return true if the handle stored in dst is valid; false otherwise.
 */
bool
platform_file_read_line
(   file_t* file_
,   char**  dst
);

/**
 * @brief Platform-independent 'file read all' function
 * (see platform/filesystem.h).
 * 
 * Uses dynamic memory allocation. Call string_free to free.
 * (see core/string.h)
 * 
 * @param file Handle to the file to read.
 * @param dst Output buffer to hold the handle to the null-terminated output
 * string.
 * @param read Output buffer to hold number of bytes read.
 * @return true if the handle stored in dst is valid; false otherwise.
 */
bool
platform_file_read_all
(   file_t* file_
,   u8**    dst
,   u64*    read
);

/**
 * @brief Platform-independent 'file write' function
 * (see platform/filesystem.h).
 * 
 * @param file Handle to the file to write to.
 * @param size Number of bytes to write.
 * @param src The data to write.
 * @param written Output buffer to hold number of bytes written.
 *
 * @return true if src written to file successfully; false otherwise.
 */
bool
platform_file_write
(   file_t*     file_
,   u64         size
,   const void* src
,   u64*        written
);

/**
 * Platform-independent 'file write line' function
 * (see platform/filesystem.h).
 * 
 * @param file Handle to the file to write to.
 * @param size Number of bytes to write.
 * @param src The string to write.
 *
 * @return true if src written to file successfully; false otherwise.
 */
bool
platform_file_write_line
(   file_t*     file_
,   u64         size
,   const char* src
);

/**
 * @brief Obtains a handle to the host platform's standard input stream.
 * 
 * @param file Output buffer. Must be non-zero.
 */
void
platform_file_stdin
(   file_t* file_
);

/**
 * @brief Obtains a handle to the host platform's standard output stream.
 * 
 * @param file Output buffer. Must be non-zero.
 */
void
platform_file_stdout
(   file_t* file_
);

/**
 * @brief Obtains a handle to the host platform's standard error stream.
 * 
 * @param file Output buffer. Must be non-zero.
 */
void
platform_file_stderr
(   file_t* file_
);

// End filesystem operations.
////////////////////////////////////////////////////////////////////////////////
// Begin error handling.

/**
 * @brief Queries the most recent platform-specific error code.
 * 
 * @return The value of the most recent platform-specific error code.
 */
i64
platform_error_code
( void );

/**
 * @brief Queries the platform-specific error message corresponding to the
 * provided error code. Writes the message into an output buffer.
 * 
 * @param error Error code.
 * @param dst Output buffer.
 * @param dst_length The maximum number of characters to write to dst.
 * @return The number of characters written to dst.
 */
u64
platform_error_message
(   i64     error
,   char*   dst
,   u64     dst_length
);

// End error handling.
////////////////////////////////////////////////////////////////////////////////
// Begin clock operations.

/**
 * @brief Platform-independent function to query the system time
 * (see core/clock.h).
 * 
 * @return The system time.
 */
f64
platform_absolute_time
( void );

// End clock operations.
////////////////////////////////////////////////////////////////////////////////

#endif  // PLATFORM_H