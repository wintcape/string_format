/**
 * @file common/static_assert.h
 * @brief Preprocessor binding to implement compile-time assertions.
 */
#ifndef STATIC_ASSERT_H
#define STATIC_ASSERT_H

#if defined(__clang__) || defined(__GNUC__)
#define STATIC_ASSERT _Static_assert
#else
#define STATIC_ASSERT static_assert
#endif

#endif  // STATIC_ASSERT_H