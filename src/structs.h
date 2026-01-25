#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define TLB_SIZE 64

typedef struct {
    uint32_t *base;
    uint32_t *kernel_base;
} PhysicalMemory;

typedef struct {
    int intervalo_timer;
    int intervalo_scheduler;
} Configuracion;

typedef struct {
    uint64_t *code;
    uint64_t *data;
    uint64_t *pgb;
} MemoryManagement;

typedef struct {
    int pid;
    int t_ex;
    int res_time;
    MemoryManagement mm;
} PCB;

typedef struct {
    PCB *procesos;
    int capacidad;
    int kont;
} ProcessQueue;

typedef struct {
    int indice;
    int quantum;
} EstadoScheduler;

typedef struct {
    uint64_t virtual;
    uint64_t physical;
    bool valid;
} TLBEntrada;

typedef struct {
    TLBEntrada entries[TLB_SIZE];
} TLB;

typedef struct {
    TLB tlb;
    int tlbIndex;
} MMU;

typedef struct {
    uint64_t PC;
    uint32_t IR;
    uint64_t  *PTBR;
    uint32_t  registers[16];
    MMU mmu;
} HardwareThread;

typedef struct {
    HardwareThread *threads;
    int numThreads;
} Core;

typedef struct {
    Core *cores;
    int coreNum;
} CPU;

typedef struct {
    CPU *cpus;
    int numCPU;
} Maquina;

typedef struct {
    uint32_t *physicalAddr;
    bool valid;
} PageTableEntry;

typedef struct {
    PageTableEntry* entries;
    int numEntries;
    bool occupied;
} PageTable;

typedef struct {
    PageTable *tables;
    int numTables;
} PageDirectory;

typedef struct {
    HardwareThread *thread;
    PCB pcb;
    int threadIndex;
} ExecutionArgs;

#endif
