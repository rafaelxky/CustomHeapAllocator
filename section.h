#ifndef SECTION_H
#define SECTION_H

#include <stdbool.h>
#include <stddef.h>

// defines beggining, end, free status, and in use status
// if free, allows handler to override its start and size
// if not in use, allow handler to change its start 
typedef struct Section
{
    void* start;
    size_t size;
    bool isInUse;
    bool isLocked;
    size_t handle;
} Section;

#endif