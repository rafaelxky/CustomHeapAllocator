#ifndef ALLOC_HANDLER_H
#define ALLOC_HANDLER_H

#include <stdbool.h>
#include <stddef.h>
#include "section.h"

void print_heap_func();
void print_registry_func();

size_t alloc(size_t size);
void* lock(size_t handle);
int unlock(size_t handle);
void* get_addr(size_t handle);
void free_mem(size_t handle);
void pack();
Section* get_section(size_t handle);
void clear_registry();

#endif