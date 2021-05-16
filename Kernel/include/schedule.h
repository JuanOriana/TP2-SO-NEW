#ifndef SCHEDULE_H
#define SCHEDULE_H

#define MAX_PROCESSES 2

#include <stdint.h>
#include <screens.h>

void initScheduler();
void *scheduler(void *oldRSP);
int addProcess(void (*entryPoint)(int, char **), int argc, char **argv, int fg, int *fd);
uint64_t killProcess(uint64_t pid);
uint64_t blockProcess(uint64_t pid);
uint64_t unblockProcess(uint64_t pid);
int getCurrPID();
void processDisplay();
void setNewCycle(uint64_t pid, int priority);
void killFgProcess();
void yield();
int currentReadsFrom();
int currentWritesTo();
int currentProcessFg();

#endif