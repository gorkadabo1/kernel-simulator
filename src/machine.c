#include "machine.h"
#include "commons.h"
#include "mmu.h"
#include <stdio.h>

int *hardwareThreadStatus;

void* iniciarMaquina(int numCPU, int coreNum, int numThreads, int programsPerThread, int extraPrograms) {
    int i, j, k, p, fileIndex = 0;
    machine.numCPU = numCPU;
    machine.cpus = malloc(numCPU * sizeof(CPU));
    int index;

    hardwareThreadStatus = calloc(totalThreads, sizeof(int));

    for (i = 0; i < numCPU; i++) {
        machine.cpus[i].coreNum = coreNum;
        machine.cpus[i].cores = malloc(coreNum * sizeof(Core));

        for (j = 0; j < coreNum; j++) {
            machine.cpus[i].cores[j].numThreads = numThreads;
            machine.cpus[i].cores[j].threads = malloc(numThreads * sizeof(HardwareThread));

            for (k = 0; k < numThreads; k++) {
                machine.cpus[i].cores[j].threads[k].PC = 0;
                machine.cpus[i].cores[j].threads[k].IR = 0;
                machine.cpus[i].cores[j].threads[k].PTBR = 0;
                initMMU(&machine.cpus[i].cores[j].threads[k].mmu);
            }
        }
    }
    return NULL;
}
