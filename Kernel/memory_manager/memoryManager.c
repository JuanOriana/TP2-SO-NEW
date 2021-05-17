// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "memoryManager.h"
#include "stringLib.h"

#define NULL 0

typedef long Align;
typedef union header Header;

union header
{
      struct
      {
            union header *ptr;
            unsigned size;
      } s;
      Align x;
};

static Header *base;
static Header *freep = NULL;

unsigned long totalUnits;

void memInitList(char *memBase, unsigned long memSize)
{
      // Initially its all a very large block
      totalUnits = (memSize + sizeof(Header) - 1) / sizeof(Header) + 1;
      freep = base = (Header *)memBase;
      freep->s.size = totalUnits;
      freep->s.ptr = freep;
}

// Ref for malloc/free : The C Programming Language  - K&R
void *mallocCustList(unsigned long nbytes)
{
      if (nbytes == 0)
            return NULL;

      unsigned long nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1; //Normalize to header units

      Header *p, *prevp;
      prevp = freep;

      for (p = prevp->s.ptr;; prevp = p, p = p->s.ptr)
      {
            if (p->s.size >= nunits) //If >=, can use this block
            {
                  if (p->s.size == nunits) // Equal just use
                        prevp->s.ptr = p->s.ptr;
                  else // Fragment block
                  {
                        p->s.size -= nunits;
                        p += p->s.size;
                        p->s.size = nunits;
                  }
                  freep = prevp;
                  return (void *)(p + 1); //Return new memspace WITHOUT header
            }
            if (p == freep) // No block found, need more space
                  return NULL;
      }
}

void freeCustList(void *freeMem)
{
      if (freeMem == NULL || (((long)freeMem - (long)base) % sizeof(Header)) != 0) //Discard null and unaligned
            return;

      Header *freeBlock, *p;
      freeBlock = (Header *)freeMem - 1; //Add header to mem to free

      if (freeBlock < base || freeBlock >= (base + totalUnits * sizeof(Header)))
            return;

      char isExternal = 0;

      for (p = freep; !(freeBlock > p && freeBlock < p->s.ptr); p = p->s.ptr)
      { // Find blocks that surround

            if (freeBlock == p || freeBlock == p->s.ptr) // block is already free!
                  return;

            if (p >= p->s.ptr && (freeBlock > p || freeBlock < p->s.ptr))
            { //Free block might be on the ends
                  isExternal = 1;
                  break;
            }
      }

      if (!isExternal && (p + p->s.size > freeBlock || freeBlock + freeBlock->s.size > p->s.ptr)) //Absurd!!
            return;

      if (freeBlock + freeBlock->s.size == p->s.ptr) //Join right
      {
            freeBlock->s.size += p->s.ptr->s.size;
            freeBlock->s.ptr = p->s.ptr->s.ptr;
      }
      else
            freeBlock->s.ptr = p->s.ptr;

      if (p + p->s.size == freeBlock) //Join left
      {
            p->s.size += freeBlock->s.size;
            p->s.ptr = freeBlock->s.ptr;
      }
      else
            p->s.ptr = freeBlock;

      freep = p;
}

void dumpMMList()
{
      long long idx = 1;
      Header *original, *p;
      original = p = freep;
      int flag = 1;

      print("Free List MM dump\n");
      print("Total memory: %d\n", totalUnits * sizeof(Header));
      if (freep == NULL)
            print("    No free blocks\n");

      while (p != original || flag)
      {
            flag = 0;
            print("    Block number %d\n", idx);
            print("        Base: %x\n", (uint64_t)p);
            print("        Free blocks: %d\n", p->s.size);
            p = p->s.ptr;
            idx++;
      }
}