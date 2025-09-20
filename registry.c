#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "section.h"

// interacts with the heap
#define REGISTRY_SIZE 64
Section registry[64];
size_t offset = -1;

// register mem in the central place
// returns a handle
size_t register_mem(Section section) {
    offset += 1;
    registry[offset] = section;
    return offset;
}

// retreives the section by handle
Section get_mem_section(size_t handle_id){
    return registry[handle_id];
}

// allows for modification of the value
void* lock_section(size_t handle){
    Section section = registry[handle];
    section.isInUse = true;
    return section.start;
}

void* get_section_addr(size_t handle){
    Section section = registry[handle];
    if (section.isLocked)
    {
        return section.start;
    }
    return NULL;
}

// lock the value so the program may clean it
void unlock_section(size_t handle){
    Section section = registry[handle];
    section.isInUse = false;
}

// change this to remove redundant checks
bool is_not_occupied(void* addr){
    for (int i = 0; i < REGISTRY_SIZE; i++) {
        if (!registry[i].isInUse) {
            return true; 
        }
        if ((uint8_t*)addr >= (uint8_t*)registry[i].start &&
            (uint8_t*)addr <  (uint8_t*)registry[i].start + registry[i].size) {
            return false; 
        }
    }
    return true; 
}