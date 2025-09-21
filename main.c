#include <stdint.h>
#include <stdio.h>
#include "alloc_handler.h"

// todo: implement GC
// implement free

int main(){
    printf("Started\n");
    // get the handler
    struct AllocHandler* allocHandler = get_alloc_handler();
    // get the pointer / handle
    size_t num_handle_1 = allocHandler->alloc(10);
    size_t num_handle_2 = allocHandler->alloc(20);
    // get the actual address
    int* addr1 = (int*)allocHandler->lock(num_handle_1);
    int* addr2 = (int*)allocHandler->lock(num_handle_2);
    *addr1 = 1;
    *addr2 = 2;

    printf("value - %d\n", *addr1);
    printf("address of addr1- %p\n", (void*)addr1);
    printf("handle - %zu\n", num_handle_1);
    printf("value - %d\n", *addr2);
    printf("address of addr2- %p\n", (void*)addr2);
    printf("handle - %zu\n", num_handle_2);

    // free 1
    allocHandler->free(num_handle_1);
    // allocate 10b
    size_t num_handle_3 = allocHandler->alloc(10);
    int* addr3 = (int*)allocHandler->lock(num_handle_3);
    *addr3 = 3; 
    allocHandler->unlock(num_handle_3);

    printf("value - %d\n", *addr3);
    printf("address for addr3 - %p\n", (void*)addr3);
    printf("handle - %zu\n", num_handle_3);

    // free 3
    allocHandler->free(num_handle_3);
    size_t handle_4 = allocHandler->alloc(20);
    int* addr4 = (int*)allocHandler->lock(handle_4);
    *addr4 = 4;

    printf("value4 = %d \n", *addr4);
    printf("address for addr4 - %p\n", (void*)addr4);
    printf("handle4 - %zu\n", handle_4);

    printf("Expected: \n");
    printf("Addr1 = Addr3 \n");
    printf("Handle1 = Handle3 = Handle4 \n");
    printf("Fragmentation \n");

    return 0;
}