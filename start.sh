#!/bin/bash

gcc main.c alloc_handler.c heap.c registry.c -o allocator
./allocator