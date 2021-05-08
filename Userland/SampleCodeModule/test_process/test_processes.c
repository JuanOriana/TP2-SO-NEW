// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <utils.h>
#include <test_util.h>
#include <processes.h>
#include <stringLib.h>

//TO BE INCLUDED
void endless_looP()
{
    while (1)
        ;
}

void process1(int argc, const char *argv[])
{
    while (1)
    {
        printStringLn(argv[1]);
    }
}

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

void test_processes()
{
    p_rq p_rqs[MAX_PROCESSES];
    uint8_t rq;
    uint8_t alive = 0;
    uint8_t action;

    while (1)
    {

        char buf[10] = {0};
        //Create MAX_PROCESSES processes
        for (rq = 0; rq < MAX_PROCESSES; rq++)
        {

            char *argv[] = {"Proceso Dummy", itoa(rq, buf, 10)};
            p_rqs[rq].pid = createProcess(&process1, 2, argv,1); //TODO: Port this call as required

            if (p_rqs[rq].pid == -1)
            {                                            //TODO: Port this as required
                printStringLn("Error creating process"); //TODO: Port this as required
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
                        {                                           //TODO: Port this as required
                            printStringLn("Error killing process"); // TODO: Port this as required
                            return;
                        }
                        p_rqs[rq].state = KILLED;
                        alive--;
                    }
                    break;

                case 1:
                    if (p_rqs[rq].state == RUNNING)
                    {
                        if (blockProcess(p_rqs[rq].pid) == -1)
                        {                                            // TODO: Port this as required
                            printStringLn("Error blocking process"); //  TODO: Port this as required
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
                    {                                              //TODO: Port this as required
                        printStringLn("Error unblocking process"); // TODO: Port this as required
                        return;
                    }
                    p_rqs[rq].state = RUNNING;
                }
        }
    }
}
