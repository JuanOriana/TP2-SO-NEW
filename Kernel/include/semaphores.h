#ifndef _SEMAPHORES_H
#define _SEMAPHORES_H

#define MAX_BLOCKED_PIDS 16

#include <stdint.h>

uint32_t sOpen(uint32_t id, uint32_t initValue);
int sWait(uint32_t id);
int sPost(uint32_t id);
int sClose(uint32_t id);
void sStatus();

#endif