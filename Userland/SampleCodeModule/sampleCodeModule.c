// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <shell.h>
#include <processes.h>

int main()
{
      char *argv[] = {"Shell"};
      createProcess(runShell, 1, argv, 1, 0);
      return 0;
}