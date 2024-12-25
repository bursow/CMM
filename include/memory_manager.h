// memory_manager.h

#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stddef.h>
#include <stdbool.h>

typedef struct MemoryBlock {
    size_t size;
    struct MemoryBlock* next;
    bool is_free;
} MemoryBlock;

typedef struct {
    MemoryBlock* free_list;
    size_t total_allocated;
    size_t total_freed;
    size_t current_memory_usage;
    size_t max_memory_usage;
} MemoryPool;

extern MemoryPool memory_pool;


void init_memory_pool(size_t size);

void* alloc_memory(size_t size);

void free_memory(void* ptr);

void print_memory_statistics(void);

void print_memory_pool(void);

void* realloc_memory(void* ptr, size_t size);

#endif // MEMORY_MANAGER_H
