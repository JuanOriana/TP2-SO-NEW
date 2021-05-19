// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <memoryManager.h>
#include <lib.h>
#include <semaphores.h>
#include <schedule.h>
#include <stringLib.h>

#define NULL 0

typedef struct Semaphore
{
    uint32_t id;
    uint32_t value;
    uint16_t listeners;
    uint32_t blockedPIDs[MAX_BLOCKED_PIDS];
    uint16_t blockedPIDsSize;
    int mutex;
    struct Semaphore *next;

} Semaphore;

Semaphore *semaphores = NULL;

static void dumpBlockedPIDs(uint32_t *blockedPIDs, uint16_t blockedPIDsSize);
static Semaphore *findSem(uint32_t id);

uint32_t sOpen(uint32_t id, uint32_t initValue)
{

    Semaphore *sem = findSem(id);
    if (sem == NULL)
    {
        sem = mallocCust(sizeof(Semaphore));
        if (sem == NULL)
            return -1;

        sem->value = initValue;
        sem->listeners = 0;
        sem->blockedPIDsSize = 0;
        sem->id = id;
        sem->next = NULL;
        sem->mutex = 0;

        Semaphore *lastSem = semaphores;
        if (lastSem == NULL)
            semaphores = sem;
        else
        {
            while (lastSem->next != NULL)
                lastSem = lastSem->next;
            lastSem->next = sem;
        }
    }

    if (sem->listeners >= MAX_BLOCKED_PIDS)
    {
        print("No space for this listener");
        return -1;
    }

    sem->listeners++;
    return id;
}

Semaphore *findSem(uint32_t id)
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

int sWait(uint32_t id)
{
    Semaphore *sem = findSem(id);
    if (sem == NULL)
        return 1;

    acquire(&(sem->mutex));
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
        release(&(sem->mutex));
        blockProcess(currPid);
    }

    return 0;
}

int sPost(uint32_t id)
{
    Semaphore *sem = findSem(id);
    if (sem == NULL)
        return 1;
    acquire(&(sem->mutex));
    if (sem->blockedPIDsSize > 0)
    {
        int nextPid = sem->blockedPIDs[0];
        for (int i = 0; i < sem->blockedPIDsSize - 1; i++)
            sem->blockedPIDs[i] = sem->blockedPIDs[i + 1];
        sem->blockedPIDsSize--;
        unblockProcess(nextPid);
        release(&(sem->mutex));
        return 0;
    }
    else
        sem->value++;
    release(&(sem->mutex));
    return 0;
}

int sClose(uint32_t id)
{
    Semaphore *sem = findSem(id);
    if (sem == NULL)
        return 1;
    sem->listeners--;
    if (sem->listeners > 0)
        return 0;
    Semaphore *aux = semaphores;
    if (aux == sem)
        semaphores = aux->next;
    else
    {
        while (aux->next != sem)
            aux = aux->next;
        aux->next = sem->next;
    }
    freeCust(sem);
    return 0;
}

void sStatus()
{
    print("\nSEMAPHORE DUMP\n");
    print("------------------------------------------------\n");
    print("Active semaphores:\n");
    Semaphore *sem = semaphores;
    int i = 1;
    while (sem)
    {
        print("-------------------------------\n");
        print("Semaphore %d\n", i++);
        print("     ID: %d\n", sem->id);
        print("     Value: %d\n", sem->value);
        print("     Number of attached processes: %d\n", sem->listeners);
        print("     Number of blocked processes: %d\n", sem->blockedPIDsSize);
        print("     Blocked processes:\n");
        dumpBlockedPIDs(sem->blockedPIDs, sem->blockedPIDsSize);
        sem = sem->next;
    }
    print("-------------------------------\n");
}

static void dumpBlockedPIDs(uint32_t *blockedPIDs, uint16_t blockedPIDsSize)
{
    for (int i = 0; i < blockedPIDsSize; i++)
    {
        print("         PID: %d\n", blockedPIDs[i]);
    }
}