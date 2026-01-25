#include <stdio.h>
#include "commons.h"
#include "page_table.h"

PhysicalMemory physical_memory;

void initPhysicalMemory() {

    physical_memory.base = (uint32_t *)malloc(TOTAL_MEMORY_ENTRIES * sizeof(uint32_t));

    if (physical_memory.base == NULL) {
        perror("Failed trying to allocate physical memory");
        exit(1);
    }

    memset(physical_memory.base, 0, TOTAL_MEMORY_ENTRIES * sizeof(uint32_t));

    size_t kernelOffset = 2 * TOTAL_MEMORY_ENTRIES / 3;
    physical_memory.kernel_base = physical_memory.base + kernelOffset;

    int numPages = (TOTAL_MEMORY_ENTRIES * sizeof (uint32_t) / 3) / PAGE_SIZE_BYTES;
    initPageDirectory(numPages, physical_memory.kernel_base);
}

void freePhysicalMemory() {
    free(physical_memory.base);
    physical_memory.base = NULL;
    physical_memory.kernel_base = NULL;
}