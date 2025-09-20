#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "section.h"
#include "registry.h"

#define HEAP_SIZE 1024 

static uint8_t heap[HEAP_SIZE];
static size_t heap_offset = 0;

void* bump_malloc(size_t size) {
    size = (size + 7) & ~7;

    if (heap_offset + size > HEAP_SIZE) {
        return NULL;
    }

    void* ptr = &heap[heap_offset];
    heap_offset += size;
    return ptr;
}

// todo: integrate with the sections so theres no intersection
// look for section of 0's with size, size
void* swipe_alloc(size_t size){
    printf("Swipe allocating");
    size = (size + 7) & ~7;
    int ptr = 0;
    int tries = 0;
    // check for continuous 0's
    while (true)
    {
        int continuous_zero = 0;
        while (heap[ptr] == 0){
            ptr ++;
            continuous_zero++;
            if (continuous_zero == size)
            {
                printf("Success swipe allocation \n");
                printf("Found free at %d \n", ptr);
                printf("After %d tries \n", tries);
                return &heap[ptr - size];
            }
           
        } 
        tries ++;
        if (tries > 100)
        {
            printf("Could not swipe allocate \n");
            return NULL;
        }
    }
}

void* swipe_alloc_sections(size_t size){
    printf("Swipe allocating with sections \n");
    int ptr = 0;
    int tries = 0;
    // check for continuous 0's
    while (ptr < HEAP_SIZE)
    {
        int  not_occupied_counter = 0;
        while (not_occupied_counter < size){ 
            if (is_not_occupied(&heap[ptr]))
            {
                printf("This section is not occupied - %d \n", ptr);
                not_occupied_counter++;
                ptr++;
                continue;
            }
            not_occupied_counter = 0;
            ptr++;
            tries ++;
            if (tries > 100)
            {
                printf("Could not swipe allocate \n");
                return NULL;
            }
        }
        return &heap[ptr - size];
    }
}

// sets a section to zeros
void zero(size_t start, size_t size){
    size = (size + 7) & ~7;
    for (int i = start; start <= start + size; start++){
        heap[i] = 0;
    }
}

void bump_reset() {
    heap_offset = 0;
}