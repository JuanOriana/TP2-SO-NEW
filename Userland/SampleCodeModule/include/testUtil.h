#include <stdint.h>
#include <stringLib.h>
#include <utils.h>
#include <processes.h>

#define MINOR_WAIT 100000
#define MAJOR_WAIT 10000000

uint32_t GetUint();
uint32_t GetUniform(uint32_t max);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);
void testPrio();
void testProcesses();
void endlessLoop(int argc, char *argv[]);
void busyWait(uint64_t n);
