// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <testUtil.h>
#include <stdint.h>
#include <stringLib.h>

#define LOW_PRIO 1
#define MED_PRIO 20
#define HIGH_PRIO 40

#define TOTAL_PROCESSES 3 // TODO: Long enough to see theese processes beeing run at least twice

void testPrio()
{
    uint64_t pids[TOTAL_PROCESSES];
    uint64_t i;

    
    char buffer[3];

    for (i = 0; i < TOTAL_PROCESSES; i++)
    {
        char *argv[] = {"endlessLoop",itoa(i,buffer,DECIMAL_BASE)};
        pids[i] = createProcess(&endlessLoop, 2, argv, BG, NULL);
    }

    busyWait(3 * MAJOR_WAIT);

    print("\nCHANGING PRIORITIES...\n");

    nice(pids[0], LOW_PRIO);
    nice(pids[1], MED_PRIO);
    nice(pids[2], HIGH_PRIO);

    busyWait(3 * MAJOR_WAIT);

    print("\nBLOCKING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        blockProcess(pids[i]);

    print("CHANGING PRIORITIES WHILE BLOCKED...\n");
    for (i = 0; i < TOTAL_PROCESSES; i++)
        nice(pids[i], MED_PRIO);

    print("UNBLOCKING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        unblockProcess(pids[i]);

    busyWait(3 * MAJOR_WAIT);

    print("\nKILLING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        killProcess(pids[i]);
    return;
}
