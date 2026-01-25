#ifndef UNTITLED_PAGE_TABLE_H
#define UNTITLED_PAGE_TABLE_H

#include "structs.h"

uint64_t* initPageTableForProcess(size_t totalEntries, uint32_t* programSegment);
void initPageDirectory(int numTables, uint32_t *kernelBase);

#endif
