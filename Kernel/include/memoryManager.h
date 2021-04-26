#ifndef _MEMMANAG_H
#define _MEMMANAG_H

void memInit(char *memBase, unsigned long memSize);
void *mallocCust(unsigned long nbytes);
void freeCust(void *ap);
void dumpMM(long *baseRet, long *freeMem, long *totalMem);

#endif