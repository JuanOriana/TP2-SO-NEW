// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <utils.h>
#include <testUtil.h>
#include <stringLib.h>

#define MAX_PROCESSES 10 //Should be around 80% of the the processes handled by the kernel

enum State
{
    ERROR,
    RUNNING,
    BLOCKED,
    KILLED
};

typedef struct P_rq
{
    uint32_t pid;
    enum State state;
} p_rq;

void testProcesses()
{
    p_rq p_rqs[MAX_PROCESSES];
    uint8_t rq;
    uint8_t alive = 0;
    uint8_t action;

    while (1)
    {
        //Create MAX_PROCESSES processes
        for (rq = 0; rq < MAX_PROCESSES; rq++)
        {
            char *argv[] = {"endlessLoop"};
            p_rqs[rq].pid = createProcess(&endlessLoop, 1, argv,  BG, NULL);

            if (p_rqs[rq].pid == -1)
            {
                printStringLn("Error creating process");
                return;
            }
            else
            {
                p_rqs[rq].state = RUNNING;
                alive++;
            }
        }

        // Randomly kills, blocks or unblocks processes until every one has been killed
        while (alive > 0)
        {
            for (rq = 0; rq < MAX_PROCESSES; rq++)
            {
                action = GetUniform(2) % 2;
                switch (action)
                {
                case 0:
                    if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED)
                    {
                        if (killProcess(p_rqs[rq].pid) == -1)
                        {
                            printStringLn("Error killing process");
                            return;
                        }
                        p_rqs[rq].state = KILLED;
                        alive--;
                        busyWait(MAJOR_WAIT);
                    }
                    break;

                case 1:
                    if (p_rqs[rq].state == RUNNING)
                    {
                        if (blockProcess(p_rqs[rq].pid) == -1)
                        {
                            printStringLn("Error blocking process");
                            return;
                        }
                        p_rqs[rq].state = BLOCKED;
                    }
                    break;
                }
            }

            //  Randomly unblocks processes
            for (rq = 0; rq < MAX_PROCESSES; rq++)
                if (p_rqs[rq].state == BLOCKED && GetUniform(2) % 2)
                {
                    if (unblockProcess(p_rqs[rq].pid) == -1)
                    {
                        printStringLn("Error unblocking process");
                        return;
                    }
                    p_rqs[rq].state = RUNNING;
                }
        }
    }
}
