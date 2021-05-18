// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <semaphores.h>
#include <pipes.h>
#include <stringLib.h>

#define BUFF_SIZE 1024
#define PIPE_COUNT 12

#define IN_USE 1
#define EMPTY 0

typedef struct
{
    int id;
    char buffer[BUFF_SIZE];
    int readIndex;
    int writeIndex;
    long totalProcesses;
    int lockW;
    int lockR;
    int state;
} Pipe;

typedef struct Pipe
{
    Pipe pipes[PIPE_COUNT];
} PipeArray;

long semId = 420;
static PipeArray pipesArray;

static int putCharPipeByIdx(int pipeIndex, char c);
static int getPipeIdx(int pipeId);
static int getFreePipe();
static int newPipe(int pipeId);

int pOpen(int pipeId)
{
    int pipeIndex = getPipeIdx(pipeId);

    if (pipeIndex == -1)
    {
        pipeIndex = newPipe(pipeId);
        if (pipeIndex == -1)
        {
            return -1;
        }
    }

    pipesArray.pipes[pipeIndex].totalProcesses++;

    return pipeId;
}

int pClose(int pipeId)
{
    int pipeIndex = getPipeIdx(pipeId);

    if (pipeIndex == -1)
        return -1;

    Pipe *pipe = &pipesArray.pipes[pipeIndex];

    pipe->totalProcesses--;

    //Depleted pipe?
    if (pipe->totalProcesses > 0)
    {
        return 1;
    }

    sClose(pipe->lockR);
    sClose(pipe->lockW);
    pipe->state = EMPTY;

    return 1;
}

int pRead(int pipeId)
{
    int pipeIndex = getPipeIdx(pipeId);

    if (pipeIndex == -1)
        return -1;

    Pipe *pipe = &pipesArray.pipes[pipeIndex];

    sWait(pipe->lockR);

    char c = pipe->buffer[pipe->readIndex];
    pipe->readIndex = (pipe->readIndex + 1) % BUFF_SIZE;

    sPost(pipe->lockW);

    return c;
}

int pWrite(int pipeId, char *str)
{
    int pipeIndex = getPipeIdx(pipeId);

    if (pipeIndex == -1)
        return -1;

    while (*str != 0)
        putCharPipeByIdx(pipeIndex, *str++);

    return pipeId;
}

static int putCharPipeByIdx(int pipeIndex, char c)
{
    Pipe *pipe = &pipesArray.pipes[pipeIndex];

    sWait(pipe->lockW);

    pipe->buffer[pipe->writeIndex] = c;
    pipe->writeIndex = (pipe->writeIndex + 1) % BUFF_SIZE;

    sPost(pipe->lockR);

    return 0;
}

int putCharPipe(int pipeId, char c)
{
    int pipeIndex = getPipeIdx(pipeId);

    if (pipeIndex == -1)
        return -1;

    putCharPipeByIdx(pipeIndex, c);

    return pipeId;
}

static int newPipe(int pipeId)
{
    int newIdx = getFreePipe();

    if (newIdx == -1)
    {
        return -1;
    }

    Pipe *pipe = &pipesArray.pipes[newIdx];

    pipe->id = pipeId;
    pipe->state = IN_USE;
    pipe->readIndex = pipe->writeIndex = pipe->totalProcesses = 0;

    if ((pipe->lockR = sOpen(semId++, 0)) == -1)
    {
        return -1;
    }

    if ((pipe->lockW = sOpen(semId++, BUFF_SIZE)) == -1)
    {
        return -1;
    }

    return pipeId;
}

static int getPipeIdx(int pipeId)
{
    for (int i = 0; i < PIPE_COUNT; i++)
    {

        if (pipesArray.pipes[i].state == IN_USE && pipesArray.pipes[i].id == pipeId)
        {
            return i;
        }
    }
    return -1;
}

static int getFreePipe()
{
    for (int i = 0; i < PIPE_COUNT; i++)
    {
        if (pipesArray.pipes[i].state == EMPTY)
        {
            return i;
        }
    }
    return -1;
}

void dumpPipes()
{
    print("\PIPE DUMP\n");
    print("------------------------------------------------\n");
    print("Active pipes:\n");
    for (int i = 0; i < PIPE_COUNT; i++)
    {
        Pipe pipe = pipesArray.pipes[i];
        if (pipe.state == IN_USE)
        {
            print("-------------------------------\n");
            print("Pipe ID: %d\n", pipe.id);
            print("   Number of attached processes: %d\n", pipe.totalProcesses);
            print("   Read sem: %d\n", pipe.lockR);
            print("   Write sem: %d\n", pipe.lockW);
            print("   Chars in buff: %d\n", pipe.lockW);
            print("   Buffer content: ");
            for (int i = pipe.readIndex; i != pipe.writeIndex; i = (i + 1) % BUFF_SIZE)
                putchar(pipe.buffer[i]);
            print("\n");
        }
    }
    print("-------------------------------\n");
}
