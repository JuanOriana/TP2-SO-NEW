// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <testMem.h>

#define MAX_BLOCKS 128
#ifdef FREE_LIST
#define MAX_MEMORY 96 * 1024 * 1024 //Should be around 80% of memory managed by the MM
#else
#define MAX_MEMORY 32 * 1024 * 1024
#endif

typedef struct MM_rq
{
  void *address;
  uint32_t size;
} mm_rq;

void testMem(int argc, char *argv[])
{
  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;

  print("Memory test begins:\n");

  while (1)
  {
    rq = 0;
    total = 0;

    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < MAX_MEMORY)
    {
      mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
      mm_rqs[rq].address = (void *)mallocCust(mm_rqs[rq].size); // TODO: Port this call as required
      if (mm_rqs[rq].address == NULL)
      {
        print("No memory!\n");
        return;
      }
      total += mm_rqs[rq].size;
      rq++;
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        memoryset(mm_rqs[i].address, i, mm_rqs[i].size); // TODO: Port this call as required

    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
          printStringLn("ERROR!"); // TODO: Port this call as required

    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        freeCust(mm_rqs[i].address); // TODO: Port this call as required
  }
}
