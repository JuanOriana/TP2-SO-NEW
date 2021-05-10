#include <semLib.h>

Semaphore *sOpen(int id, unsigned int value)
{
    syscall(SEM_OPEN, id, value, 0, 0, 0, 0);
}

int sWait(Semaphore *sem)
{
    syscall(SEM_WAIT, sem, 0, 0, 0, 0, 0);
}

int sPost(Semaphore *sem)
{
    syscall(SEM_POST, sem, 0, 0, 0, 0, 0);
}

int sClose(Semaphore *sem)
{
    syscall(SEM_CLOSE, sem, 0, 0, 0, 0, 0);
}