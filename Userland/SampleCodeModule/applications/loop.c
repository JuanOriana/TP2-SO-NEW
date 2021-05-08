#include "utils.h"
#include "stringLib.h"
#include "processes.h"

void loop(int argc, const char **args)
{
    if (argc != 1)
    {
        printString("Invalid ammount of arguments.\n");
        return;
    }

    long pid = getPID();

    while (1)
    {
        waitCycles(10);
        printInt(pid);
        printString("\n");
    }
}
