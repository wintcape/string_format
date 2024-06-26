/**
 * @file core/clock.h
 * @brief Provides an interface for a clock data structure.
 */
#ifndef CLOCK_H
#define CLOCK_H

#include "common.h"

/** @brief Type definition for a clock. */
typedef struct
{
    f64 start;
    f64 elapsed;
}
clock_t;

/**
 * @brief Updates the provided clock.
 * 
 * To be invoked just before checking the elapsed time.
 * Has no effect on clocks that have not been started.
 * 
 * @param clock The clock to update. Must be non-zero.
 */
void
clock_update
(   clock_t* clock
);

/**
 * @brief Resets the elapsed time for the provided clock and starts it.
 * 
 * @param clock The clock to start. Must be non-zero.
 */
void
clock_start
(   clock_t* clock
);

/**
 * @brief Stops the provided clock.
 * 
 * @param clock The clock to stop. Must be non-zero.
 */
void
clock_stop
(   clock_t* clock
);

#endif  // CLOCK_H