#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "commons.h"
#include "structs.h"
#include "page_table.h"
#include "utilities.h"

#define USER_SPACE_SIZE (2 * TOTAL_MEMORY_ENTRIES / 3)
#define TOTAL_USER_PAGES (USER_SPACE_SIZE * sizeof(uint32_t) / PAGE_ENTRIES)

int pageStatus[TOTAL_USER_PAGES];

void initializeUserPages() {
    memset(pageStatus, 0, sizeof(pageStatus));
}

uint32_t* getFreePhysicalMemoryInUserSpace(int pagesNeeded) {
    int i;
    int firstFreePage = -1;

    for (i = 0; i < TOTAL_USER_PAGES; i++) {
        if (pageStatus[i] == 0) {
            if (firstFreePage == -1) {
                firstFreePage = i;
            }
            if (i - firstFreePage + 1 == pagesNeeded) {
                break;
            }
        } else {
            firstFreePage = -1;
        }
    }

    if (firstFreePage == -1) {
        printf("No hay suficiente memoria disponible\n");
        return NULL;
    }

    for (i = firstFreePage; i < firstFreePage + pagesNeeded; i++) {
        pageStatus[i] = 1;
    }
    return (uint32_t*)(physical_memory.base + firstFreePage * PAGE_ENTRIES);
}

int nextPID = 1;
int getNextPID() {
    int pid = nextPID++;
    return pid;
}

void loader(int elfFileCount) {
    int index;
    for (index = 0; index < elfFileCount; index++) {
        char *filename = malloc(256 * sizeof(char));
	sprintf(filename, "elf/prog%03d.elf", index);

        FILE* file = fopen(filename, "r");
        if (!file) {
            perror("Failed to open the program file");
            continue;
        }

        PCB newPCB;
        newPCB.pid = getNextPID();

        uint64_t *textStart = malloc(sizeof(uint64_t));
        uint64_t *dataStart = malloc(sizeof(uint64_t));
        fscanf(file, ".text %lx\n", textStart);
        fscanf(file, ".data %lx\n", dataStart);

        size_t totalSize = 0;
        uint32_t buf;

	int result;
        while ((result = fscanf(file, "%x", &buf)) != EOF) {
	    if (result == 1) {
		totalSize++;
	    } else {
		printf ("ERROR LEYENDO");
	    }
        }

        fseek(file, 0, SEEK_SET);

        fscanf(file, ".text %lx\n", textStart);
        fscanf(file, ".data %lx\n", dataStart);

        int totalPagesNeeded = (totalSize * sizeof(uint32_t) + PAGE_ENTRIES - 1) / PAGE_ENTRIES;
        uint32_t* programSegment = getFreePhysicalMemoryInUserSpace(totalPagesNeeded);
        //printf("Program segment: %p\n", programSegment);

        if (programSegment == NULL) {
            printf("Error: No hay suficiente memoria disponible\n");
            fclose(file);
            free(filename);
            free(textStart);
            free(dataStart);
            return;
        }


	//printf("PID: %d\n", newPCB.pid);
        size_t i;
        for (i = 0; i < totalSize; i++) {
            if (fscanf(file, "%x", &buf) == 1) {
                memcpy(&programSegment[i], &buf, sizeof(uint32_t));
		//printf("Value = %08X (Dir: %p)\n", programSegment[i], &programSegment[i]);
            }
        }

        //printf("Program segment first entry: %x\n", programSegment[0]);

        newPCB.mm.code = textStart;
        newPCB.mm.data = dataStart;

        newPCB.mm.pgb = initPageTableForProcess(totalPagesNeeded, programSegment);
        //printf("Page table: %p\n", newPCB.mm.pgb);

        if(!enqueuePCB(newPCB)){
            printf("Error: No se pudo encolar el proceso\n");
            fclose(file);
            free(filename);
            free(textStart);
            free(dataStart);
            free(newPCB.mm.pgb);
            continue;
        }


        fclose(file);
        free(filename);
    }
}
