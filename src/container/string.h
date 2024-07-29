/**
 * @file container/string.h
 * @brief Provides an interface for a resizable string data structure.
 */
#ifndef STRING_H
#define STRING_H

#include "container/array.h"
#include "container/string/format.h"
#include "core/string.h"

/** @brief Type declaration for a resizable string. */
typedef char string_t;

/** @brief Defines resizable string default capacity. */
#define STRING_DEFAULT_CAPACITY 64

/**
 * @brief Allocates memory for a resizable string.
 * 
 * Use _string_create to explicitly specify initial capacity, or string_create
 * to use the default.
 * 
 * Uses dynamic memory allocation. Call string_destroy to free.
 * 
 * @param initial_capacity The initial capacity for the string backend array.
 * @return An empty resizable string with the specified backend array capacity.
 */
string_t*
__string_create
(   ARRAY_FIELD initial_capacity
);

#define string_create() \
    __string_create ( STRING_DEFAULT_CAPACITY )

#define _string_create(initial_capacity) \
    __string_create ( initial_capacity )

/**
 * @brief Creates a resizable copy of an existing string. O(n).
 * 
 * Use string_copy to explicitly specify string length, or string_create_from
 * to compute the length of a null-terminated string ( O(n) ) before passing it
 * to __string_copy. If the string being copied is itself a resizable string
 * (i.e. a string itself created via the string_create class of functions),
 * _string_copy may be used to implicitly fetch the current length of the
 * resizable string ( O(1) ) before passing it to __string_copy.
 * 
 * Uses dynamic memory allocation. Call string_destroy to free.
 * 
 * @param src The string to copy. Must be non-zero.
 * @param src_length The number of characters to copy from src.
 * @return A resizable copy of s.
 */
string_t*
__string_copy
(   const char* src
,   const u64   src_length
);

#define string_copy(string,length) \
    __string_copy ( (string) , (length) )

#define _string_copy(string)                                       \
    ({                                                             \
        const string_t* string__ = (string);                       \
        __string_copy ( (string__) , string_length ( string__ ) ); \
    })

#define string_create_from(string)                                  \
    ({                                                              \
        const char* string__ = (string);                            \
        __string_copy ( (string__) , _string_length ( string__ ) ); \
    })

/**
 * @brief Frees the memory used by a provided resizable string.
 * 
 * @param string The resizable string to free.
 */
void
string_destroy
(   string_t* string
);

/**
 * @brief Reads the current length of a resizable string. O(1).
 * 
 * @param string A resizable string. Must be non-zero.
 * @return The number of characters currently contained by string.
 */
u64
string_length
(   const string_t* string
);

/**
 * @brief Appends to a resizable string. Amortized O(1).
 * 
 * Use string_append to explicitly specify string length, or _string_append
 * to compute the length of a null-terminated string before passing it to
 * __string_append.
 * 
 * @param string The resizable string to append to. Must be non-zero.
 * @param src The string to append. Must be non-zero.
 * @param src_length The number of characters to copy from src.
 * @return The resizable string (possibly with new address).
 */
char*
__string_append
(   string_t*   string
,   const char* src
,   const u64   src_length
);

#define string_append(string,src,length) \
    ( (string) = __string_append ( (string) , (src) , (length) ) )

#define _string_append(string,src)                            \
    ({                                                        \
        const char* src__ = (src);                            \
        (string) = __string_append ( (string)                 \
                                   , src__                    \
                                   , _string_length ( src__ ) \
                                   );                         \
    })

/**
 * @brief Prepends to a resizable string. O(n).
 * 
 * Use string_prepend to explicitly specify string length, or _string_prepend
 * to compute the length of a null-terminated string before passing it to
 * __string_prepend.
 * 
 * @param string The resizable string to prepend to. Must be non-zero.
 * @param src The string to prepend. Must be non-zero.
 * @param src_length The number of characters to copy from src.
 * @return The resizable string (possibly with new address).
 */
string_t*
__string_prepend
(   string_t*   string
,   const char* src
,   const u64   src_length
);

#define string_prepend(string,src,length) \
    ( (string) = __string_prepend ( (string) , (src) , (length) ) )

#define _string_prepend(string,src)                            \
    ({                                                         \
        const string_t* src__ = (src);                         \
        (string) = __string_prepend ( (string)                 \
                                    , src__                    \
                                    , _string_length ( src__ ) \
                                    );                         \
    })

/**
 * @brief Inserts into a resizable string. O(n).
 * 
 * Use string_insert to explicitly specify string length, or _string_insert
 * to compute the length of a null-terminated string before passing it to
 * __string_insert.
 * 
 * @param string The resizable string to append to. Must be non-zero.
 * @param index The index to insert at.
 * @param src The string to insert. Must be non-zero.
 * @param src_length The number of characters to copy from src.
 * @return The resizable string (possibly with new address).
 */
string_t*
__string_insert
(   string_t*   string
,   u64         index
,   const char* src
,   const u64   src_length
);

#define string_insert(string,index,src,length) \
    ( (string) = __string_insert ( (string) , (index) , (src) , (length) ) )

#define _string_insert(string,index,src)                      \
    ({                                                        \
        const char* src__ = (src);                            \
        (string) = __string_insert ( (string)                 \
                                   , (index)                  \
                                   , src__                    \
                                   , _string_length ( src__ ) \
                                   );                         \
    })

/**
 * @brief Removes a substring from a resizable string. O(n).
 * 
 * @param string The resizable string to remove from. Must be non-zero.
 * @param index The starting index of the substring to remove.
 * @param count The number of characters to remove.
 * @return The resizable string with the substring removed.
 */
string_t*
__string_remove
(   string_t*   string
,   u64         index
,   u64         count
);

#define string_remove(string,index,count) \
    __string_remove ( (string) , (index) , (count) )

/**
 * @brief **Effectively** clears a resizable string. O(1).
 * 
 * @param string The resizable string to clear. Must be non-zero.
 * @return The resizable string set to empty.
 */
string_t*
__string_clear
(   string_t* string
);

#define string_clear(string) \
    __string_clear ( string )

/**
 * @brief **Effectively** truncates a resizable string. O(1).
 * 
 * @param string The resizable string to truncate. Must be non-zero.
 * @param length The new string length.
 * @return The resizable string truncated to length characters.
 */
string_t*
__string_truncate
(   string_t*   string
,   u64         length
);

#define string_truncate(string,length) \
    __string_truncate ( (string) , (length) )

/**
 * @brief Replaces all instances of a substring within a string with a different
 * substring. O(n).
 * 
 * Use string_replace to explicitly specify string length, or _string_replace
 * to compute the lengths of null-terminated strings before passing them to
 * __string_replace.
 * 
 * @param string The resizable string to mutate. Must be non-zero.
 * @param remove The substring to remove. Must be non-zero.
 * @param replace The substring to replace the removed substring with.
 * Must be non-zero.
 * @return The resizable string with all instances of the removed substring
 * replaced by the replacement substring (possibly with new address).
 */
string_t*
__string_replace
(   string_t*   string
,   const char* remove
,   const u64   remove_length
,   const char* replace
,   const u64   replace_length
);

#define string_replace(string,remove,remove_length,replace,replace_length) \
    ( (string) = __string_replace ( (string)                               \
                                  , (remove)                               \
                                  , (remove_length)                        \
                                  , (replace)                              \
                                  , (replace_length)                       \
                                  ) )

#define _string_replace(string,remove,replace)                     \
    ({                                                             \
        const char* remove__ = (remove);                           \
        const char* replace__ = (replace);                         \
        (string) = __string_replace ( (string)                     \
                                    , remove__                     \
                                    , _string_length ( remove__ )  \
                                    , replace__                    \
                                    , _string_length ( replace__ ) \
                                    );                             \
    })

/**
 * @brief Trims whitespace off front and back of a string. O(n). In-place.
 *
 * Does **not** handle strings which contain zero-bytes (i.e. binary strings).
 * 
 * For a variant which takes as argument two fixed-length i/o string buffers,
 * use string_trim (see core/string.h); for a variant which takes as argument
 * a resizable string, use __string_trim.
 * 
 * @param string The string to trim. Must be non-zero.
 * @return The resizable string with whitespace trimmed off the front and back.
 */
string_t*
__string_trim
(   string_t* string
);

/**
 * @brief Strips a string of a provided escape sequence. O(n). In-place.
 * 
 * Do not include the backslash in the escape sequence string; ex. :
 *      __string_strip_escape ( string , "{" , 1 )
 * 
 * For a variant which takes as argument two fixed-length i/o string buffers,
 * use string_strip_escape (see core/string.h); for a variant which takes as
 * argument a resizable string, use __string_strip_escape.
 * 
 * @param string The resizable string to strip. Must be non-zero.
 * @param escape Escape sequence. Must be non-zero.
 * @param escape_length The number of characters in escape.
 * @return The resizable string stripped of the provided escape sequence.
 */
string_t*
__string_strip_escape
(   string_t*   string
,   const char* escape
,   u64         escape_length
);

/**
 * @brief Strips a string of ANSI formatting codes. O(n). In-place.
 * 
 * For a variant which takes as argument two fixed-length i/o string buffers,
 * use string_strip_ansi (see core/string.h); for a variant which takes as
 * argument a resizable string, use __string_strip_ansi.
 * 
 * @param string The resizable string to strip. Must be non-zero.
 * @return The resizable string stripped of ANSI formatting codes.
 */
string_t*
__string_strip_ansi
(   string_t* string
);

#endif  // STRING_H