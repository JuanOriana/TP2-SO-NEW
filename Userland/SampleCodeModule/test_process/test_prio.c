// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <stdint.h>
#include <stringLib.h>
#include <utils.h>
#include <test_util.h>
#include <processes.h>

#define MINOR_WAIT 1000000 // TODO: To prevent a process from flooding the screen
#define WAIT 10000000      // TODO: Long enough to see theese processes beeing run at least twice

void bussy_wait(uint64_t n)
{
    uint64_t i;
    for (i = 0; i < n; i++)
        ;
}

void endless_loop(int argc, char *argv[])
{
    uint64_t pid = getPID();

    while (1)
    {
        print("%s: %d ",argv[1], pid);;
        bussy_wait(MINOR_WAIT);
    }
}

#define TOTAL_PROCESSES 3

void test_prio()
{
    uint64_t pids[TOTAL_PROCESSES];
    uint64_t i;
    char buf[4] = {0};

    for (i = 0; i < TOTAL_PROCESSES; i++)
    {
        char *argv[] = {"Proceso Dummy", itoa(i, buf, 10)};
        pids[i] = createProcess(&endless_loop, 2, argv, 0, 0);
    }

    bussy_wait(WAIT);
    printStringLn("");
    printStringLn("CHANGING PRIORITIES...");

    for (i = 0; i < TOTAL_PROCESSES; i++)
    {
        switch (i % 3)
        {
        case 0:
            nice(pids[i], 1); //lowest priority
            break;
        case 1:
            nice(pids[i], 10); //medium priority
            break;
        case 2:
            nice(pids[i], 20); //highest priority
            break;
        }
    }

    bussy_wait(WAIT);
    bussy_wait(WAIT);
    bussy_wait(WAIT);
    printStringLn("");
    printStringLn("BLOCKING...");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        blockProcess(pids[i]);

    printStringLn("CHANGING PRIORITIES WHILE BLOCKED...\n");
    for (i = 0; i < TOTAL_PROCESSES; i++)
        nice(pids[i], 20); //medium priority

    printStringLn("UNBLOCKING...");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        unblockProcess(pids[i]);

    bussy_wait(WAIT);
    bussy_wait(WAIT);
    bussy_wait(WAIT);
    printStringLn("");
    printStringLn("KILLING...");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        killProcess(pids[i]);
    return;
}
