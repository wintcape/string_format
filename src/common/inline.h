/**
 * @file common/inline.h
 * @brief Preprocessor binding to implement function inlining.
 */
#ifndef INLINE_H
#define INLINE_H

#ifdef _MSC_VER
    #define INLINE      __forceinline
    #define NOINLINE    __declspec ( noinline )
#else
    #define INLINE      static inline
    #define NOINLINE
#endif

#endif  // INLINE_H