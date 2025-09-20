#include <stdint.h>
#include <stdio.h>
#include "alloc_handler.h"

// todo: implement GC
// implement free

int main(){
    printf("Started");
    // get the handler
    struct AllocHandler* allocHandler = get_alloc_handler();
    // get the pointer / handle
    size_t num_handle_1 = allocHandler->alloc(10);
    size_t num_handle_2 = allocHandler->alloc(20);
    // get the actual address
    void* addr1 = allocHandler->lock(num_handle_1);
    void* addr2 = allocHandler->lock(num_handle_2);
    int value1 = *(int*)addr1;
    int value2 = *(int*)addr2;
    value1 = 1;
    value2 = 2;

    printf("value - %d\n", value1);
    printf("address - %p\n", (void*)addr1);
    printf("value - %d\n", value2);
    printf("address - %p\n", (void*)addr2);

    return 0;
}