/**
 * @author Matthew Weissel (mweissel3@gatech.edu)
 * @file math/div.h
 * @brief Integer division predicates.
 */
#ifndef MATH_DIV_H
#define MATH_DIV_H

#include "common.h"

/**
 * @brief Computes odd(x).
 * 
 * @param x An integer.
 * @return odd(x).
 */
#define odd(x) \
    ( ( bool )( (x) % 2 ) )

/**
 * @brief Computes even(x).
 * 
 * @param x An integer.
 * @return even(x).
 */
#define even(x) \
    ( ( bool )( !odd ( x ) ) )

#endif  // MATH_DIV_H