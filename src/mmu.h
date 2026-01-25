#ifndef UNTITLED_MMU_H
#define UNTITLED_MMU_H

#include "structs.h"

void initMMU(MMU *mmu);
uint64_t translateAddress(HardwareThread *thread, uint32_t virtualAddress);

#endif
