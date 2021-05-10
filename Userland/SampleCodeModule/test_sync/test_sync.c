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
  *p = aux;
}
//uint64_t sem, int64_t value, uint64_t N
void inc(int argc, char *argv[])
{
  uint64_t i;
  int flag = strToInt(argv[1], 0);
  int64_t value = strToInt(argv[2], 0);
  int N = strToInt(argv[3], 0);
  Semaphore *sem;

  if (flag == 1 && (sem = sOpen(SEM_ID, 1)) == 0)
  {
    print("ERROR OPENING SEM\n");
    return;
  }
  print("opening\n");
  for (i = 0; i < N; i++)
  {
    if (flag == 1 && sWait(sem) != 0)
      print("Error waiting sem\n");
    print("waiting\n");
    slowInc(&global, value);
    if (flag == 1 && sPost(sem) != 0)
      print("Error posting sem\n");
    print("posting\n");
  }

  if (flag == 1 && sClose(sem) != 0)
    print("Error closing sem\n");
  print("closing\n");

  print("Final value: %d\n", global);
}

void testSync(int argc, char *argv[])
{
  uint64_t i;

  global = 0;

  print("CREATING PROCESSES...(WITH SEM)\n");

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
  {
    char *argv1[4] = {"inc", "1", "1", "100000"};
    createProcess(&inc, 4, argv1, 0);
    char *argv2[4] = {"inc", "1", "-1", "100000"};
    createProcess(&inc, 4, argv2, 0);
  }
}

void testNoSync(int argc, char *argv[])
{
  uint64_t i;

  global = 0;

  print("CREATING PROCESSES...(WITHOUT SEM)\n");

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
  {
    char *argv1[4] = {"inc", "0", "1", "10"};
    createProcess(&inc, 4, argv1, 1);
    char *argv2[4] = {"inc", "0", "-1", "10"};
    createProcess(&inc, 4, argv2, 1);
  }
}
