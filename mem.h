#ifndef MEM_H
#define MEM_H

#include <stdint.h>

typedef struct _mem_t {
  void* MemAddr;
  void* MemFreeList;
  uint32_t MemBlkSize;
  uint32_t MemNBlks;
  uint32_t MemNFree;
} mem_t;

extern mem_t* MemCreate(void* addr, uint32_t nblks, uint32_t blksize);
extern void* MemGet(mem_t* pmem);
extern void MemPut(mem_t* pmem, void* pblk);

#endif /* end of include guard: MEM_H */
