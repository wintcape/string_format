/**
 * @author Matthew Weissel
 * @file core/clock.c
 * @brief Implementation of the core/clock header.
 * (see core/clock.h for additional details)
 */
#include "core/clock.h"

#include "platform/platform.h"

void
clock_update
(   clock_t* clock
)
{
    if ( clock->start )
    {
        clock->elapsed = platform_absolute_time () - clock->start;
    }
}

void
clock_start
(   clock_t* clock
)
{
    clock->start = platform_absolute_time ();
    clock->elapsed = 0;
}

void
clock_stop
(   clock_t* clock
)
{
    clock->start = 0;
}