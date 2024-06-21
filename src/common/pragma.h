/**
 * @file common/pragma.h
 * @brief Preprocessor bindings for executing compiler pragma directives.
 */
#ifndef PRAGMA_H
#define PRAGMA_H

#define PRAGMA(args) \
    _Pragma ( #args )

#endif  // PRAGMA_H