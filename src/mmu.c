#include <stdio.h>
#include "structs.h"
#include "tlb.h"
#include "commons.h"

void initMMU(MMU *mmu) {
    initTLB(&mmu->tlb);
    mmu->tlbIndex = 0;
}

int getNextTLBIndex(MMU *mmu) {
    int index = mmu->tlbIndex;
    mmu->tlbIndex = (mmu->tlbIndex + 1) % TLB_SIZE;
    return index;
}

uint64_t translateAddress(HardwareThread *thread, uint32_t virtualAddress) {
    uint64_t pageOffset = virtualAddress & PAGE_MASK;
    uint64_t pageIndex = virtualAddress >> PAGE_SHIFT;
/*    printf("Direccion virtual recibida: 0x%X\n", virtualAddress);
    printf("Page offset: 0x%lX\n", pageOffset);
    printf("Page index: 0x%lX\n", pageIndex);*/

    int i;
    for (i = 0; i < TLB_SIZE; i++) {
        TLBEntrada tlbEntry = thread->mmu.tlb.entries[i];
        if (tlbEntry.valid && (tlbEntry.virtual >> PAGE_SHIFT) == pageIndex) {
	    uint64_t resolvedAddress = tlbEntry.physical | pageOffset;
//	    printf("TLB Hit: Direccon fisica retornada 0x%lX\n", resolvedAddress);
            return tlbEntry.physical + pageOffset;
        }
    }

    uint64_t baseAddress = *(thread->PTBR);
    uint64_t physicalAddress = baseAddress + pageOffset;
  //  printf("TLB miss: base address 0x%lX\n", baseAddress);
  //  printf("Tlb miss: direccion fisica retornada 0x%lX\n", physicalAddress);

    int tlbIndex = getNextTLBIndex(&thread->mmu);
    thread->mmu.tlb.entries[tlbIndex].virtual = virtualAddress & ~(PAGE_MASK);
    thread->mmu.tlb.entries[tlbIndex].physical = baseAddress;
    thread->mmu.tlb.entries[tlbIndex].valid = true;
    return physicalAddress;
}
