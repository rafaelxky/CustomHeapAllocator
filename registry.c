#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "section.h"
#include <stdio.h>
#include <assert.h>
#include "registry.h"

Section registry[REGISTRY_SIZE] = {0};
size_t offset = 0;

size_t register_mem(Section section) {
    section.handle = offset;
    registry[offset] = section;
    offset += 1;
    return section.handle;
}

Section* get_mem_section(size_t handle){
    if (handle >= REGISTRY_SIZE)
    {
        fprintf(stderr, "Error: Cannot use a handle larger than the REGISTY_SIZE to get: handle %zu \n", handle);
        abort();
    }
    Section* section = &registry[handle];
    
    if (!section->isInUse)
    {
        fprintf(stderr, "Error: cannot get a section that is not in use!\n");
        fprintf(stderr, "lock_section: handle=%zu \n",section->handle);
        abort();
    }

    return section;
}

// allows for modification of the value
void* lock_section(size_t handle) {
    if (handle >= REGISTRY_SIZE) {
        fprintf(stderr, "Error: Cannot use a handle larger than the REGISTY_SIZE to lock: handle %zu \n", handle);
        abort();
    }
    
    Section* section = &registry[handle];

    if (!section->isInUse)
    {
        fprintf(stderr, "Error: cannot lock a section that is not in use!\n");
        fprintf(stderr, "lock_section: handle=%zu \n",section->handle);
        abort();
    }

    section->isLocked = true;
    return section->start;
}

void* get_section_addr(size_t handle){
    Section* section = &registry[handle];
    if (handle >= REGISTRY_SIZE)
    {
        fprintf(stderr, "Error: Cannot use a handle larger than the REGISTY_SIZE to get: handle %zu \n", handle);
        abort();
    }
    
    return section->start;
}

// lock the value so the program may clean it
void unlock_section(size_t handle){
    if (handle >= REGISTRY_SIZE)
    {
        fprintf(stderr, "Error: Cannot use a handle larger than the REGISTY_SIZE to unlock: handle %zu \n", handle);
        abort();
    }

    Section* section = &registry[handle];

    if (!section->isInUse)
    {
        fprintf(stderr, "Error: cannot unlock a section that is not in use!\n");
        fprintf(stderr, "lock_section: handle=%zu \n",section->handle);
        abort();
    }
    
    section->isLocked = false;
}

bool is_not_occupied(void* addr) {
    for (size_t i = 0; i < REGISTRY_SIZE; i++) {
        if (registry[i].isInUse) {
            if ((uint8_t*)addr >= (uint8_t*)registry[i].start &&
                (uint8_t*)addr <  (uint8_t*)registry[i].start + registry[i].size) {
                return false; 
            }
        }
    }
    return true;
}

Section* get_unused_section(){

    for(size_t i = 0; i < REGISTRY_SIZE; i++){

        if (!registry[i].isInUse)
        {
            registry[i].handle = i;
            return &registry[i];
        }
    }
    fprintf(stderr, "Error: registry overflow, no free sections available to use! \n");
    abort();
}

void free_section(size_t handle){
    if (handle >= REGISTRY_SIZE)
    {
        fprintf(stderr, "Error: Cannot use a handle larger than the REGISTY_SIZE to free: handle %zu \n", handle);
        abort();
    }
    
    Section* section = &registry[handle];
    if (!section->isInUse)
    {
        fprintf(stderr, "Error: cannot free a section that is not in use!\n");
        fprintf(stderr, "lock_section: handle=%zu \n",section->handle);
        abort();
    }
    
    registry[handle].isInUse = false;
    registry[handle].isLocked = false;
}

void print_registry(){
    for(size_t i = 0; i < REGISTRY_SIZE; i++){
        if (registry[i].isInUse)
        {
            printf("[DEBUG] Section %zu: isInUse=%d, start=%p, handle=%zu, size=%d, isLocked=%d, value=%d \n", i ,registry[i].isInUse, registry[i].start, registry[i].handle, registry[i].size, registry[i].isLocked, *registry[i].start );
        }
    }
    printf("\n");
}