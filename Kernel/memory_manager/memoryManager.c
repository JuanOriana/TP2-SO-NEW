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
static Header *startingNode = NULL;

unsigned long totalUnits;

void memInitList(char *memBase, unsigned long memSize)
{
      // Initially its all a very large block
      totalUnits = (memSize + sizeof(Header) - 1) / sizeof(Header) + 1;
      startingNode = base = (Header *)memBase;
      startingNode->s.size = totalUnits;
      startingNode->s.ptr = startingNode;
}

// Ref for malloc/free : The C Programming Language  - K&R
void *mallocCustList(unsigned long nbytes)
{
      if (nbytes == 0)
            return NULL;

      unsigned long nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1; //Normalize to header units

      Header *currNode, *prevNode;
      prevNode = startingNode;

      for (currNode = prevNode->s.ptr;; prevNode = currNode, currNode = currNode->s.ptr)
      {
            if (currNode->s.size >= nunits) //If >=, can use this block
            {
                  if (currNode->s.size == nunits) // Equal just use
                        prevNode->s.ptr = currNode->s.ptr;
                  else // Fragment block
                  {
                        currNode->s.size -= nunits;
                        currNode += currNode->s.size;
                        currNode->s.size = nunits;
                  }
                  startingNode = prevNode;
                  return (void *)(currNode + 1); //Return new memspace WITHOUT header
            }
            if (currNode == startingNode) // No block found, need more space
                  return NULL;
      }
}

void freeCustList(void *freeMem)
{
      if (freeMem == NULL || (((long)freeMem - (long)base) % sizeof(Header)) != 0) //Discard null and unaligned
            return;

      Header *freeBlock, *currNode;
      freeBlock = (Header *)freeMem - 1; //Add header to mem to free

      if (freeBlock < base || freeBlock >= (base + totalUnits * sizeof(Header)))
            return;

      char isExternal = 0;

      for (currNode = startingNode; !(freeBlock > currNode && freeBlock < currNode->s.ptr); currNode = currNode->s.ptr)
      { // Find blocks that surround

            if (freeBlock == currNode || freeBlock == currNode->s.ptr) // block is already free!
                  return;

            if (currNode >= currNode->s.ptr && (freeBlock > currNode || freeBlock < currNode->s.ptr))
            { //Free block might be on the ends
                  isExternal = 1;
                  break;
            }
      }

      if (!isExternal && (currNode + currNode->s.size > freeBlock || freeBlock + freeBlock->s.size > currNode->s.ptr)) //Absurd!!
            return;

      if (freeBlock + freeBlock->s.size == currNode->s.ptr) //Join right
      {
            freeBlock->s.size += currNode->s.ptr->s.size;
            freeBlock->s.ptr = currNode->s.ptr->s.ptr;
      }
      else
            freeBlock->s.ptr = currNode->s.ptr;

      if (currNode + currNode->s.size == freeBlock) //Join left
      {
            currNode->s.size += freeBlock->s.size;
            currNode->s.ptr = freeBlock->s.ptr;
      }
      else
            currNode->s.ptr = freeBlock;

      startingNode = currNode;
}

void dumpMMList()
{
      long long idx = 1;
      Header *original, *p;
      original = p = startingNode;
      int flag = 1;

      print("\nMEMORY DUMP (Free List)\n");
      print("- - Units of 16 bytes\n");
      print("------------------------------------------------\n");
      print("Total memory: %d units\n\n", totalUnits * sizeof(Header));
      if (startingNode == NULL)
            print("    No free blocks\n");
      print("Free blocks: \n");
      print("-------------------------------\n");
      while (p != original || flag)
      {
            flag = 0;
            print("    Block number %d\n", idx);
            print("        Base: %x\n", (uint64_t)p);
            print("        Free units: %d\n", p->s.size);
            print("-------------------------------\n");
            p = p->s.ptr;
            idx++;
      }
      print("\n\n");
}