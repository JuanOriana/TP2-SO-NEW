#ifndef _SEMAPHORES_H
#define _SEMAPHORES_H

#define MAX_BLOCKED_PIDS 10

typedef struct Semaphore
{
    int id;
    int value;
    int listeners;
    int blockedPIDs[MAX_BLOCKED_PIDS];
    int blockedPIDsSize;
    int mutex;
    struct Semaphore *next;

} Semaphore;

uint64_t sOpen(uint64_t id, uint64_t initValue);

int sWait(uint64_t id);

int sPost(uint64_t id);

int sClose(uint64_t id);

Semaphore *findSem(uint64_t id);

void sStatus();

void getBlockedProc(char *buffer, int id);

#endif