#ifndef UNTITLED_UTILITIES_H
#define UNTITLED_UTILITIES_H

#include "structs.h"

void iniProcessQueue(int capacity);
bool enqueuePCB(PCB pcb);
int countElfFiles(const char* directory);
PCB dequeuePCB();
void iniciarEjecucion(HardwareThread *thread, PCB pcb, int threadIndex);

#endif
