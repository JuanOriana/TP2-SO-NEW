#include <IOPathing.h>
#include <schedule.h>
#include <stringLib.h>
#include <keyboardDriver.h>
#include <pipes.h>

void pathPrint(char *string, uint8_t length, t_colour bgColour, t_colour fontColour)
{
    int outFd = currentWritesTo();

    if (outFd == 1)
        sys_write(string, length, bgColour, fontColour);
    else
        print("Writing string %s into fd %d\n", string, outFd);
}

int pathGetChar()
{
    int inFd = currentReadsFrom();

    if (inFd == 0)
    {
        if (currentProcessFg() == 1)
            return getchar();
        else
            return -1;
    }
    return 'P';
}