#include <memoryManager.h>
#include <processes.h>
#include <lib.h>

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

int sClose(int id);

void sStatus(void *buffer, int *qty);

void getBlockedProc(char *buffer, int id);

Semaphore *sOpen(int id, unsigned int initValue)
{
    Semaphore *sem = findSem(id);
    if (sem == NULL)
        sem = mallocCust(sizeof(Semaphore));
    if (sem == NULL)
        return NULL;
    sem->value = initValue;
    sem->listeners = 0;
    sem->blockedPIDsSize = 0;
    sem->id = id;
    sem->next = NULL;

    Semaphore *lastSem = semaphores;
    if (lastSem == NULL)
        semaphores = lastSem = sem;
    else
    {
        while (lastSem->next != NULL)
            lastSem = lastSem->next;
        lastSem->next = sem;
    }

    sem->listeners++;

    return sem;
}

Semaphore *findSem(int id)
{
    Semaphore *sem = semaphores;
    while (sem)
    {
        if (sem->id == id)
            return sem;
        sem = sem->next;
    }
    return NULL;
}

int sWait(Semaphore *sem)
{
    if (!findSem(sem->id))
        return 1;
    acquire(&sem->mutex);
    if (sem->value > 0)
        sem->value--;
    else
    {
        int currPid = getPID();
        sem->blockedPIDs[sem->blockedPIDsSize++] = currPid;
        blockProcess(currPid);
        sem->value--;
    }
    release(&sem->mutex);
    return 0;
}

int sPost(Semaphore *sem)
{
    
}