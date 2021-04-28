#include <RTCTime.h>
#include <keyboardDriver.h>
#include <lib.h>
#include <stringLib.h>
#include <sysCallDispatcher.h>
#include <schedule.h>
#include <videoDriver.h>
#include <memoryManager.h>

#define SYS_GETMEM_ID 0
#define SYS_RTC_TIME_ID 1
#define SYS_TEMP_ID 2
#define SYS_WRITE_ID 3
#define SYS_GETCHAR_ID 4
#define SYS_CLEAR_ID 5
#define SYS_INIT_PROC_ID 6
#define SYS_INFOREG_ID 7
#define SYS_MALLOC_ID 8
#define SYS_FREE_ID 9
#define SYS_KILL_ID 10
#define SYS_BLOCK_ID 11
#define SYS_UNLOCK_ID 12
#define SYS_GET_PID 13

#define SYSCALLS_QTY 14

uint64_t sysCallDispatcher(t_registers *r)
{
      if (r->rax >= 0 && r->rax < SYSCALLS_QTY)
      {
            switch (r->rax)
            {
            case SYS_GETMEM_ID:
                  sys_getMem(r->rdi, (uint8_t *)r->rsi);
                  break;

            case SYS_RTC_TIME_ID:
                  return getDecimalTimeInfo((t_timeInfo)(r->rdi));
                  break;

            case SYS_TEMP_ID:
                  return cpuTemp();
                  break;

            case SYS_WRITE_ID:
                  sys_write((char *)(r->rdi), (uint8_t)(r->rsi), (t_colour)(r->rdx), (t_colour)(r->r10));
                  break;

            case SYS_GETCHAR_ID:
                  return getchar();
                  break;

            case SYS_CLEAR_ID:
                  clearScreen();
                  break;

            case SYS_INIT_PROC_ID:
                  return addProcess((void (*)(int, char **))r->rdi, (int)r->rsi, (char **)r->rdx);
                  break;
            case SYS_INFOREG_ID:
                  return (uint64_t)getSnapshot();
                  break;
            case SYS_MALLOC_ID:
                  return (uint64_t)mallocCust(r->rdi);
                  break;
            case SYS_FREE_ID:
                  freeCust((void *)(r->rdi));
                  break;
            case SYS_KILL_ID:
                  return killProcess(r->rdi);
                  break;
            case SYS_BLOCK_ID:
                  return blockProcess(r->rdi);
                  break;
            case SYS_UNLOCK_ID:
                  return unblockProcess(r->rdi);
                  break;
            case SYS_GET_PID:
                  return getCurrPID();
                  break;
            }
      }
      return 0;
}