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

Semaphore *sOpen(int id, unsigned int initValue);

int sWait(Semaphore *sem);

int sPost(Semaphore *sem);

int sClose(Semaphore *sem);

Semaphore *findSem(int id);

void sStatus();

void getBlockedProc(char *buffer, int id);

#endif