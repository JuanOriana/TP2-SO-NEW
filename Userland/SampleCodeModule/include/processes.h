#ifndef _PROCESSES_H
#define _PROCESSES_H

#include <stdint.h>
#include <systemCalls.h>

#define NULL (void *) 0
#define FG 1
#define BG 0

int createProcess(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int *fd);
int killProcess(uint64_t pid);
int blockProcess(uint64_t pid);
int unblockProcess(uint64_t pid);
void nice(uint64_t pid, int priority);
int getPID();
void yield();
void wait(uint64_t pid);

#endif