#ifndef SYSTEM_CALLS
#define SYSTEM_CALLS

//dataTypes
#include <RTCTime.h>
#include <colours.h>
#include <cpuInfo.h>
#include <stdint.h>

typedef enum
{
    GET_MEM = 0,
    RTC_TIME = 1,
    TEMP = 2,
    WRITE = 3,
    GETCHAR = 4,
    CLEAR = 5,
    CREATE_PROC = 6,
    INFOREG = 7,
    MALLOC = 8,
    FREE = 9,
    KILL = 10,
    BLOCK = 11,
    UNLOCK = 12,
    GET_PID = 13,
    PS = 14,
    NICE = 15,
    DUMP_MM = 16
} syscallID;

uint64_t syscall(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9);

#endif