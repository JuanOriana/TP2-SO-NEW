#ifndef _PROCESSES_H
#define _PROCESSES_H

#include <stdint.h>
#include <systemCalls.h>

int createProcess(void (*entryPoint)(int, const char **), int argc, char **argv);
int killProcess(uint64_t pid);
int blockProcess(uint64_t pid);
int unblockProcess(uint64_t pid);
void nice(uint64_t pid, int priority);
int getPID();

#endif