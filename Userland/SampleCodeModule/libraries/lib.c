// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <systemCalls.h>
#include <stdint.h>
#include <lib.h>

void* mallocCust(unsigned int qty)
{
    return (void*)syscall(MALLOC, qty, 0, 0, 0, 0, 0);
}

void freeCust(void * memory)
{
    syscall(FREE, (uint64_t)memory, 0, 0, 0, 0, 0);
}