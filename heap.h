
#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>

void* swipe_alloc_sections(size_t size);
void pack_mem();
void print_heap();

#endif