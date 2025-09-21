#ifndef ALLOC_HANDLER_H
#define ALLOC_HANDLER_H

#include <stdbool.h>
#include <stddef.h>

// front facing interface and abstraction from the registry and heap
// interacts with the registry

typedef struct AllocHandler {
    size_t (*alloc)(size_t size);
    void* (*lock)(size_t);
    int (*unlock)(size_t);
    void* (*get)(size_t);
    void (*free)(size_t); 
    void (*pack)();
} AllocHandler;

void print_heap_func();
void print_registry_func();

struct AllocHandler* get_alloc_handler();

#endif