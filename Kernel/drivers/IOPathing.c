#include <IOPathing.h>
#include <schedule.h>
#include <stringLib.h>
#include <keyboardDriver.h>
#include <pipes.h>

void pathPrint(char *str, uint8_t length, t_colour bgColour, t_colour fontColour)
{
    int outFd = currentWritesTo();

    if (outFd == 1)
    {
        sys_write(str, length, bgColour, fontColour);
    }
    else
    {
        pWrite(outFd, str);
    }
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
    return pRead(inFd);
}