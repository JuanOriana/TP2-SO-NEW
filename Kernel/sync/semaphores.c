#include <memoryManager.h>
#include <lib.h>
#include <semaphores.h>
#include <schedule.h>

#define NULL 0
Semaphore *semaphores = NULL;


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
        int currPid = getCurrPID();
        sem->blockedPIDs[sem->blockedPIDsSize++] = currPid;
        blockProcess(currPid);
        sem->value--;
    }
    release(&sem->mutex);
    return 0;
}

int sPost(Semaphore *sem)
{
    if (!findSem(sem->id))
        return 1;
    acquire(&sem->mutex);
    sem->value++;
    if (sem->blockedPIDsSize > 0)
    {
        int nextPid = sem->blockedPIDs[0];
        for (int i = 0; i < sem->blockedPIDsSize - 1; i++)
            sem->blockedPIDs[i] = sem->blockedPIDs[i + 1];
        unblockProcess(nextPid);
    }
    release(&sem->mutex);
    return 0;
}

int sClose(Semaphore *sem)
{
    if (!findSem(sem->id))
        return 1;
    Semaphore *aux = semaphores;
    //What if there are some processes using this sem?
    while (aux->next != sem)
        aux = aux->next;
    aux->next = sem->next;
    freeCust(sem);
    return 0;
}