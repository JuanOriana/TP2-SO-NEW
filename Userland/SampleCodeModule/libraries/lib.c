#include <systemCalls.h>
#include <stdint.h>
#include <lib.h>

uint64_t mallocCust(unsigned int qty)
{
    return syscall(MALLOC, qty, 0, 0, 0, 0, 0);
}

void freeCust(uint64_t memory)
{
    syscall(FREE, memory, 0, 0, 0, 0, 0);
}