
#include <systemCalls.h>
#include <shell.h>
#include <processes.h>
#include <stringLib.h>

int main()
{

      char *args[] = {"Shell"};
      createProcess(&runShell, 1, args);
      syscall(DUMP_MM, 0, 0, 0, 0, 0, 0);
      return 0;
}