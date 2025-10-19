#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "../alloc_handler.h"

enum TestStatus{
    Passed = true,
    Failed = false
};



bool alloc_free_tests(){
    printf(" - Free and alloc tests \n");
    //alloc 1 byte
    uint8_t handle1 = alloc(1);
    uint8_t* addr1 = (uint8_t*) get_addr(handle1);
    // alloc 1 byte
    uint8_t handle2 = alloc(1);
    uint8_t* addr2 = (uint8_t*) get_addr(2);
    // free byte 1 causing fragmentation
    free_mem(handle1);

    // handle1 is expected to be no in use because of free
    if (get_section(handle1)->isInUse == true)
    {
        printf("Error in freeing, handle is valid when its supposed to be invalid! - X \n");
        return false;
    }

    // alloc 2 bytes
    // these will be after the handle 2 
    uint8_t handle3 = alloc(2);
    uint8_t* addr3 = (uint8_t*) get_addr(handle3);

    // its expected that addr 1 and 3 are diferent because 3 cant fit in 1
    if (addr1 == addr3)
    {
        printf("Error in allocation, allocated 2 bytes in a 1 byte space - X \n");
        printf("handle1 %p == handle3 %p \n", addr1, addr3);
        return false;
    }

    // the handles contrary to the addresses should be the same since they are size independent and should be reused
    if (handle1 != handle3)
    {
        printf("Error in section reuse, failed to reuse unused section - X \n");
        printf("handle1 %d == handle3 %d \n", handle1, handle3);
        return false;
    }
    
    
    // alloc 1 byte
    // this one is supposed to fill in the handle 1 gap
    uint8_t handle4 = alloc(1);
    uint8_t* addr4 = (uint8_t*) get_addr(handle4);

    // its expected that addr 1 and 4 are equal because both are 1 byte
    if (addr1 != addr4)
    {
        printf("Error allocating, 1 byte alloc did not get put into 1 byte gap - X \n");
        printf("handle1 %d != handle4 %d \n", handle1, handle4);
        return false;
    }
    // handle 1 and 4 are expected to be diferent however because 1 has been replaced by 3, 
    // this test in not necessary because we already know that 1 = 3 by this point

    printf("Free and alloc tests passed! \n");
    return true;
}

bool pack_tests(){
    printf(" - Starting pack tests \n");

    uint8_t handle1 = alloc(1);
    uint8_t* addr1 = get_addr(handle1);
    *addr1 = 1;

    uint8_t handle2 = alloc(1);
    uint8_t* addr2 = get_addr(handle2);
    *addr2 = 2;

    uint8_t handle3 = alloc(1);
    uint8_t* addr3 = get_addr(handle3);
    *addr3 = 3;

    //print_heap_func();

    free_mem(handle1);
    lock(handle3);

    pack();

    uint8_t* new_addr1 = get_addr(handle1);
    uint8_t* new_addr2 = get_addr(handle2);
    uint8_t* new_addr_3 = get_addr(handle3);

    // addr 3 should remain the same since its locked
    if (addr3 != new_addr_3)
    {
        printf("Error packing, packed locked memory - X \n");
        return false;
    }

    // the new addr 2 should take the place of the old addr 1 due to free
    if (new_addr2 != addr1)
    {
        printf("Error packing, failed to override free memory - X \n");
        printf("new_addr2 %p != addr1 %p \n", new_addr2, addr1);
        return false;
    }

    

    printf("Pack tests passed! \n");
    return true;
}

bool basic_alloc_tests(){
    printf(" - Starting basic alloc tests\n");

    uint8_t handle1 = alloc(1);
    uint8_t* addr1 = get_addr(handle1);
    *addr1 = 1;

    uint8_t handle2 = alloc(1);
    uint8_t* addr2 = get_addr(handle2);
    *addr2 = 2;

    uint8_t handle3 = alloc(1);
    uint8_t* addr3 = get_addr(handle3);
    *addr3 = 3;

    uint8_t handle4 = alloc(1);
    uint8_t* addr4 = get_addr(handle4);
    *addr4 = 4;

    if (addr2 != addr1 + 1 || addr3 != addr2 + 1 || addr4 != addr3 + 1 )
    {
        printf("Error allocating, failed to allocate continuous memory! - X \n");
        printf("addr1 %p, addr2 %p, addr3 %p, addr4 %p \n", addr1, addr2, addr3, addr4);
        print_heap_func();
        return false;
    }

    printf("Basic alloc tests passed! \n");
    return true;
}

void start_all_tests(){
    printf("Starting tests \n");

    basic_alloc_tests();
    clear_registry();

    alloc_free_tests();  
    clear_registry();

    pack_tests();
    clear_registry();
    
    print_registry_func();

    print_heap_func();
}