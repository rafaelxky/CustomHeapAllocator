#!/bin/bash

rm allocator
gcc main.c alloc_handler.c heap.c registry.c tests/customHeapTests.c -o allocator
./allocator