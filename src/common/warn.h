/**
 * @file common/warn.h
 * @brief Preprocessor bindings for disabling compiler warnings.
 */
#ifndef WARN_H
#define WARN_H

#include "common/pragma.h"

/**
 * @brief Disables a warning.
 * 
 * @param warning The compiler warning to disable.
 */
#define DISABLE_WARNING(warning)   \
    PRAGMA ( GCC diagnostic push ) \
    PRAGMA ( GCC diagnostic ignored #warning )

/**
 * @brief Re-enables the most recently disabled compiler warning.
 */
#define REENABLE_WARNING() \
    PRAGMA ( GCC diagnostic pop )

#endif  // WARN_H