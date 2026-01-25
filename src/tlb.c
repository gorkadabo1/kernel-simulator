#include "structs.h"

void initTLB(TLB *tlb) {
    int i;
    for (i = 0; i < TLB_SIZE; i++) {
        tlb->entries[i].virtual = 0;
        tlb->entries[i].physical = 0;
        tlb->entries[i].valid = false;
    }
}