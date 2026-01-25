#include <stdio.h>
#include "commons.h"
#include "utilities.h"
#include "structs.h"

void *threadedExecution(void *args);

void* scheduler_dispatcher() {
   int pulseCount = 0;
   int i;
   int currentThreadIndex = 0;

   while (1) {
       pthread_cond_wait(&tick_cond, &lock);
       pulseCount++;
       if (pulseCount == config.intervalo_scheduler) {
           if (process_queue.kont > 0) {

	       int assigned = 0;

	       for (i = 0; i < totalThreads; i++) {
           	  int idx = (currentThreadIndex + i) % totalThreads;

                   if (hardwareThreadStatus[idx] == 0) {
                       hardwareThreadStatus[idx] = 1;
                       PCB pcb = dequeuePCB();
                       int cpuIndex = idx / (coreNum * numThreads);
                       int coreIndex = (idx % (coreNum * numThreads)) / numThreads;
                       int threadIndex = idx % numThreads;

                       machine.cpus[cpuIndex].cores[coreIndex].threads[threadIndex].PTBR = pcb.mm.pgb;

		       ExecutionArgs *args = malloc(sizeof(ExecutionArgs));
                       if (args == NULL) {
                           fprintf(stderr, "Error al reservar memoria para los argumentos del hilo.\n");
                           continue;
                       }

                       args->thread = &machine.cpus[cpuIndex].cores[coreIndex].threads[threadIndex];
                       args->pcb = pcb;
                       args->threadIndex = i;

                       printf("PID %d encolado en CPU %d, Core %d, Thread %d, KONT: %d\n", pcb.pid, cpuIndex, coreIndex, threadIndex, process_queue.kont);

		       pthread_t exec_thread;
                       pthread_create(&exec_thread, NULL, threadedExecution, args);
                       pthread_detach(exec_thread);
		       currentThreadIndex = (idx + 1) % totalThreads;
		       assigned = 1;
		       break;
                   }
	       }

              if (!assigned) {
                   printf("No hay hilos disponibles en este momento.\n");
               }
               pulseCount = 0;
           } else {
               printf("Todos los procesos han finalizado.\n");
         }
       }
   }
   return NULL;
}

void *threadedExecution(void *args) {
    ExecutionArgs *execArgs = (ExecutionArgs *)args;
    iniciarEjecucion(execArgs->thread, execArgs->pcb, execArgs->threadIndex);
    free(args);
    return NULL;
}
