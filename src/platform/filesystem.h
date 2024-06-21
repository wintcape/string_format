/**
 * @file platform/filesystem.h
 * @brief Provides an interface for interacting with files on the host platform.
 */
#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "common.h"

/** @brief Type definition for a file. */
typedef struct
{
    void*   handle;
    bool    valid;
}
file_t;

/** @brief Type and instance definitions for file modes. */
typedef enum
{
    FILE_MODE_ACCESS = 0x0
,   FILE_MODE_READ   = 0x1
,   FILE_MODE_WRITE  = 0x2
}
FILE_MODE;

/**
 * @brief Tests if a file with the provided mode exists at the provided path on
 * the host platform.
 * 
 * FILE_MODE_ACCESS : Test only for file existence.
 * FILE_MODE_READ   : Test for read-only file.
 * FILE_MODE_WRITE  : Test for write-only file.
 * FILE_MODE_READ |
 * FILE_MODE_WRITE  : Test for file with both read and write permission.
 * 
 * @param path The filepath to test.
 * @param mode Mode flag.
 * @return true if file exists; false otherwise.
 */
bool
file_exists
(   const char* path
,   FILE_MODE   mode
);

/**
 * @brief Attempts to open a file on the host platform.
 * 
 * File size and read-write position is precomputed and handled **internally.**
 * For files whose size or read-write position may change by means other than
 * the functions included by this filesystem header, use file_update to update
 * these fields before invoking the corresponding filesystem function.
 * 
 * @param path The filepath.
 * @param mode Mode flag.
 * @param file Output buffer for file handle.
 * @return true if file opened successfully; false otherwise.
 */
bool
file_open
(   const char* path
,   FILE_MODE   mode
,   file_t*     file
);

/**
 * @brief Attempts to close a file on the host platform.
 * 
 * @param file Handle to the file to close.
 */
void
file_close
(   file_t* file
);

/**
 * @brief Recomputes the size and read-write position of a file.
 * 
 * Useful if the file's size or read-write position may change by means other
 * than the functions included by this filesystem header.
 * 
 * @param file Handle to the file to update.
 * @return true on success; false if both the file size and read-write position
 * were unchanged.
 */
bool
file_update
(   file_t* file
);

/**
 * @brief Queries the filepath of a file on the host platform.
 * 
 * @param file Handle to a file.
 * @return The filepath of file.
 */
const char*
file_path
(   file_t* file
);

/**
 * @brief Queries the mode of a file on the host platform.
 * 
 * @param file Handle to a file.
 * @return The mode of file.
 */
FILE_MODE
file_mode
(   file_t* file
);

/**
 * @brief Queries the size (in bytes) of a file on the host platform.
 * 
 * @param file Handle to a file.
 * @return The filesize of file in bytes.
 */
u64
file_size
(   file_t* file
);

/**
 * @brief Queries the current read-write position within a file on the host
 * platform.
 * 
 * @param file Handle to a file.
 * @return The current read-write position within file.
 */
u64
file_position_get
(   file_t* file
);

/**
 * @brief Sets the current read-write position within a file on the host
 * platform.
 * 
 * @param file Handle to a file.
 * @param position The position to set.
 * @return true on success; false otherwise.
 */
bool
file_position_set
(   file_t* file
,   u64     position
);

/**
 * @brief Reads a specified amount of content from a file on the host platform
 * into an output buffer.
 * 
 * @param file Handle to the file to read.
 * @param size Number of bytes to read.
 * @param dst Output buffer for the content.
 * @param read Output buffer to hold number of bytes read.
 * @return true if file read into dst successfully; false otherwise.
 */
bool
file_read
(   file_t* file
,   u64     size
,   void*   dst
,   u64*    read
);

/**
 * @brief Reads content from a file from the host platform into a resizable
 * string buffer until EOF or line break encountered (see container/string.h).
 * 
 * Uses dynamic memory allocation. Call string_destroy to free.
 * 
 * @param file Handle to the file to read.
 * @param dst Output buffer to hold the handle to the resizable output string.
 * @return true if the handle stored in dst is valid; false otherwise.
 */
bool
file_read_line
(   file_t* file
,   char**  dst
);

/**
 * @brief Generates a copy of the entire contents of a file on the host
 * platform.
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
file_read_all
(   file_t* file
,   u8**    dst
,   u64*    read
);

/**
 * @brief Writes a specified amount of data to a file on the host platform.
 * 
 * Use file_write to explicitly specify string length, or _file_write to compute
 * the length of a null-terminated string before passing it to file_write.
 * 
 * @param file Handle to the file to write to.
 * @param size Number of bytes to write.
 * @param src The data to write.
 * @param written Output buffer to hold number of bytes written.
 *
 * @return true if src written to file successfully; false otherwise.
 */
bool
file_write
(   file_t*     file
,   u64         size
,   const void* src
,   u64*        written
);

#define _file_write(file,src,written)                                         \
    ({                                                                        \
        const char* src__ = (src);                                            \
        file_write ( (file) , _string_length ( src__ ) , src__ , (written) ); \
    })
   

/**
 * @brief Writes a string to file on the host platform and appends the `\n`
 * character.
 * 
 * Use file_write_line to explicitly specify string length, or _file_write_line
 * to compute the length of a null-terminated string before passing it to
 * file_write_line.
 * 
 * @param file Handle to the file to write to.
 * @param size Number of bytes to write.
 * @param src The string to write.
 *
 * @return true if src written to file successfully; false otherwise.
 */
bool
file_write_line
(   file_t*     file
,   u64         size
,   const char* src
);

#define _file_write_line(file,src)                                     \
    ({                                                                 \
        const char* src__ = (src);                                     \
        file_write_line ( (file) , _string_length ( src__ ) , src__ ); \
    })

/**
 * @brief Obtains a handle to the host platform's standard input stream.
 * 
 * @param file Output buffer. Must be non-zero.
 */
void
file_stdin
(   file_t* file
);

/**
 * @brief Obtains a handle to the host platform's standard output stream.
 * 
 * @param file Output buffer. Must be non-zero.
 */
void
file_stdout
(   file_t* file
);

/**
 * @brief Obtains a handle to the host platform's standard error stream.
 * 
 * @param file Output buffer. Must be non-zero.
 */
void
file_stderr
(   file_t* file
);

#endif  // FILESYSTEM_H
