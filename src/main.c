// main.c

#include "memory_manager.h"
#include <stdio.h>

int main() {
    // Initialize memory pool with 2048 bytes
    init_memory_pool(2048);

    // Print the memory pool status
    print_memory_pool();

    // Try allocating memory
    void* ptr1 = alloc_memory(256);
    if (ptr1 != NULL) {
        printf("Allocated 256 bytes from memory pool.\n");
    }

    // Print the memory pool status after allocation
    print_memory_pool();

    // Try allocating memory again
    void* ptr2 = alloc_memory(128);
    if (ptr2 != NULL) {
        printf("Allocated 128 bytes from memory pool.\n");
    }

    // Print the memory pool status again
    print_memory_pool();

    return 0;
}
