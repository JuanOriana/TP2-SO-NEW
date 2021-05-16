// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
void inc(int argc, char *argv[])
{
  uint64_t i;
  int flag = strToInt(argv[1]);
  int64_t value = strToInt(argv[2]);
  int N = strToInt(argv[3]);

  if (flag && sOpen(SEM_ID, 1) == -1)
  {
    print("ERROR OPENING SEM\n");
    return;
  }
  for (i = 0; i < N; i++)
  {
    if (flag && sWait(SEM_ID) != 0)
      print("Error waiting sem\n");
    slowInc(&global, value);
    if (flag && sPost(SEM_ID) != 0)
      print("Error posting SEM_ID\n");
  }

  if (flag && sClose(SEM_ID) != 0)
    print("Error closing sem\n");

  print("Final value: %d\n", global);
}

void testSync(int argc, char *argv[])
{
  uint64_t i;

  global = 0;

  print("CREATING PROCESSES...(WITH SEM)\n");

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
  {
    char *argv1[4] = {"inc", "1", "1", "100"};
    createProcess(&inc, 4, argv1, 0, 0);
    char *argv2[4] = {"inc", "1", "-1", "100"};
    createProcess(&inc, 4, argv2, 0, 0);
  }
}

void testNoSync(int argc, char *argv[])
{
  uint64_t i;

  global = 0;

  print("CREATING PROCESSES...(WITHOUT SEM)\n");

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
  {
    char *argv1[4] = {"inc", "0", "1", "100"};
    createProcess(&inc, 4, argv1, 0, 0);
    char *argv2[4] = {"inc", "0", "-1", "100"};
    createProcess(&inc, 4, argv2, 0, 0);
  }
}
