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
// allocates memory registering a section
// returns a handle
size_t alloc(size_t size){
    printf("Allocating memory from controller \n");
    // get the start from the heap free section

    struct Section* section_ptr = get_unused_section();
    section_ptr->isInUse = true;
    section_ptr->size = size;

    void* start = swipe_alloc_sections(size);
    section_ptr->start = start;

    return section_ptr->handle;
}

// marks the section as in use
void* lock(size_t handle){
    printf("Locked section for use\n");
    return lock_section(handle);
};

int unlock(size_t handle){
    printf("Unlocked section for use\n");
    unlock_section(handle);
    return 0;
}

void* get_addr(size_t handle){
    return get_section_addr(handle);
}

void mem_free(size_t handle){
    free_section(handle);
}

// gets the object's singleton instance that will handle all memory
struct AllocHandler* get_alloc_handler() {
    printf("Getting alloc handler\n");

    static struct AllocHandler handler_instance;
    static bool initialized = false;

    if (!initialized) {
        printf("Created alloc handler \n");
        handler_instance.alloc   = alloc;
        handler_instance.lock    = lock;
        handler_instance.unlock  = unlock;
        handler_instance.get     = get_addr;
        handler_instance.free    = mem_free;
        handler_instance.pack    = pack_mem;

        initialized = true;
    }

    printf("Returned alloc handler \n");
    return &handler_instance;
}


void print_heap_func(){
    print_heap();
}

void print_registry_func(){
    print_registry();
}

