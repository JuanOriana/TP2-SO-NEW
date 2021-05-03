#include <schedule.h>
#include <stdint.h>
#include <stringLib.h>
#include <utils.h>

#define MINOR_WAIT 1000000 // TODO: To prevent a process from flooding the screen
#define WAIT 10000000      // TODO: Long enough to see theese processes beeing run at least twice

int process1(int argc, char *argv[])
{
  while (1)
  {
    printStringWC(argv[1], argv[1] == "2"?  BLACK : BLUE, argv[1] == "1"? RED:GREEN);
    printInt(getCurrPID());
  }
  return 0;
}

void bussy_wait(uint64_t n)
{
  uint64_t i;
  for (i = 0; i < n; i++)
    ;
}

void endless_loop()
{
  uint64_t pid = getCurrPID();

  while (1)
  {
    printInt(pid);
    bussy_wait(MINOR_WAIT);
  }
}

#define TOTAL_PROCESSES 3

void test_prio()
{
  uint64_t pids[TOTAL_PROCESSES];
  uint64_t i;
  char* buf[1] = {0};

  for (i = 0; i < TOTAL_PROCESSES; i++)
  {
    char *argv[] = {{"Proceso Dummy"},{itoa(i,buf,10)}};
    pids[i] = addProcess(&process1, 2, argv);
  }

  bussy_wait(WAIT);
  printStringLn("");
  printStringLn("CHANGING PRIORITIES...");


  for (i = 0; i < TOTAL_PROCESSES; i++)
  {
    switch (i % 3)
    {
    case 0:
      setNewCycle(pids[i], 0); //lowest priority
      break;
    case 1:
      setNewCycle(pids[i], 1); //medium priority
      break;
    case 2:
      setNewCycle(pids[i], 2); //highest priority
      break;
    }
  }

  bussy_wait(WAIT);
  printStringLn("");
  printStringLn("BLOCKING...");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    blockProcess(pids[i]);

  printf("CHANGING PRIORITIES WHILE BLOCKED...\n");
  for (i = 0; i < TOTAL_PROCESSES; i++)
  {
    switch (i % 3)
    {
    case 0:
      setNewCycle(pids[i], 1); //medium priority
      break;
    case 1:
      setNewCycle(pids[i], 1); //medium priority
      break;
    case 2:
      setNewCycle(pids[i], 1); //medium priority
      break;
    }
  }

  print("UNBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    unblockProcess(pids[i]);

  bussy_wait(WAIT);
  printStringLn("");
  printStringLn("KILLING...");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    killProcess(pids[i]);
}
