/**
 * @file common/ascii.h
 * @brief ASCII datatype operations.
 */
#ifndef ASCII_H
#define ASCII_H

#include "common/inline.h"
#include "common/types.h"

/**
 * @brief Uppercase? Y/N
 * 
 * @param c An ASCII character.
 * @return true if c uppercase; false otherwise.
 */
INLINE
bool
uppercase
(   const char c
)
{
    return c >= 'A' && c <= 'Z';
}

/**
 * @brief Lowercase? Y/N
 * 
 * @param c An ASCII character.
 * @return true if c lowercase; false otherwise.
 */
INLINE
bool
lowercase
(   const char c
)
{
    return c >= 'a' && c <= 'z';
}

/**
 * @brief Latin alphabet? Y/N
 * 
 * @param c An ASCII character.
 * @return true if c is a character in the Latin alphabet; false otherwise.
 */
INLINE
bool
alpha
(   const char c
)
{
    return uppercase ( c ) || lowercase ( c );
}

/**
 * @brief Arabic numeral? Y/N
 * 
 * @param c An ASCII character.
 * @return true if c is an arabic numeral character; false otherwise.
 */
INLINE
bool
digit
(   const char c
)
{
    return c >= '0' && c <= '9';
}

/**
 * @brief Alphanumeric? Y/N
 * 
 * @param c An ASCII character.
 * @return true if c is alphanumeric; false otherwise.
 */
INLINE
bool
alphanumeric
(   const char c
)
{
    return alpha ( c ) || digit ( c );
}

/**
 * @brief Whitespace? Y/N
 * 
 * @param c An ASCII character.
 * @return true if c is whitespace; false otherwise.
 */
INLINE
bool
whitespace
(   const char c
)
{
    return c ==  ' ' || c == '\n' || c == '\t'
        || c == '\v' || c == '\f' || c == '\r'
        ;
}

/**
 * @brief Newline? Y/N
 * 
 * @param c An ASCII character.
 * @return true if c is newline; false otherwise.
 */
INLINE
bool
newline
(   const char c
)
{
    return c == '\n' || c == '\r';
}

/**
 * @brief Obtains an uppercase representation of a lowercase character.
 * 
 * @param c An ASCII character.
 * @return The uppercase version of c, if c is a lowercase character in the
 * Latin alphabet. Otherwise, c.
 */
INLINE
char
to_uppercase
(   const char c
)
{
    return lowercase ( c ) ? c - 32
                           : c
                           ;
}

/**
 * @brief Obtains a lowercase representation of an uppercase character.
 * 
 * @param c An ASCII character.
 * @return The lowercase version of c, if c is an uppercase character in the
 * Latin alphabet. Otherwise, c.
 */
INLINE
char
to_lowercase
(   const char c
)
{
    return uppercase ( c ) ? c + 32
                           : c
                           ;
}

/**
 * @brief Obtains an integer representation of an Arabic numeral character.
 * 
 * @param c An unsigned ASCII character.
 * @return An integer representation of c, if c is an Arabic numeral character.
 * Otherwise, c.
 */
INLINE
u8
to_digit
(   const unsigned char c
)
{
    return digit ( c ) ? c - ( ( unsigned char ) '0' )
                       : c
                       ;
}

/**
 * @brief Obtains the Arabic numeral ASCII character corresponding to a provided
 * single-digit integer.
 * 
 * @param digit An integer value in the range [ 0 , 9 ].
 * @return The Arabic numeral ASCII character corresponding to digit, if it is a
 * single base-10 digit. Otherwise, digit.
 */
INLINE
unsigned char
char_digit
(   const u8 digit
)
{
    return ( digit < 10 ) ? ( ( unsigned char ) '0' ) + digit
                          : digit
                          ;
}

#endif  // ASCII_H