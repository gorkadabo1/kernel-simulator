#include <stdio.h>
#include "structs.h"
#include "commons.h"

void initPageTable(PageTable *pt, int numEntries, uint32_t *base) {
    pt->entries = (PageTableEntry *)base;
    pt->numEntries = numEntries;
    pt->occupied = false;
    int i;

    for (i = 0; i < numEntries; i++) {
        pt->entries[i].physicalAddr = 0;
        pt->entries[i].valid = false;
    }
}

void initPageDirectory(int numTables, uint32_t *kernelBase) {
    int i;
    pageDirectory.tables = malloc(numTables * sizeof(PageTable));
    pageDirectory.numTables = numTables;
    uint32_t *currentBase = kernelBase;

    for (i = 0; i < numTables; i++) {
        initPageTable(&(pageDirectory.tables[i]), PAGE_ENTRIES, currentBase);
        currentBase += PAGE_ENTRIES;
    }
}

uint64_t* initPageTableForProcess(size_t totalEntries, uint32_t* programSegment) {

    size_t firstFreeTable = -1, firstFreeEntry = -1;
    size_t currentFreeCount = 0;

    size_t tableIndex, entryIndex, i;

    for (tableIndex = 0; tableIndex < pageDirectory.numTables; tableIndex++) {
        currentFreeCount = 0;
        PageTable *table = &pageDirectory.tables[tableIndex];

        if(table->occupied) {
            continue;
        }

        for (entryIndex = 0; entryIndex < table->numEntries; entryIndex++) {
            if (!table->entries[entryIndex].valid) {
                if (firstFreeEntry == -1) {
                    firstFreeTable = tableIndex;
                    firstFreeEntry = entryIndex;
                }
                currentFreeCount++;
                if (currentFreeCount == totalEntries) {
                    break;
                }
            } else {
                firstFreeEntry = -1;
            }
        }
        if (currentFreeCount == totalEntries) {
            break;
        }
    }

    if (firstFreeEntry == -1 || currentFreeCount < totalEntries) {
        printf("No hay espacio suficiente en las tablas de páginas del kernel.\n");
        return NULL;
    }

    pageDirectory.tables[firstFreeTable].occupied = true;

    for (i = 0; i < totalEntries; i++) {
        pageDirectory.tables[firstFreeTable].entries[firstFreeEntry + i].physicalAddr = programSegment + i * PAGE_ENTRIES;
        pageDirectory.tables[firstFreeTable].entries[firstFreeEntry + i].valid = true;
        //printf("Asignando pagina %d en la tabla %d, entrada %d\n", i, firstFreeTable, firstFreeEntry + i);
    }

    return (uint64_t*)(physical_memory.kernel_base + firstFreeTable * PAGE_ENTRIES);
}
