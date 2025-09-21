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
    // handle 0
    size_t num_handle_1 = allocHandler->alloc(10);
    // handle 1
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

    // free handle 0
    allocHandler->free(num_handle_1);
    // allocate 10
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

    printf("Allocating for ptr1\n");
    size_t ptr1 = allocHandler->alloc(2);
    printf("Allocating for ptr2\n");
    size_t ptr2 = allocHandler->alloc(3);

    int* ptr11 = (int*)allocHandler->get(ptr1);
    int* ptr22 = (int*)allocHandler->get(ptr2);
    *ptr11 = 8;
    *ptr22 = 9;

    // locked section 
    allocHandler->unlock(ptr2);
    // free section 1 w/ value 8
    allocHandler->free(ptr1);
    allocHandler->unlock(0);

    // ptr1 mus be free and ptr2 must be unlocked 

    print_registry_func();
    print_heap_func();
    allocHandler->pack();

    // fix issue here were this overrides the value 9
    size_t ptr0 = allocHandler->alloc(1);
    int* ptr00 = (int*)allocHandler->get(ptr0);
    *ptr00 = 0;

    print_heap_func();



    return 0;
}