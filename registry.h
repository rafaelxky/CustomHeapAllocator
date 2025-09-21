#ifndef REGISTRY_H
#define REGISTY_H

#include <stdbool.h>
#include <stddef.h>
#include "section.h"

size_t register_mem(Section section);
Section* get_mem_section(size_t handle_id);
void* lock_section(size_t handle);
void unlock_section(size_t handle);
void* get_section_addr(size_t handle);
bool is_not_occupied(void* addr);
Section* get_unused_section();
void free_section(size_t handle);

#endif