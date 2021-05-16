// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <commands.h>
#include <RTCTime.h>
#include <cpuInfo.h>
#include <lib.h>
#include <stringLib.h>
#include <systemCalls.h>
#include <processes.h>
#include <utils.h>
#include <test_util.h>
#include <loop.h>
#include <semLib.h>

#define EOF -1

static void memToString(char *buffer, uint8_t *mem, int bytes);

//devuelve el tiempo acutal del sistema
void time(int argc, char **args)
{
      if (argc != 1)
      {
            printStringLn("Invalid ammount of arguments.");
            putchar('\n');
            return;
      }

      char timeFormat[3][3];
      uint8_t hours = syscall(RTC_TIME, HOURS, 0, 0, 0, 0, 0);
      uint8_t mins = syscall(RTC_TIME, MINUTES, 0, 0, 0, 0, 0);
      uint8_t secs = syscall(RTC_TIME, SECONDS, 0, 0, 0, 0, 0);
      printString(" >Current time: ");
      intToStr(hours, timeFormat[0], 2);
      intToStr(mins, timeFormat[1], 2);
      intToStr(secs, timeFormat[2], 2);

      for (int i = 0; i < 3; i++)
      {
            printString(timeFormat[i]);
            if (i != 2)
            {
                  putchar(':');
            }
      }
      putchar('\n');
      putchar('\n');
}

//devuelve el modelo y vendedor del cpu
void cpuInfo(int argc, char **args)
{
      if (argc != 1)
      {
            printStringLn("Invalid ammount of arguments.");
            putchar('\n');
            return;
      }
      char vendor[13] = {0};
      t_cpuInfo cpuInfo = {vendor, 0};
      cpuVendor(cpuInfo.cpuVendor);
      cpuInfo.model = cpuModel();
      printString(" > CPU Vendor: ");
      printStringLn(cpuInfo.cpuVendor);
      printString(" > CPU model: ");
      printInt(cpuInfo.model);
      putchar('\n');
      putchar('\n');
}

//Hace un dump de 32 bytes de memria a partir de la direccion pedida
void printmem(int argc, char **args)
{
      if (argc != 2)
      {
            printStringLn("Invalid ammount of arguments.");
            putchar('\n');
            return;
      }

      int error = 0;
      uint64_t memDir = strToHex(args[1], &error);
      if (error)
      {
            printStringLn("Invalid argument for function printmem (must be a hex value).");
            putchar('\n');
            return;
      }

      int bytes = 32;

      uint8_t memData[bytes];
      syscall(GET_MEM, memDir, (uint64_t)memData, 0, 0, 0, 0);

      char byteStr[bytes * 2];
      memToString(byteStr, memData, bytes);

      printString(" >Data dump:");
      for (int i = 0; i < 32; i++)
      {
            if (i % 4 == 0)
            {
                  putchar('\n');
                  printString("   -[0x");
                  printHex(memDir);
                  printString("]: ");
                  memDir += 4;
            }
            if (i % 2 == 0)
            {
                  putcharWC(byteStr[i], BLACK, BLUE);
                  putcharWC(byteStr[i + 1], BLACK, BLUE);
                  putchar(' ');
            }
            else
            {
                  putchar(byteStr[i]);
                  putchar(byteStr[i + 1]);
                  putchar(' ');
            }
      }
      putchar('\n');
      putchar('\n');
}

//Imprime la temperatura actual del cpu
void cpuTemp(int argc, char **args)
{
      if (argc != 1)
      {
            printStringLn("Invalid ammount of arguments.");
            putchar('\n');
            return;
      }
      printString("CPU temp: ");
      printInt(syscall(TEMP, 0, 0, 0, 0, 0, 0));
      printStringLn(" C");
      putchar('\n');
}

//causa una excepcion de dividir por cero
void checkZeroException(int argc, char **args)
{
      if (argc != 1)
      {
            printStringLn("Invalid ammount of arguments.");
            putchar('\n');
            return;
      }
      int a = 0;
      int b = 20 / a;
      if (b == 0)
      {
      }
}

//causa una excepcion de tipo invalid opcode
void checkInvalidOpcodeException(int argc, char **args)
{
      if (argc != 1)
      {
            printStringLn("Invalid ammount of arguments.");
            putchar('\n');
            return;
      }
      __asm__("ud2"); // https://hjlebbink.github.io/x86doc/html/UD2.html
}

//Muestra los argumentos pasados al comando
void showArgs(int argc, char **args)
{
      for (int i = 0; i < argc && i < MAX_ARGS; i++)
      {
            printString("arg[");
            printInt(i);
            printString("]=");
            printStringLn(args[i]);
      }
      putchar('\n');
}

static void memToString(char *buffer, uint8_t *mem, int bytes)
{
      for (int i = 0; i < bytes * 2; i++)
      {
            if (mem[i] <= 0xF)
            {
                  buffer[i] = '0';
                  uintToBase(mem[i], buffer + i + 1, 16);
            }
            else
            {
                  uintToBase(mem[i], buffer + i, 16);
            }
      }
}

void ps(int argc, char **args)
{
      syscall(PS, 0, 0, 0, 0, 0, 0);
}

void mem(int argc, char **args)
{
      syscall(DUMP_MM, 0, 0, 0, 0, 0, 0);
}

void loopCommand(int argc, char **args)
{

      char *argv[] = {"Loop"};
      createProcess(&loop, 1, argv, 0, 0);
      return;
}

void testPriority(int argc, char **args)
{
      test_prio();
}

void testScheduler(int argc, char **args)
{
      test_processes();
}

void kill(int argc, char **args)
{
      if (argc != 2)
      {
            print("Error: Invalid ammount of arguments.\n");
            return;
      }
      int pid = strToInt(args[1]);
      killProcess(pid);
}

void niceProcess(int argc, char **args)
{
      if (argc != 3)
      {
            print("Error: Invalid ammount of arguments.\n");
            return;
      }
      int pid = strToInt(args[1]);
      int priority = strToInt(args[2]);
      nice(pid, priority);
}

void block(int argc, char **args)
{
      if (argc != 2)
      {
            print("Error: Invalid ammount of arguments.\n");
            return;
      }
      int pid = strToInt(args[1]);
      blockProcess(pid);
}

void unblock(int argc, char **args)
{
      if (argc != 2)
      {
            print("Error: Invalid ammount of arguments.\n");
            return;
      }
      int pid = strToInt(args[1]);
      unblockProcess(pid);
}

void sem(int argc, char **args)
{
      syscall(DUMP_SEM, 0, 0, 0, 0, 0, 0);
}

void cat(int argc, char **args)
{
      if (argc != 1)
      {
            print("Error: Invalid ammount of arguments.\n");
            return;
      }

      char buffer[BUFFER_SIZE] = {0};

      int c;
      int i;
      while ((c = getchar()) != EOF)
      {
            cleanString(buffer);
            i = 0;
            while (c != '\n')
            {
                  putchar(c);
                  buffer[i++] = c;
                  c = getchar();
            }

            print("\n%s\n", buffer);
      }
}

//counts the lines recieved from input
void wc(int argc, char **args)
{
      if (argc != 1)
      {
            print("Error: Invalid ammount of arguments.\n");
            return;
      }

      int c;
      int count = 1;

      while ((c = getchar()) != EOF)
      {
            putchar(c);
            if ((char)c == '\n')
                  count++;
      }
      print("\nNumber of lines: %d\n", count);
}

void filter(int argc, char **args)
{
      if (argc != 1)
      {
            print("Error: Invalid ammount of arguments.\n");
            return;
      }
      char buffer[BUFFER_SIZE] = {0};

      int c;
      int i;
      while ((c = getchar()) != EOF)
      {
            cleanString(buffer);
            i = 0;
            while (c != '\n')
            {
                  putchar(c);
                  if (!check_vowel(c))
                        buffer[i++] = c;
                  c = getchar();
            }

            print("\n%s\n", buffer);
      }
}