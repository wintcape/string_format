/**
 * @file common/units.h
 * @brief Defines common measurement units.
 */
#ifndef UNITS_H
#define UNITS_H

#define GiB(x) ( (x) * 1024ULL*1024ULL*1024ULL )
#define MiB(x) ( (x) * 1024ULL*1024ULL )
#define KiB(x) ( (x) * 1024ULL )

#define GB(x) ( (x) * 1000ULL*1000ULL*1000ULL )
#define MB(x) ( (x) * 1000ULL*1000ULL )
#define KB(x) ( (x) * 1000ULL )

#endif  // UNITS_H