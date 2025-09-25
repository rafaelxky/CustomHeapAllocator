#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "section.h"
#include "heap.h"
#include "registry.h"
#include "alloc_handler.h"

struct Section new_section(void* start, size_t size) {
    struct Section s;
    s.start = start;
    s.size = size;
    s.isInUse = true;
    s.isLocked = false;
    return s;
}

size_t alloc(size_t size){
    struct Section* section_ptr = get_unused_section();
    section_ptr->isInUse = true;
    section_ptr->size = size;

    void* start = swipe_alloc_sections(size);
    section_ptr->start = start;
    zero(start, size);

    return section_ptr->handle;
}

void* lock(size_t handle){
    return lock_section(handle);
};

int unlock(size_t handle){
    unlock_section(handle);
    return 0;
}

void* get_addr(size_t handle){
    return get_section_addr(handle);
}

void free_mem(size_t handle){
    free_section(handle);
}

void pack(){
    pack_mem();
}

void print_heap_func(){
    print_heap();
}

void print_registry_func(){
    print_registry();
}

