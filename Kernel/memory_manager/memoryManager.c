// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifdef FREE_LIST

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
      } data;
      Align x;
};

static Header *base;
static Header *startingNode = NULL;

unsigned long totalUnits;

void memInit(char *memBase, unsigned long memSize)
{
      // Initially its all a very large block
      totalUnits = (memSize + sizeof(Header) - 1) / sizeof(Header) + 1;
      startingNode = base = (Header *)memBase;
      startingNode->data.size = totalUnits;
      startingNode->data.ptr = startingNode;
}

// Ref for malloc/free : The C Programming Language  - K&R
void *mallocCust(unsigned long nbytes)
{
      if (nbytes == 0)
            return NULL;

      unsigned long nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1; //Normalize to header units

      Header *currNode, *prevNode;
      prevNode = startingNode;

      for (currNode = prevNode->data.ptr;; prevNode = currNode, currNode = currNode->data.ptr)
      {
            if (currNode->data.size >= nunits)
            {
                  if (currNode->data.size == nunits) // Equal just use
                        prevNode->data.ptr = currNode->data.ptr;
                  else
                  {
                        currNode->data.size -= nunits;
                        currNode += currNode->data.size;
                        currNode->data.size = nunits;
                  }
                  startingNode = prevNode;
                  return (void *)(currNode + 1); //Return new memspace WITHOUT header
            }
            if (currNode == startingNode)
                  return NULL;
      }
}

void freeCust(void *freeMem)
{
      if (freeMem == NULL || (((long)freeMem - (long)base) % sizeof(Header)) != 0)
            return;

      Header *freeBlock, *currNode;
      freeBlock = (Header *)freeMem - 1; //Add header to mem to free

      if (freeBlock < base || freeBlock >= (base + totalUnits * sizeof(Header)))
            return;

      char isExternal = 0;

      for (currNode = startingNode; !(freeBlock > currNode && freeBlock < currNode->data.ptr); currNode = currNode->data.ptr)
      {

            if (freeBlock == currNode || freeBlock == currNode->data.ptr)
                  return;

            if (currNode >= currNode->data.ptr && (freeBlock > currNode || freeBlock < currNode->data.ptr))
            {
                  isExternal = 1;
                  break;
            }
      }

      if (!isExternal && (currNode + currNode->data.size > freeBlock || freeBlock + freeBlock->data.size > currNode->data.ptr)) //Absurd!!
            return;

      if (freeBlock + freeBlock->data.size == currNode->data.ptr) //Join right
      {
            freeBlock->data.size += currNode->data.ptr->data.size;
            freeBlock->data.ptr = currNode->data.ptr->data.ptr;
      }
      else
            freeBlock->data.ptr = currNode->data.ptr;

      if (currNode + currNode->data.size == freeBlock) //Join left
      {
            currNode->data.size += freeBlock->data.size;
            currNode->data.ptr = freeBlock->data.ptr;
      }
      else
            currNode->data.ptr = freeBlock;

      startingNode = currNode;
}

void dumpMM()
{
      long long idx = 1;
      Header *original, *current;
      original = current = startingNode;
      int flag = 1;

      print("\nMEMORY DUMP (Free List)\n");
      print("- - Units of 16 bytes\n");
      print("------------------------------------------------\n");
      print("Total memory: %d bytes\n\n", totalUnits * sizeof(Header));
      if (startingNode == NULL)
            print("    No free blocks\n");
      print("Free blocks: \n");
      print("-------------------------------\n");
      while (current != original || flag)
      {
            flag = 0;
            print("    Block number %d\n", idx);
            print("        Base: %x\n", (uint64_t)current);
            print("        Free units: %d\n", current->data.size);
            print("-------------------------------\n");
            current = current->data.ptr;
            idx++;
      }
      print("\n\n");
}

#endif