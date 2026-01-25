#include <pthread.h>
#include "commons.h"
#include <dirent.h>
#include <string.h>
#include "process.h"
#include "mmu.h"
#include <stdio.h>
#include "tlb.h"

int countElfFiles(const char* directory) {
    int fileCount = 0;
    DIR* dir = opendir(directory);
    if (dir == NULL) {
        perror("Failed to open directory");
        return -1;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".elf") != NULL) {
            fileCount++;
        }
    }
    closedir(dir);
    return fileCount;
}

void iniProcessQueue(int capacity) {
    process_queue.procesos = (PCB *)malloc(capacity * sizeof(PCB));
    process_queue.capacidad = capacity;
    process_queue.kont = 0;
}

pthread_mutex_t queueMutex;

bool enqueuePCB(PCB pcb) {
    if (process_queue.kont >= process_queue.capacidad) {
        pthread_mutex_unlock(&queueMutex);
        return false;
    }
    process_queue.procesos[process_queue.kont++] = pcb;
    return true;
}

PCB dequeuePCB() {
    PCB pcb = process_queue.procesos[0];
    int i;
    for (i = 0; i < process_queue.kont - 1; i++) {
        process_queue.procesos[i] = process_queue.procesos[i + 1];
    }
    process_queue.kont--;
    return pcb;
}

void iniciarEjecucion(HardwareThread *thread, PCB pcb, int threadIndex) {
    uint32_t virtualAddr = *pcb.mm.code;
    uint32_t opcode, rd, rf1, rf2, addr, data;
    uint64_t physicalAddr;

    thread->PC = translateAddress(thread, virtualAddr);

    int running = 1;
    while (running) {
        thread->IR = *((uint64_t *)thread->PC);
//        printf("PID: %d | IR = 0x%X\n", pcb.pid, thread->IR);

        opcode = ((thread->IR) >> 28) & 0xF;
        rd = ((thread->IR) >> 24) & 0xF;
        addr = (thread->IR) & 0xFFFFFF;


        switch (opcode) {
            case 0x0: // ld rd, addr

                physicalAddr = translateAddress(thread, addr);
                data = *((uint64_t *)physicalAddr);
                thread->registers[rd] = data;
                printf("PID: %d | ld r%d, 0x%06X -> r%d = 0x%X\n", pcb.pid, rd, addr, rd, data);
                break;

            case 0x1: // st rf, addr

                rf1 = ((thread->IR) >> 24) & 0xF;
                physicalAddr = translateAddress(thread, addr);
                *((uint64_t *)physicalAddr) = thread->registers[rf1];
                printf("PID: %d | st r%d, 0x%06X\n", pcb.pid, rf1, addr);
                break;

            case 0x2: // add rd, rf1, rf2

                rf1 = ((thread->IR) >> 20) & 0xF;
                rf2 = ((thread->IR) >> 16) & 0xF;
                thread->registers[rd] = thread->registers[rf1] + thread->registers[rf2];
                printf("PID: %d | add r%d, r%d, r%d -> r%d = 0x%X\n", pcb.pid, rd, rf1, rf2, rd, thread->registers[rd]);
                break;

            case 0xF: // exit
                initTLB(&thread->mmu.tlb);
                running = 0;
                break;
            default:
                printf("PID: %d | Opcode no reconocido: 0x%X\n", pcb.pid, opcode);
                break;
        }
	thread->PC += 4;
        virtualAddr += 4;
    }

    hardwareThreadStatus[threadIndex] = 0;
}
