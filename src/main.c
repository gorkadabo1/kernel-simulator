#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "clock_timer.h"
#include "structs.h"
#include "process.h"
#include "scheduler_dispatcher.h"
#include "utilities.h"
#include "machine.h"
#include "physical_memory.h"

Configuracion config;
ProcessQueue process_queue;
EstadoScheduler estado_scheduler;
Maquina machine;
PageDirectory pageDirectory;

pthread_cond_t beat;
pthread_cond_t final_beat;
pthread_mutex_t lock;


int totalThreads;
int coreNum;
int numThreads;
int numCPU;

int main(int argc, char *argv[]) {

    if (argc < 7) {
        printf("Uso: %s <intervalo_timer> <intervalo_scheduler> <quantum> <numCPU> <coreNum> <numThreads>\n", argv[0]);
        return 1;
    }

    int elfFileCount = countElfFiles("elf");
    printf("Number of .elf files: %d\n", elfFileCount);
    config.intervalo_timer = atoi(argv[1]);
    config.intervalo_scheduler = atoi(argv[2]);
    int quantum = atoi(argv[3]);
    numCPU = atoi(argv[4]);
    coreNum = atoi(argv[5]);
    numThreads = atoi(argv[6]);

    totalThreads = numCPU * coreNum * numThreads;
    int programsPerThread = elfFileCount / totalThreads;
    int extraPrograms = elfFileCount % totalThreads;

    pthread_t clock_thread, timer_thread, scheduler_dispatcher_thread;

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&beat, NULL);
    pthread_mutex_init(&queueMutex, NULL);

    iniProcessQueue(elfFileCount);
    initPhysicalMemory();
    initializeUserPages();
    iniciarMaquina(numCPU, coreNum, numThreads, programsPerThread, extraPrograms);
    loader(elfFileCount);

    estado_scheduler.indice = 0;
    estado_scheduler.quantum = quantum;

    pthread_create(&clock_thread, NULL, clock_function, NULL);
    pthread_create(&timer_thread, NULL, timer_function, NULL);
    pthread_create(&scheduler_dispatcher_thread, NULL, scheduler_dispatcher, NULL);

    pthread_join(scheduler_dispatcher_thread, NULL);
    pthread_join(clock_thread, NULL);
	printf("CLOCK ENDED\n");
    pthread_join(timer_thread, NULL);
	printf("TIMER ENDED\n");

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&beat);
    pthread_mutex_destroy(&queueMutex);

    freePhysicalMemory();

    return 0;
}
