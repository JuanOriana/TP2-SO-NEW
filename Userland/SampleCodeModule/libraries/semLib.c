#include <semLib.h>

Semaphore *sOpen(int id, unsigned int value)
{
   return (Semaphore *)syscall(SEM_OPEN, id, value, 0, 0, 0, 0);
}

int sWait(Semaphore *sem)
{
    return syscall(SEM_WAIT, (uint64_t)sem, 0, 0, 0, 0, 0);
}

int sPost(Semaphore *sem)
{
    return syscall(SEM_POST, (uint64_t)sem, 0, 0, 0, 0, 0);
}

int sClose(Semaphore *sem)
{
   return  syscall(SEM_CLOSE, (uint64_t)sem, 0, 0, 0, 0, 0);
}