// memory_manager.c

#include "memory_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

MemoryPool memory_pool = {NULL, 0, 0, 0, 0};

static pthread_mutex_t memory_pool_lock = PTHREAD_MUTEX_INITIALIZER;

void init_memory_pool(size_t size) {
    pthread_mutex_lock(&memory_pool_lock);

    memory_pool.free_list = malloc(sizeof(MemoryBlock) + size);
    if (memory_pool.free_list == NULL) {
        printf("Memory allocation failed during pool initialization.\n");
        pthread_mutex_unlock(&memory_pool_lock);
        return;
    }

    memory_pool.free_list->size = size;
    memory_pool.free_list->next = NULL;
    memory_pool.free_list->is_free = true;

    memory_pool.total_allocated = size;
    memory_pool.current_memory_usage = 0;
    memory_pool.max_memory_usage = size;

    printf("Memory pool initialized with size %zu bytes.\n", size);

    pthread_mutex_unlock(&memory_pool_lock);
}


void* alloc_memory(size_t size) {
    pthread_mutex_lock(&memory_pool_lock);

    MemoryBlock* current = memory_pool.free_list;
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            current->is_free = false;
            memory_pool.current_memory_usage += current->size;

            if (memory_pool.current_memory_usage > memory_pool.max_memory_usage) {
                memory_pool.max_memory_usage = memory_pool.current_memory_usage;
            }

            printf("Allocated %zu bytes from memory pool.\n", size);
            pthread_mutex_unlock(&memory_pool_lock);
            return (void*)(current + 1);  // Return the memory pointer
        }
        current = current->next;
    }

    printf("Expanding memory pool...\n");
    size_t new_pool_size = memory_pool.total_allocated * 2;
    init_memory_pool(new_pool_size);

    return alloc_memory(size);
}



void free_memory(void* ptr) {
    if (ptr == NULL) return;

    MemoryBlock* block = (MemoryBlock*)ptr - 1;  // Adjust for the MemoryBlock header
    block->is_free = true;
    memory_pool.current_memory_usage -= block->size;
    memory_pool.total_freed += block->size;


    MemoryBlock* next = block->next;
    if (next != NULL && next->is_free) {
        block->size += next->size;
        block->next = next->next;
    }

    MemoryBlock* current = memory_pool.free_list;
    while (current != NULL) {
        if (current->next == block && current->is_free) {
            current->size += block->size;
            current->next = block->next;
            break;
        }
        current = current->next;
    }

    block->next = memory_pool.free_list;
    memory_pool.free_list = block;
}


void print_memory_statistics(void) {
    printf("Total Allocated: %zu bytes\n", memory_pool.total_allocated);
    printf("Total Freed: %zu bytes\n", memory_pool.total_freed);
    printf("Current Memory Usage: %zu bytes\n", memory_pool.current_memory_usage);
    printf("Max Memory Usage: %zu bytes\n", memory_pool.max_memory_usage);
}

void* realloc_memory(void* ptr, size_t size) {
    pthread_mutex_lock(&memory_pool_lock);

    if (ptr == NULL) {
        printf("Attempted to realloc a NULL pointer.\n");
        pthread_mutex_unlock(&memory_pool_lock);
        return NULL;
    }

    MemoryBlock* block = (MemoryBlock*)ptr - 1;
    void* new_ptr = malloc(size);
    if (new_ptr != NULL) {
        memcpy(new_ptr, ptr, block->size);
        free_memory(ptr);
    }

    pthread_mutex_unlock(&memory_pool_lock);
    return new_ptr;
}

void print_memory_pool() {
    printf("Memory Pool Status:\n");
    MemoryBlock* current = memory_pool.free_list;
    while (current != NULL) {
        printf("Block size: %zu bytes, Free: %s\n", current->size, current->is_free ? "Yes" : "No");
        current = current->next;
    }
}
