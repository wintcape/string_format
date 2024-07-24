/**
 * @file core/string.h
 * @brief Defines common fixed-length string operations. For strings with
 * mutable length, see container/string.h.
 */
#ifndef STRING_UTIL_H
#define STRING_UTIL_H

#include "common.h"

/** @brief Defines the maximum size (in bytes) for a local string buffer. */
#define STACK_STRING_MAX_SIZE \
    KiB ( 32 )

// (see string_i64 and string_u64).
#define STRING_INTEGER_MAX_LENGTH 64 /** @brief Maximum stringified integer length. */
#define STRING_INTEGER_MIN_RADIX  2  /** @brief Minimum radix for string_i64 and string_u64 (see string_i64 and string_u64). */
#define STRING_INTEGER_MAX_RADIX  36 /** @brief Maximum radix for string_i64 and string_u64 (see string_i64 and string_u64). */

// (see string_f64).
#define STRING_FLOAT_MAX_LENGTH    64 /** @brief Maximum stringified floating point number length. */
#define STRING_FLOAT_MAX_PRECISION 20 /** @brief Maximum precision for string_f64 (see string_f64). */

/**
 * @brief Computes the number of characters in a null-terminated string. O(n).
 * 
 * String is assumed to terminate within an accessible memory range.
 * For potentially unsafe strings, use _string_length_clamped instead.
 * 
 * For an O(1) alternative, use string_length instead (see container/string.h).
 * 
 * @param string The null-terminated string to read. Must be non-zero.
 * @return The number of characters in string.
 */
u64
_string_length
(   const char* string
);

/**
 * @brief Clamped variant of _string_length for unsafe strings.
 * 
 * @param string The string to read. Must be non-zero.
 * @param limit The maximum number of characters to read from string.
 * @return The number of characters in string, or limit if string contains more
 * than limit characters.
 */
u64
_string_length_clamped
(   const char* string
,   const u64   limit
);

/**
 * @brief String equality test predicate.
 * 
 * Use string_equal to explicitly specify string length, or _string_equal
 * to compute the lengths of null-terminated strings before passing them to
 * string_equal.
 * 
 * @param s1 A string. Must be non-zero.
 * @param s1_length The number of characters in s1.
 * @param s2 A string. Must be non-zero.
 * @param s2_length The number of characters in s2.
 * @return true if strings are equal; false otherwise.
 */
bool
string_equal
(   const char* s1
,   const u64   s1_length
,   const char* s2
,   const u64   s2_length
);

#define _string_equal(s1,s2)                          \
    ({                                                \
        const char* s1__ = (s1);                      \
        const char* s2__ = (s2);                      \
        string_equal ( s1__ , _string_length ( s1__ ) \
                     , s2__ , _string_length ( s2__ ) \
                     );                               \
    })
    

/**
 * @brief Empty string test predicate.
 * 
 * @param string The string to check. Must be non-zero.
 * @return true if string is empty; false otherwise.
 */
bool
string_empty
(   const char* string
);

/**
 * @brief Searches a string for a substring.
 * 
 * Use string_contains to explicitly specify string length, or _string_contains
 * to compute the lengths of null-terminated strings before passing them to
 * string_contains.
 * 
 * @param search The string to search. Must be non-zero.
 * @param search_length The number of characters in search.
 * @param find The string to find. Must be non-zero.
 * @param find_length The number of characters in find.
 * @param reverse Search in reverse? Y/N
 * @param index Output buffer to hold the index in search at which find was
 * found. Pass 0 to retrieve nothing.
 * @return true if search contains find, or if find_length is zero and
 * search_length is non-zero; false if search does not contain find, or if
 * find_length is greater than search_length.
*/
bool
string_contains
(   const char* search
,   const u64   search_length
,   const char* find
,   const u64   find_length
,   bool        reverse
,   u64*        index
);

#define _string_contains(search,find,reverse,index)              \
    ({                                                           \
        const char* search__ = (search);                         \
        const char* find__ = (find);                             \
        string_contains ( search__ , _string_length ( search__ ) \
                        , find__ , _string_length ( find__ )     \
                        , (reverse)                              \
                        , (index)                                \
                        );                                       \
    })

/**
 * @brief Reverses a string. O(n). In-place.
 * 
 * Use string_reverse to explicitly specify string length, or _string_reverse
 * to compute the length of a null-terminated string before passing it to
 * string_reverse.
 * 
 * @param string The string to reverse. Must be non-zero.
 * @param string_length The number of characters in string.
 * @return The string after reversal.
 */
char*
string_reverse
(   char*       string
,   const u64   string_length
);

#define _string_reverse(string)                                    \
    ({                                                             \
        char* string__ = (string);                                 \
        string_reverse ( string__ , _string_length ( string__ ) ); \
    })

/**
 * @brief Signed integer stringify utility.
 * 
 * @param value A 64-bit signed integer.
 * @param radix Integer radix in the range [2..36] (inclusive).
 * @param dst Output buffer for string. Must be non-zero. Should have access to
 * an adequate number of characters given the supplied value and radix. The
 * maximum amount written to dst is 64 bytes, so dst should have access to a
 * minimum of 65 bytes to guarantee it will never overflow for any valid value
 * and radix combination (+1 byte for terminator).
 * @return The number of characters written to dst.
 */
u64
string_i64
(   i64     value
,   u8      radix
,   char*   dst
);

/**
 * @brief Unsigned integer stringify utility.
 * 
 * @param value A 64-bit unsigned integer.
 * @param radix Integer radix in the range [2..36] (inclusive).
 * @param dst Output buffer for string. Must be non-zero. Should have access to
 * an adequate number of characters given the supplied value and radix. The
 * maximum amount written to dst is 64 bytes, so this should have access to a
 * minimum of 65 bytes to guarantee it will never overflow for any valid value
 * and radix combination (+1 byte for terminator).
 * @return The number of characters written to dst.
 */
u64
string_u64
(   u64     value
,   u8      radix
,   char*   dst
);

/**
 * @brief Floating point number stringify utility.
 * 
 * @param value A 64-bit floating point number.
 * @param precision Floating point precision.
 * @param abbreviated Use abbreviated (scientific) notation? Y/N
 * @param dst Output buffer for string. Must be non-zero. Should have access to
 * an adequate number of characters given the supplied value and precision. The
 * maximum amount written to dst is 64 bytes, so this should have access to a
 * minimum of 65 bytes to guarantee it will never overflow for any valid value
 * and precision combination (+1 byte for terminator).
 * @return The number of characters written to dst.
 */
u64
string_f64
(   f64     value
,   u8      precision
,   bool    abbreviated
,   char*   dst
);

/**
 * @brief Character stringify utility.
 * 
 * @param c The character(s).
 * @return A string copy of c.
 */
#define string_char(...) \
    ( ( char[] ){ __VA_ARGS__ , 0 } )

/**
 * @brief Bytesize to string.
 * 
 * Converts size into appropriate units. The unit abbreviation string is
 * returned, and the converted size is written into the output buffer.
 * 
 * @param size Size in bytes.
 * @param amount Output buffer for size (after conversion to appropriate units).
 * Must be non-zero.
 * @return "GiB" | "MiB" | "KiB" | "B"
 */
const char*
string_bytesize
(   u64     size
,   f64*    amount
);

/**
 * @brief Parses a string as a (radix-10) 64-bit signed integer.
 * 
 * Use to_i64 to explicitly specify string length, or _to_i64 to compute the
 * length of a null-terminated string before passing it to to_i64.
 * 
 * @param string Input string. Must be non-zero.
 * @param string_length The number of characters in string.
 * @param value Output buffer. Must be non-zero.
 * @return true on success; false otherwise.
 */
bool
to_i64
(   const char* string
,   u64         string_length
,   i64*        value
);

#define _to_i64(string,value) \
    to_i64 ( (string) , _string_length ( string ) , (value) )

/**
 * @brief Parses a string as a (radix-10) 64-bit unsigned integer.
 * 
 * Use to_u64 to explicitly specify string length, or _to_u64 to compute the
 * length of a null-terminated string before passing it to to_u64.
 * 
 * @param string Input string. Must be non-zero.
 * @param string_length The number of characters in string.
 * @param value Output buffer. Must be non-zero.
 * @return true on success; false otherwise.
 */
bool
to_u64
(   const char* string
,   u64         string_length
,   u64*        value
);

#define _to_u64(string,value) \
    to_u64 ( (string) , _string_length ( string ) , (value) )

/**
 * @brief Parses a string as a 64-bit floating point number.
 * 
 * Use to_f64 to explicitly specify string length, or _to_f64 to compute the
 * length of a null-terminated string before passing it to to_f64.
 * 
 * Does not **currently** support abbreviated notation.
 * TODO: Implement support for abbreviated notation.
 * 
 * @param string Input string. Must be non-zero.
 * @param string_length The number of characters in string.
 * @param value Output buffer. Must be non-zero.
 * @return true on success; false otherwise.
 */
bool
to_f64
(   const char* string
,   u64         string_length
,   f64*        value
);

#define _to_f64(string,value) \
    to_f64 ( (string) , _string_length ( string ) , (value) )

/**
 * @brief Trims whitespace off front and back of a string. O(n). In-place.
 * 
 * Output buffer should have access to at least as many characters as the source
 * buffer to guarantee no overflow.
 * 
 * For a variant which takes as argument two explicit-length i/o string buffers,
 * use string_trim; to compute the length of a null-terminated string prior to
 * passing it to string_trim, use _string_trim; for a variant which takes as
 * argument a resizable string, use __string_trim (see container/string.h).
 * 
 * @param src The string to trim. Must be non-zero.
 * @param string_length The number of characters in src.
 * @param dst Output buffer. Must be non-zero.
 * @return dst.
 */
char*
string_trim
(   const char* src
,   const u64   src_length
,   char*       dst
);

#define _string_trim(src,dst) \
    string_trim ( (src) , _string_length ( src ) , (dst) )

/**
 * @brief Strips a string of a provided escape sequence. O(n). In-place.
 * 
 * Do not include the backslash in the escape sequence string; ex. :
 *      _string_strip_escape ( string , "{" ) -> ( strips "\\{" )
 * 
 * Output buffer should have access to at least as many characters as the source
 * buffer to guarantee no overflow.
 * 
 * For a variant which takes as arguments explicit-length i/o string buffers,
 * use string_strip_escape; to compute the lengths of null-terminated strings
 * prior to passing them to string_strip_escape, use _string_strip_escape; for a
 * variant which takes as argument a resizable string, use __string_strip_escape
 * (see container/string.h).
 * 
 * @param src The string to trim. Must be non-zero.
 * @param string_length The number of characters in src.
 * @param escape Escape sequence. Must be non-zero.
 * @param escape_length The number of characters in escape.
 * @param dst Output buffer. Must be non-zero.
 * @return dst.
 */
char*
string_strip_escape
(   const char* src
,   const u64   src_length
,   const char* escape
,   u64         escape_length
,   char*       dst
);

#define _string_strip_escape(src,escape,dst)                   \
    string_strip_escape ( (src) , _string_length ( src )       \
                        , (escape) , _string_length ( escape ) \
                        , (dst)                                \
                        )

/**
 * @brief Strips a string of ANSI formatting codes. O(n). In-place.
 * 
 * Output buffer should have access to at least as many characters as the source
 * buffer to guarantee no overflow.
 * 
 * For a variant which takes as arguments explicit-length i/o string buffers,
 * use string_strip_ansi; to compute the lengths of null-terminated strings
 * prior to passing them to string_strip_ansi, use _string_strip_ansi; for a
 * variant which takes as argument a resizable string, use __string_strip_ansi
 * (see container/string.h).
 * 
 * @param src The string to trim. Must be non-zero.
 * @param string_length The number of characters in src.
 * @param dst Output buffer. Must be non-zero.
 * @return dst.
 */
char*
string_strip_ansi
(   const char* src
,   const u64   src_length
,   char*       dst
);

#define _string_strip_ansi(src,dst) \
    string_strip_ansi ( (src) , _string_length ( src ) , (dst) )

/**
 * @brief Allocates memory for a string of the provided size.
 * 
 * Uses dynamic memory allocation. Call string_free to free.
 * 
 * @param size The number of bytes of memory to allocate.
 * @return An empty string of the provided size.
 */
char*
string_allocate
(   u64 size
);

/**
 * @brief Generates a copy of a null-terminated string.
 * 
 * Uses dynamic memory allocation. Call string_free to free.
 * 
 * @param string The null-terminated string to copy. Must be non-zero.
 * @return A copy of string.
 */
char*
string_allocate_from
(   const char* string
);

/**
 * @brief Frees the memory used by a provided string.
 * 
 * @param string The string to free.
 */
void
string_free
(   void* string
);

#endif  // STRING_UTIL_H