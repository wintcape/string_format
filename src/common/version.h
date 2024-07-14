/**
 * @file common/version.h
 * @brief Version info.
 */
#ifndef VERSION_H
#define VERSION_H

#define VERSION_MAJOR   0 /** @brief Major version number. */
#define VERSION_MINOR   0 /** @brief Minor version number. */
#define VERSION_PATCH   9 /** @brief Patch version number. */

#define VERSION_RELEASE 0 /** @brief Release build? Y/N */
#define VERSION_DEBUG   1 /** @brief Debug mode? Y/N */

// Auto-disables debug mode for release builds.
#if VERSION_RELEASE == 1
#define VERSION_DEBUG 0
#endif

#endif  // VERSION_H
