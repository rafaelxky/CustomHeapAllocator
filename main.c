#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "alloc_handler.h"

// helper to check memory at an offset
void check_int_value(size_t handle, int expected) {
    int* ptr = (int*)get_addr(handle);
    if(ptr != NULL){
        printf("ptr is not null \n");
    } else {
        printf("ptr is null \n");
    };

    if (*ptr == expected){
        printf("ptr value is expected %d == %d \n", *ptr, expected);
    } else {
        printf("ptr value is not expected %d != %d \n",*ptr, expected);
    };
}

int main() {
    // Allocate 10 and 20 bytes
    size_t num_handle_1 = alloc(10);
    size_t num_handle_2 = alloc(20);
    int* addr1 = (int*)lock(num_handle_1);
    int* addr2 = (int*)lock(num_handle_2);
    *addr1 = 1;
    *addr2 = 2;

    // Free first allocation
    free_mem(num_handle_1);

    // Allocate 10 bytes → should reuse first freed space
    size_t num_handle_3 = alloc(10);
    int* addr3 = (int*)lock(num_handle_3);
    *addr3 = 3;
    free_mem(num_handle_3);

    // Allocate 20 → should move into freed space without overwriting 3
    size_t handle_4 = alloc(20);
    int* addr4 = (int*)lock(handle_4);
    *addr4 = 4;

    // Allocate 2 and 3 bytes → assign 8 and 9
    size_t ptr1 = alloc(2);
    size_t ptr2 = alloc(3);
    int* ptr11 = (int*)get_addr(ptr1);
    int* ptr22 = (int*)get_addr(ptr2);
    *ptr11 = 8;
    *ptr22 = 9;

    // Unlock 9 to be movable, free 8 to be overridden
    unlock(ptr2);
    unlock(handle_4);
    free_mem(ptr1);

    // Print heap before packing
    printf("Heap before packing:\n");
    print_heap_func();
    print_registry_func();

    // Pack memory
    pack();

    printf("\nHeap after packing:\n");
    print_heap_func();
    print_registry_func();

    // --- TESTS ---
    // 9 should now occupy the space previously held by 8
    check_int_value(ptr2, 9);

    // 4 should occupy the space previously held by 3
    check_int_value(handle_4, 4);

    // Freed blocks should not contain old values (optional)
    // e.g., ptr1 was freed, ptr0 was never allocated yet

    printf("\nAll tests passed!\n");
    return 0;
}
