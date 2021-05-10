#include <processes.h>
#include <systemCalls.h>
#include <semLib.h>
#include <stringLib.h>
#include <utils.h>

#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID 101

int64_t global; //shared memory

void slowInc(int64_t *p, int64_t inc)
{
  int64_t aux = *p;
  aux += inc;
  yield();
  *p = aux;
}
//uint64_t sem, int64_t value, uint64_t N
void inc(int argc, char* argv[])
{
  uint64_t i;
  int flag = strToInt(argv[1],0);
  int value = strToInt(argv[2],0);
  int N = strToInt(argv[3],0);
  Semaphore* sem = sOpen(SEM_ID,value);

  if (flag && !sem)
  {
    print("ERROR OPENING SEM\n");
    return;
  }

  for (i = 0; i < N; i++)
  {
    if (flag)
      sWait(sem);
    slowInc(&global, value);
    if (flag)
      sPost(sem);
  }

  if (flag)
    sClose(sem);

  print("Final value: %d\n", global);
}

void test_sync()
{
  uint64_t i;

  global = 0;

  print("CREATING PROCESSES...(WITH SEM)\n");

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
  {
     char *argv1[4] = {"inc","1","1","1000000"};
    createProcess(&inc,3,argv1,1);
    char* argv2[4] = {"inc","1","-1","1000000"};
    createProcess(&inc, 3, argv2, 1);
  }
}

void test_no_sync()
{
  uint64_t i;

  global = 0;

  print("CREATING PROCESSES...(WITHOUT SEM)\n");

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
  {
    char *argv1[4] = {"inc","0","1","1000000"};
    createProcess(&inc,3,argv1,1);
    char* argv2[4] = {"inc","0","-1","1000000"};
    createProcess(&inc, 3, argv2, 1);
  }
}
