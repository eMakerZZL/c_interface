#include "mem.h"
#include <assert.h>
#include <string.h>

mem_t* MemCreate(void* addr, uint32_t nblks, uint32_t blksize)
{
    mem_t* pmem;
    uint8_t* pblk;
    void** plink;
    uint32_t i;

    pmem = (mem_t*)malloc(sizeof(*pmem));
    assert(pmem);

    plink = (void**)addr;
    pblk = (uint8_t*)addr + blksize;

    for (i = 0; i < (nblks - 1); i++) {
        *plink = (void*)pblk;
        plink = *plink;
        pblk = pblk + blksize;
    }
    *plink = (void*)0;

    pmem->MemAddr = addr;
    pmem->MemFreeList = addr;
    pmem->MemNFree = nblks;
    pmem->MemNBlks = nblks;
    pmem->MemBlkSize = blksize;

    return (pmem);
}

void* MemGet(mem_t* pmem)
{
    void* pblk;

    assert(pmem);
    assert(pmem->MemNFree > 0);

    pblk = pmem->MemFreeList;
    pmem->MemFreeList = *(void**)pblk;
    pmem->MemNFree -= 1;

    return (pblk);
}

void MemPut(mem_t* pmem, void* pblk)
{
    assert(pmem);
    assert(pblk);

    if (pmem->MemNFree < pmem->MemNBlks) {
        *(void**)pblk = pmem->MemFreeList;
        pmem->MemFreeList = pblk;
        pmem->MemNFree += 1;
    } 
}
