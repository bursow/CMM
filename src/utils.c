// utils.c

#include "memory_manager.h"
#include <stdio.h>

// Örneğin, bellek bloklarını yazdıran bir fonksiyon
void print_memory_blocks() {
    MemoryBlock* current = memory_pool.free_list;
    while (current != NULL) {
        printf("Block size: %zu, Free: %d\n", current->size, current->is_free);
        current = current->next;
    }
}

