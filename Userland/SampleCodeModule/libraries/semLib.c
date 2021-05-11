#include <semLib.h>

uint64_t sOpen(uint64_t id, uint64_t value)
{
   return syscall(SEM_OPEN, id, value, 0, 0, 0, 0);
}

int sWait(uint64_t id)
{
    return syscall(SEM_WAIT, id, 0, 0, 0, 0, 0);
}

int sPost(uint64_t id)
{
    return syscall(SEM_POST, id, 0, 0, 0, 0, 0);
}

int sClose(uint64_t id)
{
   return syscall(SEM_CLOSE, id, 0, 0, 0, 0, 0);
}