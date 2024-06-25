/**
 * @file container/string/format.h
 * @brief Provides a custom string formatting interface.
 */
#ifndef STRING_FORMAT_H
#define STRING_FORMAT_H

#include "common.h"

/** @brief (see container/string.h). */
typedef char string_t;

/** @brief Type and instance definitions for format specifier tags. */
typedef enum
{
    STRING_FORMAT_SPECIFIER_IGNORE
,   STRING_FORMAT_SPECIFIER_RAW
,   STRING_FORMAT_SPECIFIER_INTEGER
,   STRING_FORMAT_SPECIFIER_FLOATING_POINT
,   STRING_FORMAT_SPECIFIER_FLOATING_POINT_SHOW_FRACTIONAL
,   STRING_FORMAT_SPECIFIER_FLOATING_POINT_ABBREVIATED
,   STRING_FORMAT_SPECIFIER_FLOATING_POINT_FRACTIONAL_ONLY
,   STRING_FORMAT_SPECIFIER_ADDRESS
,   STRING_FORMAT_SPECIFIER_CHARACTER
,   STRING_FORMAT_SPECIFIER_STRING
,   STRING_FORMAT_SPECIFIER_RESIZABLE_STRING
,   STRING_FORMAT_SPECIFIER_FILE_INFO

,   STRING_FORMAT_SPECIFIER_COUNT
}
STRING_FORMAT_SPECIFIER;

/** @brief Type and instance definitions for format modifier tags. */
typedef enum
{
    STRING_FORMAT_MODIFIER_WILDCARD
,   STRING_FORMAT_MODIFIER_PAD_LEFT
,   STRING_FORMAT_MODIFIER_PAD_RIGHT
,   STRING_FORMAT_MODIFIER_SHOW_SIGN
,   STRING_FORMAT_MODIFIER_HIDE_SIGN
,   STRING_FORMAT_MODIFIER_FIX_PRECISION
,   STRING_FORMAT_MODIFIER_RADIX
,   STRING_FORMAT_MODIFIER_ARRAY
,   STRING_FORMAT_MODIFIER_RESIZABLE_ARRAY

,   STRING_FORMAT_MODIFIER_COUNT
}
STRING_FORMAT_MODIFIER;

#define STRING_FORMAT_SPECIFIER_INVALID STRING_FORMAT_SPECIFIER_COUNT /** @brief An alias for detecting an invalid format specifier tag. */
#define STRING_FORMAT_MODIFIER_INVALID  STRING_FORMAT_MODIFIER_COUNT  /** @brief An alias for detecting an invalid format modifier tag. */

#define STRING_FORMAT_SPECIFIER_TOKEN_ID                             "%"    /** @brief The format specifier token. */
                                                                            /** @brief Format specifier: ignore. */
#define STRING_FORMAT_SPECIFIER_TOKEN_IGNORE                         STRING_FORMAT_SPECIFIER_TOKEN_ID

#define STRING_FORMAT_SPECIFIER_TOKEN_RAW                            "u"    /** @brief Format specifier: raw. */
#define STRING_FORMAT_SPECIFIER_TOKEN_INTEGER                        "i"    /** @brief Format specifier: integer. */
#define STRING_FORMAT_SPECIFIER_TOKEN_FLOATING_POINT                 "f"    /** @brief Format specifier: floating point. */
#define STRING_FORMAT_SPECIFIER_TOKEN_FLOATING_POINT_SHOW_FRACTIONAL "F"    /** @brief Format specifier: floating point (always show fractional). */
#define STRING_FORMAT_SPECIFIER_TOKEN_FLOATING_POINT_ABBREVIATED     "e"    /** @brief Format specifier: floating point (abbreviated notation). */
#define STRING_FORMAT_SPECIFIER_TOKEN_FLOATING_POINT_FRACTIONAL_ONLY "d"    /** @brief Format specifier: floating point (fractional only). */
#define STRING_FORMAT_SPECIFIER_TOKEN_ADDRESS                        "@"    /** @brief Format specifier: address. */
#define STRING_FORMAT_SPECIFIER_TOKEN_CHARACTER                      "c"    /** @brief Format specifier: character. */
#define STRING_FORMAT_SPECIFIER_TOKEN_STRING                         "s"    /** @brief Format specifier: string. */
#define STRING_FORMAT_SPECIFIER_TOKEN_RESIZABLE_STRING               "S"    /** @brief Format specifier: resizable string. */
#define STRING_FORMAT_SPECIFIER_TOKEN_FILE_INFO                      "file" /** @brief Format specifier: file info. */

#define STRING_FORMAT_MODIFIER_TOKEN_PAD                             "P"    /** @brief Format modifier: pad. */
#define STRING_FORMAT_MODIFIER_TOKEN_PAD_MINIMUM                     "p"    /** @brief Format modifier: pad (minimum width). */
#define STRING_FORMAT_MODIFIER_TOKEN_LEFT                            "l"    /** @brief Format modifier: left. */
#define STRING_FORMAT_MODIFIER_TOKEN_RIGHT                           "r"    /** @brief Format modifier: right. */
#define STRING_FORMAT_MODIFIER_TOKEN_SHOW_SIGN                       "+"    /** @brief Format modifier: show sign. */
#define STRING_FORMAT_MODIFIER_TOKEN_HIDE_SIGN                       "-"    /** @brief Format modifier: hide sign. */
#define STRING_FORMAT_MODIFIER_TOKEN_FIX_PRECISION                   "."    /** @brief Format modifier: fix precision. */
#define STRING_FORMAT_MODIFIER_TOKEN_RADIX                           "r"    /** @brief Format modifier: radix. */
#define STRING_FORMAT_MODIFIER_TOKEN_ARRAY                           "a"    /** @brief Format modifier: array. */
#define STRING_FORMAT_MODIFIER_TOKEN_RESIZABLE_ARRAY                 "A"    /** @brief Format modifier: resizable array. */
#define STRING_FORMAT_MODIFIER_TOKEN_WILDCARD                        "?"    /** @brief Format modifier: wildcard. */

/**
 * @brief String format function.
 * 
 * Accepts a variadic argument list (see common/args.h).
 *
 * Uses dynamic memory allocation. Call string_destroy to free.
 * 
 * FORMAT SPECIFIERS :
 * 
 * %% : Ignore (i.e. "%" character).
 * %u : Unsigned.
 * %i : Signed integer.
 * %F : Floating point number. The corresponding argument must be the address
 *      of an f64. (For additional information about this limitation, see
 *      common/args.h).
 * %f : Floating point number. The corresponding argument must be the address
 *      of an f64. (For additional information about this limitation, see
 *      common/args.h). If the value stored at the address is a whole number,
 *      no decimal point or fractional part will be printed.
 * %e : Floating point number (short-notation). The corresponding argument
 *      must be the address of an f64. (For additional information about this
 *      limitation, see common/args.h).
 * %d : Floating point number (fractional-only). The corresponding argument
 *      must be the address of an f64. (For additional information about this
 *      limitation, see common/args.h).
 * %@ : Address.
 * %c : Single character.
 * %s : Null-terminated string of characters.
 *      Length is computed at runtime via O(n) _string_length.
 * %S : Resizable string of characters.
 *      This includes any string created with the __string_create class of
 *      functions. Length is fetched at runtime via O(1) string_length.
 * %file : File info. The corresponding argument must be a file handle.
 *      
 * FORMAT MODIFIERS :
 * 
 * These may each be used once preceding a format specifier. They only apply
 * to arguments of a sensible type for their purpose.
 * 
 * - Pl<character><number>{} : Fix print width to <number>. If needed, pad with
 *                             <character> to the left.
 *                             Works with any number of format specifiers
 *                             contained within the braces. Can be nested.
 *                             Use `\{` and `\}` to print braces.
 * - Pr<character><number>{} : Fix print width to <number>. If needed, pad with
 *                             <character> to the right.
 *                             Works with any number of format specifiers
 *                             contained within the braces. Can be nested.
 *                             Use `\{` and `\}` to print braces.
 * - pl<character><number>{} : Set minimum print width to <number>. If needed,
 *                             pad with <character> to the left.
 *                             Works with any number of format specifiers
 *                             contained within the braces. Can be nested.
 *                             Use `\{` and `\}` to print braces.
 * - pr<character><number>{} : Set minimum print width to <number>. If needed,
 *                             pad with <character> to the right.
 *                             Works with any number of format specifiers
 *                             contained within the braces. Can be nested.
 *                             Use `\{` and `\}` to print braces.
 * - + : Always print sign. Default behavior is to print the sign only for a
 *       negative number.
 *       Works only with signed numeric format specifiers: %f, %F, %e, %i.
 * - - : Never print sign. Default behavior is to print the sign only for a
 *       negative number.
 *       Works only with signed numeric format specifiers: %f, %F, %e, %i.
 * - .<number> : Fix fractional precision to <number> decimal places.
 *               Works only with floating point format specifiers: %f, %F, %e,
 *                                                                 %d.
 * - r<number> : Print integer in radix <number> format.
 *               Works only with integral format specifiers: %i, %u.
 * - A : Resizable array. The argument must be an array created with the
 *       _array_create class of functions.
 *       Works with any format specifier; the format specifier specifies the
 *       print method for each array element.
 * - a : Fixed-length array. Consumes **three** arguments, in the following
 *       order: (1) address of the array, (2) array length, (3) array stride.
 *       Works with any format specifier; the format specifier specifies the
 *       print method for each array element.
 * - ? : Wildcard. Use in-place of <character> or <number> in any viable format
 *       modifier to set the value from an argument rather than from the format
 *       string itself. Consumes **one** additional argument.
 *
 * @param format Formatting string.
 * @param args Variadic argument list (see common/args.h).
 * @return The formatted string.
 */
string_t*
_string_format
(   const char* format
,   args_t      args
);

/** @brief Alias for calling _string_format with __VA_ARGS__. */
#define string_format(format,...)                           \
    ({                                                      \
        DISABLE_WARNING ( -Wint-conversion )                \
        _string_format ( (format) , ARGS ( __VA_ARGS__ ) ); \
        REENABLE_WARNING ()                                 \
    })

#endif // STRING_FORMAT_H