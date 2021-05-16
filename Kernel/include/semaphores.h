#ifndef _SEMAPHORES_H
#define _SEMAPHORES_H

#define MAX_BLOCKED_PIDS 10

#include <stdint.h>

uint64_t sOpen(uint64_t id, uint64_t initValue);
int sWait(uint64_t id);
int sPost(uint64_t id);
int sClose(uint64_t id);
void sStatus();
void getBlockedProc(char *buffer, int id);

#endif