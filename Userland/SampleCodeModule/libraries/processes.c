#include <processes.h>

int createProcess(void (*entryPoint)(int, const char **), int argc, char **argv)
{
    return syscall(CREATE_PROC, (uint64_t)entryPoint, argc, (uint64_t)argv, 0, 0, 0);
}

int killProcess(uint64_t pid)
{
    return syscall(KILL, pid, 0, 0, 0, 0, 0);
}

int blockProcess(uint64_t pid)
{
    return syscall(BLOCK, pid, 0, 0, 0, 0, 0);
}

int unlockProcess(uint64_t pid)
{
    return syscall(UNLOCK, pid, 0, 0, 0, 0, 0);
}

int getPID() 
{
    return syscall(GET_PID, 0, 0, 0, 0, 0, 0);
}