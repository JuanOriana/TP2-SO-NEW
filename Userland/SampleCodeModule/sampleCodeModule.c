
#include <systemCalls.h>
#include <shell.h>
#include <processes.h>
#include <stringLib.h>

int main()
{

      char *args[] = {"Shell"};
      createProcess(&runShell, 1, args);

      return 0;
}