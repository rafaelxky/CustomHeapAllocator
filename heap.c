#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "section.h"
#include "registry.h"
#include <stdlib.h>

#define HEAP_SIZE 1024 
static uint8_t heap[HEAP_SIZE];

void print_heap(){
    printf("HEAP \n");
    for (int i = 0; i < HEAP_SIZE; i++){
        printf("%d", heap[i]);
    }
    printf("\n");
}

void* swipe_alloc_sections(size_t size){
    printf("Swipe allocating with sections \n");
    int ptr = 0;
    while (ptr < HEAP_SIZE)
    {
        int not_occupied_counter = 0;
        while (not_occupied_counter < size){ 
            if (is_not_occupied(&heap[ptr]))
            {
                printf("This section is not occupied - %d - %p\n", ptr, &heap[ptr]);
                not_occupied_counter++;
                ptr++;
                continue;
            }
            not_occupied_counter = 0;
            ptr++;
               
        }
        printf("Allocated %d memory spaces! \n", size);
        printf("Returning heap address %p \n", (void*)&heap[ptr - size]);
        return &heap[ptr - size];
    } 

    fprintf(stderr, "Error: heap overflow, cannot allocate any more memory in heap!");
    abort();
}

// sets a section to zeros
void zero(size_t start, size_t size){
    size = (size + 7) & ~7;
    for (int i = start; start <= start + size; i++){
        heap[i] = 0;
    }
}

void pack_mem(){
    printf("Packing memory \n");
    for(size_t i = 0; i < REGISTRY_SIZE; i++){
        //if locked skip
        if (registry[i].isLocked || registry[i].size == 0)
        {
           // printf("Found locked section \n");
            continue;
        }
        // if not locked, reorder
        printf("Found non locked section\n");
        uint8_t* free_addr = registry[i].start - 1;
        printf("Moving from address %p \n", free_addr);
        uint8_t* new_start;

        while (free_addr >= &heap[0])
        {
            printf("if heap != 0 while \n");
            // if addr is free continue to check 
            if (is_not_occupied(free_addr) && free_addr != &heap[0])
            {
                printf("Found free address, continuing \n");
                free_addr -= 1;
                continue;
            }
            // if not reorder
            free_addr++;
            printf("Moving data to address %p \n", free_addr);

            size_t offset_counter = 0;
            uint8_t* curr_addr = registry[i].start;
            new_start = free_addr;
            // curr addr and free addr will move as a window until offset_counter is == to size
            printf("Reordering data \n");
            while (offset_counter != registry[i].size)
            {
                printf("moving %d with addr %p to %d with addr %p \n",*free_addr, free_addr, *curr_addr, curr_addr);
                *free_addr = *curr_addr;
                free_addr++;
                curr_addr++;
                offset_counter++;
            }
            break;
        }
        printf("free addr >= &heap[0] \n");
        registry[i].start = new_start;
    }
    printf("Finished pack \n");
}