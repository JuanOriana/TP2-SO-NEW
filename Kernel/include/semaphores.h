#ifndef _SEMAPHORES_H
#define _SEMAPHORES_H

#define MAX_BLOCKED_PIDS 10
#define NULL 0

typedef struct
{
    int id;
    int value;
    int listeners;
    int blockedPIDs[MAX_BLOCKED_PIDS];
    int blockedPIDsSize;
    int mutex;
    Semaphore *next;

} Semaphore;

Semaphore *semaphores = NULL;

Semaphore *sOpen(int id, unsigned int initValue);

int sWait(Semaphore *sem);

int sPost(Semaphore *sem);

int sClose(Semaphore *sem);

void sStatus(void *buffer, int *qty);

void getBlockedProc(char *buffer, int id);

#endif