#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "section.h"
#include "heap.h"
#include "registry.h"
#include "alloc_handler.h"

// front facing interface and abstraction from the registry and heap
// interacts with the registry

// creates a new section
struct Section new_section(void* start, size_t size) {
    printf("Created new section\n");
    struct Section s;
    s.start = start;
    s.size = size;
    s.isInUse = true;
    s.isLocked = false;
    return s;
}

// main function to be exposed
// allocates memory by creating and registering a section
// returns a handle
size_t alloc(size_t size){
    printf("Allocating memory from controller \n");
    // get the start from the heap
    void* start = swipe_alloc_sections(size);
    struct Section section = new_section(start, size);
    return register_mem(section);
}

// marks the section as in use
void* lock(size_t handle){
    printf("Locked section for use\n");
    lock_section(handle);
};

void unlock(size_t handle){
    printf("Unlocked section for use\n");
    unlock_section(handle);
}

void* get_addr(size_t handle){
    return get_section_addr(handle);
}

// gets the object's singleton instance that will handle all memory
struct AllocHandler* get_alloc_handler() {
    printf("Getting alloc handler\n");
    static struct AllocHandler* handler = NULL;

    if (handler == NULL) {
        printf("Created alloc handler \n");
        handler = malloc(sizeof(struct AllocHandler));
        if (!handler) return NULL;

        handler->alloc = alloc;
        handler->lock = lock;
        handler->unlock = unlock;
        handler->get = get_addr;
    }
    printf("Returned alloc handler \n");

    return handler;
}

