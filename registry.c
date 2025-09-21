#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "section.h"
#include <stdio.h>

// interacts with the heap
#define REGISTRY_SIZE 64
Section registry[64];
size_t offset = 0;

// register mem in the central place
// returns a handle
size_t register_mem(Section section) {
    section.handle = offset;
    registry[offset] = section;
    offset += 1;
    return section.handle;
}

// retreives the section by handle
Section get_mem_section(size_t handle_id){
    return registry[handle_id];
}

// allows for modification of the value
void* lock_section(size_t handle) {
    if (handle >= REGISTRY_SIZE) {
        printf("[DEBUG] lock_section: invalid handle %zu\n", handle);
        return NULL;
    }

    Section* section = &registry[handle];  
    section->isInUse = true;

    printf("[DEBUG] lock_section: handle=%zu, addr=%p, isInUse=%d\n",
           handle, section->start, section->isInUse);

    return section->start;
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

bool is_not_occupied(void* addr) {
    for (int i = 0; i < REGISTRY_SIZE; i++) {
        if (registry[i].isInUse) {
            if ((uint8_t*)addr >= (uint8_t*)registry[i].start &&
                (uint8_t*)addr <  (uint8_t*)registry[i].start + registry[i].size) {
                return false; 
            }
        }
    }
    return true;
}

// returns the address to an unused section
// if none found, return null
Section* get_unused_section(){
    printf("Trying to get unused section\n");
    for(size_t i = 0; i < REGISTRY_SIZE; i++){
        printf("[DEBUG] Section %zu: isInUse=%d, start=%p, handle=%zu\n", i ,registry[i].isInUse, registry[i].start, registry[i].handle);
        if (!registry[i].isInUse)
        {
            // issue here
            printf("Found unused section at i = %zu \n", i);
            registry[i].handle = i;
            return &registry[i];
        }
    }
    return NULL;
}

void free_section(size_t handle){
    registry[handle].isInUse = false;
    registry[handle].isLocked = false;
}