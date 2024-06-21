/**
 * @file platform/memory.c
 * @brief Implementation of the platform/memory header.
 * (see platform/memory.h for additional details)
 */
#include "platform/memory.h"
#include "platform/platform.h"

void*
memory_allocate
(   u64 size
)
{
    return platform_memory_allocate ( size );
}

void
memory_free
(   void* memory
)
{
    platform_memory_free ( memory );
}

void*
memory_clear
(   void*   memory
,   u64     size
)
{
    return platform_memory_clear ( memory , size );
}

void*
memory_set
(   void*   memory
,   i32     value
,   u64     size
)
{
    return platform_memory_set ( memory , value , size );
}

void*
memory_copy
(   void*       dst
,   const void* src
,   u64         size
)
{
    return platform_memory_copy ( dst , src , size );    
}

void*
memory_move
(   void*       dst
,   const void* src
,   u64         size
)
{
    return platform_memory_move ( dst , src , size );    
}

bool
memory_equal
(   const void* s1
,   const void* s2
,   u64         size
)
{
    return platform_memory_equal ( s1 , s2 , size );
}