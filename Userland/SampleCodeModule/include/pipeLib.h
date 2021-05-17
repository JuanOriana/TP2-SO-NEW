#ifndef PIPELIB_H
#define PIPELIB_H

#include <systemCalls.h>

uint64_t pOpen(uint64_t id);

int pRead(uint64_t id);

int pWrite(uint64_t id, char *str);

int pClose(uint64_t id);

int pDump();

#endif