#include <phylo.h>
#include <systemCalls.h>
#include <processes.h>
#include <semLib.h>
#include <lib.h>
#include <stringLib.h>
#include <utils.h>
#include <phylo.h>
//Tanenbaum's based implementation

#define MAX_PHiLOS 10
#define BASE_PHiLOS 5
#define MUTEX_ID 1
#define NULL 0


typedef enum
{
    THINKING = 1,
    EATING = 2,
    HUNGRY = 3,
} STATE;

typedef struct Philosopher
{
    int state;
    int pid;
    int sem;
    int idx;
    STATE philoState;
} Philosopher;

Philosopher *philos[MAX_PHiLOS];
static int actualPhilosopherCount = 0;
static int tableMutex;
static int problemRunning;
static int semId = 1000;


#define RIGHT(i) (i + 1) % (actualPhilosopherCount)                         /* number of i’s right neighbor */
#define LEFT(i) (i + actualPhilosopherCount - 1) % (actualPhilosopherCount) /* number of i’s left neighbor */


void philo(int argc, char *argv[])
{
    sWait(tableMutex);
    int idx = strToInt(argv[1]);
    sPost(tableMutex);

    while (problemRunning)
    {
        for (int i = 0; i < 1000000; i++)
            ;
        takeForks(idx);
        for (int i = 0; i < 1000000; i++)
            ;
        placeForks(idx);
    }
}

void takeForks(int i)
{
    sWait(tableMutex);
    philos[i]->state = HUNGRY;
    test(i);
    sPost(tableMutex);
    if(philos[i]->state == HUNGRY)
        sWait(philos[i]->sem);
}

void placeForks(int i)
{
    sWait(tableMutex);
    philos[i]->state = THINKING;
    //Now my left and right philos can use my forks
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
    tableMutex = sOpen(MUTEX_ID,1);
    print("Welcome to the Philosophers Problem!\n");
    print("You start with 5 philosophers, and have a minimum of 5 and maximum of 10 philosophers.\n");
    print("You can add them with \'a\', delete them with \'d\' and exit the problem with \'q\'.\n");
    print("The state of each will be displayed as E (Eating) or . (HUNGRY)\n\n");

    for (int i = 0; i < BASE_PHiLOS; i++)
        addPhilosopher();

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
                print("Can\'t remove another philosopher. Minimum 5 philosophers.\n");
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
        printTable();
    }

    for (int i = 0; i < actualPhilosopherCount; i++)
    {
        sClose(philos[i]->sem);
        killProcess(philos[i]->pid);
        freeCust(philos[i]);
    }
    sClose(MUTEX_ID);
}

int addPhilosopher()
{
    if (actualPhilosopherCount + 1 > MAX_PHiLOS)
        return -1;

    sWait(tableMutex);
    Philosopher *auxPhilo = mallocCust(sizeof(Philosopher));
    if(auxPhilo == NULL)
        return -1;

    auxPhilo->state = THINKING;
    auxPhilo->sem = sOpen(semId++, 1);
    char buf[4] = {0};
    char *name[] = {"philo", itoa(actualPhilosopherCount, buf, 10)};
    auxPhilo->pid = createProcess(&philo, 2, name, 0, 0);
    philos[actualPhilosopherCount++] = auxPhilo;
    sPost(tableMutex);
    return 0;
}

int removePhilosopher()
{
    if (actualPhilosopherCount == BASE_PHiLOS)
    {
        return -1;
    }
    sWait(tableMutex);
    actualPhilosopherCount--;
    Philosopher *lastPhilo = philos[actualPhilosopherCount];
    sClose(lastPhilo->sem);
    killProcess(lastPhilo->pid);
    freeCust(lastPhilo);
    philos[actualPhilosopherCount] = 0;
    sPost(tableMutex);
    return 0;
}

void printTable()
{
    for(int i=0; i<actualPhilosopherCount;i++){
            philos[i]->state == EATING ? putchar('E') : putchar('.');
        }
    putchar('\n');
}