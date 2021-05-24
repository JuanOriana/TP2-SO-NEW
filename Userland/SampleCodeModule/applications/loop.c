// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "utils.h"
#include "stringLib.h"
#include "processes.h"

void loop(int argc, char **args)
{
    if (argc != 1)
    {
        print("Invalid ammount of arguments.\n");
        return;
    }

    long pid = getPID();

    while (1)
    {
        waitCycles(10);
        print("%d\n", pid);
    }
}
