
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
    int arrLock;
} PipeArray;

long semId = 420;
static PipeArray pipes;

int initPipes()
{
    //Init lock for PipeArray
    if ((pipes.arrLock = sOpen(semId++, 1)) == -1)
    {
        print("Error initing pipes\n");
        return -1;
    }
    return 0;
}

int pOpen(int pipeId)
{
    sWait(pipes.arrLock);

    int pipeIndex = getPipeIdx(pipeId);

    if (pipeIndex == -1)
    {
        pipeIndex = newPipe(pipeId);
        if (pipeIndex == -1)
        {
            sPost(pipes.arrLock);
            return -1;
        }
    }

    pipes.pipes[pipeIndex].totalProcesses++;

    sPost(pipes.arrLock);

    return pipeId;
}

int pClose(int pipeId)
{
    int pipeIndex = getPipeIdx(pipeId);

    if (pipeIndex == -1)
        return -1;

    sWait(pipes.arrLock);

    Pipe *pipe = &pipes.pipes[pipeIndex];

    pipe->totalProcesses--;

    //Depleted pipe?
    if (pipe->totalProcesses > 0)
    {
        sPost(pipes.arrLock);
        return 1;
    }

    sClose(pipe->lockR);
    sClose(pipe->lockW);
    pipe->state = EMPTY;

    sPost(pipes.arrLock);

    return 1;
}

int pRead(int pipeId)
{
    int pipeIndex = getPipeIdx(pipeId);

    if (pipeIndex == -1)
        return -1;

    Pipe *pipe = &pipes.pipes[pipeIndex];

    sWait(pipe->lockR);

    char c = pipe->buffer[pipe->readIndex];
    pipe->readIndex = (pipe->readIndex + 1) % BUFF_SIZE;

    sem_post(pipe->lockR);

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

int putCharPipeByIdx(int pipeIndex, char c)
{
    Pipe *pipe = &pipes.pipes[pipeIndex];

    sWait(pipe->lockR);

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

    Pipe *pipe = &pipes.pipes[newIdx];

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

        if (pipes.pipes[i].state == IN_USE && pipes.pipes[i].id == pipeId)
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
        if (pipes.pipes[i].state == EMPTY)
        {
            return i;
        }
    }
    return -1;
}