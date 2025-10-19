#ifndef REGISTRY_H
#define REGISTY_H

#include <stdbool.h>
#include <stddef.h>
#include "section.h"

#define REGISTRY_SIZE 64
extern Section registry[REGISTRY_SIZE]; 
extern size_t offset;

size_t register_mem(Section section);
Section* get_mem_section(size_t handle_id);
void* lock_section(size_t handle);
void unlock_section(size_t handle);
void* get_section_addr(size_t handle);
bool is_not_occupied(void* addr);
Section* get_unused_section();
void free_section(size_t handle);
Section* get_registry_section(size_t handle);
void clear_registry_func();

void print_registry();

#endif