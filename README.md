# Custom Allocator Demo

This project demonstrates a simple **custom memory allocator** with
support for:

-   Allocating memory blocks using `alloc(size_t size)`
-   Accessing memory through `lock(handle)` and `get_addr(handle)`
-   Freeing blocks with `free_mem(handle)`
-   Compacting the heap with `pack()` to remove fragmentation
-   Inspecting memory layout with `print_heap_func()`
-   Inspecting registered sections with `print_registry_func()`

------------------------------------------------------------------------

## Locks vs. Direct Access

There are two ways to work with allocated memory:

### 1. Using `lock(handle)`

-   Returns a pointer to the allocated block.\
-   The block becomes **locked**, meaning it cannot be moved during
    `pack()`.\
-   Use this when you need a stable pointer while working with the data.

Example:

``` c
int* addr = (int*)lock(h);
*addr = 42;
unlock(h); // allow it to be moved again
```

### 2. Using `get_addr(handle)`

-   Returns a pointer **without locking** the block.\
-   The block can be moved when `pack()` is called.\
-   After packing, the pointer may become invalid --- always refresh it
    with another `get_addr(handle)`.

Example:

``` c
int* ptr = (int*)get_addr(h);
*ptr = 99;
// After pack(), call get_addr again before reuse
```

------------------------------------------------------------------------

## Example Flow

The provided `main.c` shows how the allocator works:

``` c
// 1. Allocate memory
size_t h1 = alloc(10);
size_t h2 = alloc(20);

// 2. Use lock to assign values
int* addr1 = (int*)lock(h1);
int* addr2 = (int*)lock(h2);
*addr1 = 1;
*addr2 = 2;

// 3. Free and reuse memory
free_mem(h1);
size_t h3 = alloc(10);
int* addr3 = (int*)lock(h3);
*addr3 = 3;
free_mem(h3);

// 4. Allocate more and use get_addr
size_t h4 = alloc(20);
int* addr4 = (int*)lock(h4);
*addr4 = 4;

size_t p1 = alloc(2);
size_t p2 = alloc(3);
int* ptr1 = (int*)get_addr(p1);
int* ptr2 = (int*)get_addr(p2);
*ptr1 = 8;
*ptr2 = 9;

// 5. Unlock and free
unlock(p2);   // allow 9 to move
free_mem(p1); // free 8
unlock(h4);   // allow 4 to move

// 6. Print state and pack
print_heap_func();     // shows raw heap contents
print_registry_func(); // shows registered sections
pack();
print_heap_func();
print_registry_func();
```

------------------------------------------------------------------------

## Expected Behavior

-   **Locked blocks** stay fixed during `pack()`.\
-   **Unlocked blocks** are shifted down to close gaps.\
-   After packing:
    -   `9` moves into the freed space of `8`.
    -   `4` moves into the freed space of `3`.

------------------------------------------------------------------------

## Build & Run
just run the start.sh script