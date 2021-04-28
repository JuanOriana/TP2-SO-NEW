#ifndef _PROCESSES_H
#define _PROCESSES_H

#include <stdint.h>
#include <systemCalls.h>

int createProcess(void (*entryPoint)(int, const char **), int argc, char **argv);
int killProcess(uint64_t pid);
int blockProcess(uint64_t pid);
int unlockProcess(uint64_t pid);
int getPID();

#endif