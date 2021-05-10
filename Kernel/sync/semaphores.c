#include <memoryManager.h>
#include <lib.h>
#include <semaphores.h>
#include <schedule.h>
#include <stringLib.h>

#define NULL 0
Semaphore *semaphores = NULL;

static void dumpBlockedPIDs(int *blockedPIDs, int blockedPIDsSize);

Semaphore *sOpen(int id, unsigned int initValue)
{
    Semaphore *sem = findSem(id);
    if (sem == NULL)
    {
        sem = mallocCust(sizeof(Semaphore));
        if (sem == NULL)
        {
            return NULL;
        }
        sem->value = initValue;
        sem->listeners = 0;
        sem->blockedPIDsSize = 0;
        sem->id = id;
        sem->next = NULL;
        sem->mutex = 0;

        Semaphore *lastSem = semaphores;
        if (lastSem == NULL)
            semaphores = lastSem = sem;
        else
        {
            while (lastSem->next != NULL)
                lastSem = lastSem->next;
            lastSem->next = sem;
        }
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
    {
        sem->value--;
        release(&(sem->mutex));
        return 0;
    }
    else
    {
        int currPid = getCurrPID();
        sem->blockedPIDs[sem->blockedPIDsSize++] = currPid;
        blockProcess(currPid);
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
        sem->blockedPIDsSize--;
        unblockProcess(nextPid);
    }
    release(&sem->mutex);
    return 0;
}

int sClose(Semaphore *sem)
{
    if (!findSem(sem->id))
        return 1;
    sem->listeners--;
    if (sem->listeners > 0)
        return 0;
    Semaphore *aux = semaphores;
    if (aux == sem)
    {
        semaphores = aux->next;
    }
    else
    {
        while (aux->next != sem)
        {
            aux = aux->next;
        }
        aux->next = sem->next;
    }
    freeCust(sem);
    return 0;
}

void sStatus()
{
    print("Active semaphores:\n");
    Semaphore *sem = semaphores;
    int i = 1;
    while (sem)
    {
        print("Semaphore %d\n", i++);
        print("     Index: %d\n", sem->id);
        print("     Value: %d\n", sem->value);
        print("     Number of attached processes: %d\n", sem->listeners);
        print("     Number of blocked processes: %d\n", sem->blockedPIDsSize);
        print("     Blocked processes:\n");
        dumpBlockedPIDs(sem->blockedPIDs, sem->blockedPIDsSize);
        sem = sem->next;
    }
}

static void dumpBlockedPIDs(int *blockedPIDs, int blockedPIDsSize)
{
    for (int i = 0; i < blockedPIDsSize; i++)
    {
        print("         PID: %d\n", blockedPIDs[i]);
    }
}