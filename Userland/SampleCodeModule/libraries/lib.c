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