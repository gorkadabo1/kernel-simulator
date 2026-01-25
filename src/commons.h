#ifndef COMMONS_H
#define COMMONS_H

#include <pthread.h>
#include "structs.h"

#define TOTAL_MEMORY_ENTRIES (1 << 24)
#define PAGE_ENTRIES 256
#define PAGE_SIZE_BYTES (PAGE_ENTRIES * sizeof(uint32_t))

//Los sigueintes valores deben modificarse de acuerdo al tamaño de las páginas
#define PAGE_MASK  0x3FF // 10 bits
#define PAGE_SHIFT  10 // 10 bits

extern int nextPID;
extern pthread_mutex_t queueMutex;

extern pthread_mutex_t lock;
extern pthread_cond_t beat;
extern pthread_cond_t tick_cond;

extern Configuracion config;
extern ProcessQueue process_queue;
extern EstadoScheduler estado_scheduler;
extern Maquina machine;
extern PhysicalMemory physical_memory;
extern PageDirectory pageDirectory;

extern int coreNum;
extern int numThreads;
extern int numCPU;


extern int totalThreads;
extern int *hardwareThreadStatus;

#endif
