#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "section.h"
#include "registry.h"
#include <stdlib.h>

#define HEAP_SIZE 1024 
static uint8_t heap[HEAP_SIZE];

void print_heap(){
    for (int i = 0; i < HEAP_SIZE; i++){
        printf("%d", heap[i]);
    }
    printf("\n");
}

void* swipe_alloc_sections(size_t size){
    size_t ptr = 0;
    while (ptr + size <= HEAP_SIZE)
    {
        size_t not_occupied_counter = 0;
        while (not_occupied_counter < size){ 
            if (is_not_occupied(&heap[ptr]))
            {
                not_occupied_counter++;
                ptr++;
            } else {
                ptr = ptr - not_occupied_counter + 1;
                not_occupied_counter = 0;
            }
               
        }
        return &heap[ptr - size];
    } 

    fprintf(stderr, "Error: heap overflow, cannot allocate any more memory in heap!");
    abort();
}

void zero(void* start, size_t size){
    uint8_t* ptr = (uint8_t*)start;
    for (size_t i = 0; i < size; i++){
        ptr[i] = 0;
    }
}

void pack_mem(){
    printf("heap0 = %p \n", &heap[0]);
    for(size_t i = 0; i < REGISTRY_SIZE; i++){
        if (registry[i].isLocked || registry[i].size == 0 || !registry[i].isInUse || registry[i].start == &heap[0]){
            if (registry[i].start != NULL)
            {
                printf("Locked, size 0, heap 0 or not in use - %d \n", *registry[i].start);
            }
            
            continue;
        }

        uint8_t* free_addr = registry[i].start - 1;
        printf("start free_adrr = %p \n", free_addr);
        uint8_t* new_start;

        while (free_addr >= &heap[0])
        {
            if (is_not_occupied(free_addr) && free_addr != &heap[0])
            {
                printf("Not occupied %p \n", free_addr);
                free_addr -= 1;
                continue;
            }
            printf("Occupied %p \n", free_addr);
            printf("value %d \n", *free_addr);
            
            size_t offset_counter = 0;
            uint8_t* curr_addr = registry[i].start;
            new_start = free_addr;
            while (offset_counter != registry[i].size)
            {
                printf("Moving %p to %p \n", curr_addr, free_addr);
                printf("Moving %d to %d \n", *curr_addr, *free_addr);
                *free_addr = *curr_addr;
                free_addr++;
                curr_addr++;
                offset_counter++;
            }
            break;
        }
        registry[i].start = new_start;
    }
}