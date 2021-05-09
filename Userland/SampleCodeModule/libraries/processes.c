// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <processes.h>

int createProcess(void (*entryPoint)(int, char **), int argc, char **argv, int fg)
{
    return syscall(CREATE_PROC, (uint64_t)entryPoint, argc, (uint64_t)argv, (int)fg, 0, 0);
}

int killProcess(uint64_t pid)
{
    return syscall(KILL, pid, 0, 0, 0, 0, 0);
}

int blockProcess(uint64_t pid)
{
    return syscall(BLOCK, pid, 0, 0, 0, 0, 0);
}

int unblockProcess(uint64_t pid)
{
    return syscall(UNBLOCK, pid, 0, 0, 0, 0, 0);
}

int getPID()
{
    return syscall(GET_PID, 0, 0, 0, 0, 0, 0);
}

void nice(uint64_t pid, int priority)
{
    syscall(NICE, pid, priority, 0, 0, 0, 0);
}