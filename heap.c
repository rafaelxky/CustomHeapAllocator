#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "section.h"
#include "registry.h"
#include <stdlib.h>

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

void* swipe_alloc_sections(size_t size){
    printf("Swipe allocating with sections \n");
    int ptr = 0;
    while (ptr < HEAP_SIZE)
    {
        int  not_occupied_counter = 0;
        while (not_occupied_counter < size){ 
            if (is_not_occupied(&heap[ptr]))
            {
                //printf("This section is not occupied - %d \n", ptr);
                not_occupied_counter++;
                ptr++;
                continue;
            }
            not_occupied_counter = 0;
            ptr++;
               
        }
        printf("Allocated %d memory spaces! \n", size);
        printf("Returning heap address %p \n", (void*)&heap[ptr-size]);
        return &heap[ptr - size];
    } 

    fprintf(stderr, "Error: heap overflow, cannot allocate any more memory in heap!");
    abort();
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