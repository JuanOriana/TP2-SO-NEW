// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <shell.h>
#include <commands.h>
#include <keys.h>
#include <lib.h>
#include <registers.h>
#include <stdint.h>
#include <stringLib.h>
#include <systemCalls.h>
#include <utils.h>
#include <loop.h>
#include <processes.h>
#include <loop.h>
#include <testMem.h>
#include <testSync.h>

static void initShell(t_shellData *shellData);
static void shellText(t_shellData *shellData);
static void processCommand(t_shellData *shellData);
static void processChar(char c, t_shellData *shellData);

static t_shellData shell;

static char *regNames[] = {"R15: ", "R14: ", "R13: ", "R12: ", "R11: ", "R10: ", "R9: ",
                           "R8: ", "RSI: ", "RDI: ", "RBP: ", "RDX: ", "RCX: ", "RBX: ",
                           "RAX: ", "RIP: ", "RSP: "};

void runShell()
{
      initShell(&shell);
      char c;
      while (1)
      {
            c = getchar();
            processChar(c, &shell);
      }
}

//inicia la shell y todas sus estructuras
static void initShell(t_shellData *shellData)
{
      t_command commandsData[] = {
          {&help, "help", "shows the list of commands and their use"},
          {&inforeg, "inforeg", "prints the value of all the registers on screen, press ctrl + s to update values"},
          {&printmem, "printmem", "recieves an hexadecimal direction and makes a memory dump of 32 bytes on screen"},
          {&time, "time", "prints the current system time on screen"},
          {&cpuInfo, "cpuInfo", "prints the processor brand and model on screen"},
          {&cpuTemp, "cpuTemp", "prints the current processor temperature on screen"},
          {&changeUsername, "changeUsername", "changes the shell prompt username"},
          {&checkZeroException, "checkZeroException", "triggers a zero division exception"},
          {&checkInvalidOpcodeException, "checkInvalidOpcodeException", "triggers an invalid opcode exception"},
          {&showArgs, "showArgs", "prints the arguments passed to this command"},
          {&ps, "ps", "displays information about a selection of the active processes"},
          {&mem, "mem", "prints the current state of memory"},
          {&testPriority, "priotest", "tests the implementation of the priority manager in the system"},
          {&testScheduler, "schedtest", "tests the implementation of the scheduler in the system"},
          {&testMem, "memtest", "tests the implementation of the memory manager in the system"},
          {&testSync, "synctest", "tests the implementation of the sync manager in the system"},
          {&testNoSync, "nosynctest", "tests the implementation of the sync manager (withour sem usage) in the system"},
          {&loop, "loop", "loops and prints his pid endlessly"},
          {&kill, "kill", "kills process"},
          {&niceProcess, "nice", "changes process priority"},
          {&block, "block", "blocks process"},
          {&unblock, "unblock", "unblocks process"},
          {&sem, "sem", "prints current state of active semaphores"},
          {&cat, "cat", "prints standard input in standard output"},
          {&wc, "wc", "counts the number of input lines"},
          {&filter, "filter", "filter the vowels of the input"}};

      for (int i = 0; i < COMMANDS; i++)
      {
            shellData->commands[i].command = commandsData[i].command;
            shellData->commands[i].name = commandsData[i].name;
            shellData->commands[i].description = commandsData[i].description;
      }
      char *username = "USER";
      cleanBuffer(&shellData->buffer);
      strcopy(username, shellData->username);
      shellText(shellData);
}

//procesa el caracter recibido actua segun el mismo
static void processChar(char c, t_shellData *shellData)
{
      if (c != 0)
      {
            switch (c)
            {
            case CLEAR_SCREEN:
                  syscall(CLEAR, 0, 0, 0, 0, 0, 0);
                  cleanBuffer(&shellData->buffer);
                  shellText(shellData);
                  break;
            case '\n':
                  putchar('\n');
                  processCommand(shellData);
                  cleanBuffer(&shellData->buffer);
                  shellText(shellData);
                  break;
            case '\b':
                  if (shellData->buffer.index > 0)
                  {
                        shellData->buffer.buffer[--shellData->buffer.index] = 0;
                        deletechar();
                  }
                  break;

            default:
                  if (shellData->buffer.index < BUFFER_SIZE)
                  {
                        shellData->buffer.buffer[shellData->buffer.index++] = c;
                        putchar(c);
                  }
            }
      }
}

//procesa el comando, tokenizando lo ingresado.
static void processCommand(t_shellData *shellData)
{

      int argc = 0;
      char *argv[MAX_ARGS] = {0};
      int fg = 1;
      argc = tokenizeBuffer(' ', argv, shellData->buffer.buffer, MAX_ARGS);

      if (argv[argc - 1][0] == '&')
      {
            fg = 0;
            argc--;
      }

      int idx = -1;

      for (int i = 0; i < COMMANDS; i++)
      {
            if (stringcmp(shellData->commands[i].name, argv[0]) == 0)
            {
                  idx = i;
            }
      }
      if (idx == -1)
      {
            printStringLn("Invalid command");
            return;
      }
      createProcess(shellData->commands[idx].command, argc, argv, fg, 0);
}

//muestra en pantalla el texto de la shell
static void shellText(t_shellData *shellData)
{
      printStringWC(shellData->username, BLACK, LIGHT_BLUE);
      printStringWC(" $ > ", BLACK, LIGHT_BLUE);
}

//muestra la informacion recolectada sobre los registros obtenidos al haber presionado ctrl + s
void inforeg(int argc, char **args)
{
      if (argc != 1)
      {
            printStringLn("Invalid ammount of arguments.");
            putchar('\n');
            return;
      }
      uint64_t *regData = (uint64_t *)syscall(INFOREG, 0, 0, 0, 0, 0, 0);
      for (int i = 0; i < REGISTERS; i++)
      {
            printString(" > ");
            printString(regNames[i]);
            printHexWL(regData[i], 8);
            putchar('\n');
      }
      putchar('\n');
}

//cambia el nombre del usuario mostrado en la shell
void changeUsername(int argc, char **argv)
{
      if (argc != 2)
      {
            printStringLn("Invalid ammount of arguments.");
            putchar('\n');
            return;
      }
      cleanString(shell.username);
      strcopy(argv[1], shell.username);
}

//muestra la lista de comandos con sus descripciones
void help(int argc, char **args)
{
      if (argc != 1)
      {
            printStringLn("Invalid amount of arguments.");
            putchar('\n');
            return;
      }

      printStringLn("These shell commands are defined internally.  Type 'help' to see this list.");
      for (int i = 0; i < COMMANDS; i++)
      {
            printString(" >");
            printString(shell.commands[i].name);
            printString(": ");
            printStringLn(shell.commands[i].description);
      }
      putchar('\n');
}
