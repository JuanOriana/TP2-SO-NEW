// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <lib.h>
#include <stringLib.h>
#include <schedule.h>

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6
#define REGISTERS 15
#define EXCEPTION_STACK_BASE 14

static void zero_division();
static void invalid_op_code();
static void printRegisters(uint64_t *registers);

static char *regNames[] = {"R15: ", "R14: ", "R13: ", "R12: ", "R11: ", "R10: ", "R9: ",
                           "R8: ", "RSI: ", "RDI: ", "RBP: ", "RDX: ", "RCX: ", "RBX: ",
                           "RAX: "};

//Maneja la excepcion recibida y reinicia el proceso actual
void exceptionDispatcher(int exception, void *stackframe)
{
      putchar('\n');
      switch (exception)
      {
      case ZERO_EXCEPTION_ID:
            zero_division();
            break;

      case INVALID_OPCODE_ID:
            invalid_op_code();
            break;
      }
      printRegisters(stackframe);
      putchar('\n');
      killProcess(getCurrPID());
}

static void zero_division()
{
      printStringWC("Exception 0: division by zero is undefined\n", BLACK, RED);
}

static void invalid_op_code()
{
      printStringWC("Exception 6: invalid opcode\n", BLACK, RED);
}

static void printRegisters(uint64_t *registers)
{
      printStringWC("RIP: ", BLACK, RED);
      printHexWLC(registers[EXCEPTION_STACK_BASE + 1], 8, BLACK, RED); //print rip value from interrupt stackframe
      putchar('\n');
      printStringWC("RSP: ", BLACK, RED);
      printHexWLC(registers[EXCEPTION_STACK_BASE + 4], 8, BLACK, RED); //print rsp value from interrupt stackframe
      putchar('\n');
      for (int i = 0; i < REGISTERS; i++)
      {
            printStringWC(regNames[i], BLACK, RED);
            printHexWLC(registers[i], 8, BLACK, RED);
            putchar('\n');
      }
}