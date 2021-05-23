// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <systemCalls.h>
#include <processes.h>
#include <semLib.h>
#include <lib.h>
#include <stringLib.h>
#include <utils.h>
#include <phylo.h>
//Tanenbaum's based implementation

#define MAX_PHILOS 10
#define BASE_PHILOS 2
#define MUTEX_ID 999
#define NULL 0

typedef enum
{
    THINKING = 1,
    EATING = 2,
    HUNGRY = 3,
} STATE;

typedef struct Philosopher
{
    int pid;
    int sem;
    STATE state;
} Philosopher;

Philosopher *philos[MAX_PHILOS];
static int actualPhilosopherCount = 0;
static int tableMutex;
static int problemRunning;
static int semId = 1000;

#define RIGHT(i) ((i) + 1) % (actualPhilosopherCount)                         /* number of i’s right neighbor */
#define LEFT(i) ((i) + actualPhilosopherCount - 1) % (actualPhilosopherCount) /* number of i’s left neighbor */

int getMyIdx(int pid)
{

    for (int i = 0; i < actualPhilosopherCount; i++)
    {
        if (philos[i]->pid == pid)
            return i;
    }
    return -1;
}

void philo(int argc, char *argv[])
{
    int myPid = getPID();
    while (problemRunning)
    {
        int idx = getMyIdx(myPid);
        takeForks(idx);
           sleep(1);
        placeForks(idx);
           sleep(1);
    }
}

void takeForks(int i)
{
    sWait(tableMutex);
    philos[i]->state = HUNGRY;
    test(i);
    sPost(tableMutex);
    sWait(philos[i]->sem);
}

void placeForks(int i)
{
    sWait(tableMutex);
    philos[i]->state = THINKING;
    test(LEFT(i));
    test(RIGHT(i));
    sPost(tableMutex);
}

void test(int i)
{
    if (philos[i]->state == HUNGRY && philos[LEFT(i)]->state != EATING && philos[RIGHT(i)]->state != EATING)
    {
        philos[i]->state = EATING;
        sPost(philos[i]->sem);
    }
}

void philosopherProblem(int argc, char *argv[])
{
    problemRunning = 1;
    tableMutex = sOpen(MUTEX_ID, 1);
    print("Welcome to the Philosophers Problem!\n");
    print("You start with 2 philosophers, and have maximum of 10 philosophers.\n");
    print("You can add them with \'a\', delete them with \'d\' and exit the problem with \'q\'.\n");
    print("The state of each will be displayed as E (Eating) or . (HUNGRY)\n\n");

    for (int i = 0; i < BASE_PHILOS; i++)
        addPhilosopher();
    char *args[] = {"PrintTable"};
    int printTablePid = createProcess(&printTable, 1, args, 0, 0);
    while (problemRunning)
    {

        char key = getchar();
        switch (key)
        {
        case 'a':
            if (addPhilosopher() == -1)
                print("Can\'t add another philosopher. Maximum 10 philosophers.\n");
            else
                print("A new philosopher joined!\n");
            break;
        case 'd':
            if (removePhilosopher() == -1)
                print("Can\'t remove another philosopher. Minimum 2 philosophers.\n");
            else
                print("One philosopher has left!\n");
            break;
        case 'q':
            print("Program Finished!\n");
            problemRunning = 0;
            break;
        default:
            break;
        }
    }

    for (int i = 0; i < actualPhilosopherCount; i++)
    {
        sClose(philos[i]->sem);
        killProcess(philos[i]->pid);
        freeCust(philos[i]);
    }
    actualPhilosopherCount = 0;
    semId = 1000;
    killProcess(printTablePid);
    sClose(MUTEX_ID);
}

int addPhilosopher()
{
    if (actualPhilosopherCount + 1 > MAX_PHILOS)
        return -1;

    sWait(tableMutex);
    Philosopher *auxPhilo = mallocCust(sizeof(Philosopher));
    if (auxPhilo == NULL)
        return -1;

    auxPhilo->state = THINKING;
    auxPhilo->sem = sOpen(semId++, 1);
    char *name[] = {"philo"};
    auxPhilo->pid = createProcess(&philo, 1, name, 0, 0);
    philos[actualPhilosopherCount++] = auxPhilo;
    sPost(tableMutex);
    return 0;
}

int removePhilosopher()
{
    if (actualPhilosopherCount == BASE_PHILOS)
    {
        return -1;
    }
    sWait(tableMutex);
    actualPhilosopherCount--;
    Philosopher *chosenPhilo = philos[actualPhilosopherCount];

    sClose(chosenPhilo->sem);
    killProcess(chosenPhilo->pid);
    freeCust(chosenPhilo);
    sPost(tableMutex);

    return 0;
}

void printTable(int argc, char *argv[])
{
    while (problemRunning)
    {
        sWait(tableMutex);
        for (int i = 0; i < actualPhilosopherCount; i++)
        {
            philos[i]->state == EATING ? putchar('E') : putchar('.');
            putchar(' ');
        }
        putchar('\n');
        sPost(tableMutex);
        yield();
        
    }
}
