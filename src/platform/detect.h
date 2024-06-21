/**
 * @file platform/detect.h
 * @brief Enables compile-time platform detection.
 */
#ifndef PLATFORM_DETECT_H
#define PLATFORM_DETECT_H

// Microsoft.
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    // Windows (64-bit).
    #ifdef _WIN64
        #define PLATFORM_WINDOWS 1
        #define PLATFORM_STRING "Windows 64-bit"
    //  Windows (32-bit).
    #else
        #define PLATFORM_WINDOWS 1
        #define PLATFORM_STRING "Windows 32-bit"
    #endif

// GNU/Linux-based OS.
#elif defined(__linux__) || defined(__gnu_linux__)
    #define PLATFORM_LINUX 1
    #define PLATFORM_STRING "GNU/Linux"
    // Linux: Android OS.
    #if defined(__ANDROID__)
        #define PLATFORM_ANDROID 1
        #define PLATFORM_STRING "Android"
    #endif

// Unix-based OS.
#elif defined(__unix__)
    #define PLATFORM_UNIX 1
    #define PLATFORM_STRING "Unix"

// Other POSIX-compliant OS.
#elif defined(_POSIX_VERSION)
    #define PLATFORM_POSIX 1
    #define PLATFORM_STRING "Unknown POSIX OS"

// Apple.
#elif __APPLE__
    #define PLATFORM_APPLE 1
    #include <TargetConditionals.h>
    // Apple: iOS.
    #if TARGET_OS_IPHONE
        #define PLATFORM_IOS 1
        #define PLATFORM_STRING "iOS"
    // Apple: macOS/OSX.
    #elif TARGET_OS_MAC
        #define PLATFORM_MAC 1
        #define PLATFORM_STRING "macOS/OSX"
    // Apple: iOS Emulator.
    #elif TARGET_IPHONE_SIMULATOR
        #define PLATFORM_IOS 1
        #define PLATFORM_IOS_EMULATOR 1
        #define PLATFORM_STRING "iOS Emulator"
    // Apple: Unknown.
    #else
        #error "Unknown Apple platform."
    #endif

// Other.
#else
    #error "Unknown platform."
#endif

#endif  // PLATFORM_DETECT_H