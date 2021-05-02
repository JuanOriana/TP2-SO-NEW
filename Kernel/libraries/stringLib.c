// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stringLib.h>
#include <videoDriver.h>
#include <utils.h>
#include <stdarg.h>

static int scan(char const *fmt, va_list arg);

void sys_write(char *string, uint8_t length, t_colour bgColour, t_colour fontColour)
{
      if (length <= 0 || string == 0)
      {
            return;
      }

      for (int i = 0; i < length && string[i] != 0; i++)
      {
            if (string[i] == '\n')
            {
                  changeLineOnScreen();
            }
            else if (string[i] == '\b')
            {
                  removeCharFromScreen();
            }
            else
            {
                  printCharOnScreen(string[i], bgColour, fontColour, 1);
            }
      }
}

void sys_staticwrite(char *string, uint8_t length, t_colour bgColour, t_colour fontColour)
{
      if (length <= 0 || string == 0)
      {
            return;
      }

      for (int i = 0; i < length && string[i] != 0; i++)
      {
            printCharOnScreen(string[i], bgColour, fontColour, 0);
      }
}

void printString(char *str)
{
      sys_write(str, strlen(str), BLACK, WHITE);
}

void printStringWC(char *str, t_colour bgColour, t_colour fontColour)
{
      sys_write(str, strlen(str), bgColour, fontColour);
}

void printStringLn(char *str)
{
      printString(str);
      putchar('\n');
}

void putchar(char c)
{
      sys_write(&c, 1, BLACK, WHITE);
}

void printHex(uint64_t num)
{
      char buffer[10];
      uintToBase(num, buffer, 16);
      printString(buffer);
}

void printHexWLC(uint64_t num, int length, t_colour bgColour, t_colour fontColour)
{
      char buffer[10];
      uintToBaseWL(num, buffer, 16, 8);
      printStringWC(buffer, bgColour, fontColour);
}

void printHexWC(uint64_t num, t_colour bgColour, t_colour fontColour)
{
      char buffer[10];
      uintToBase(num, buffer, 16);
      printStringWC(buffer, bgColour, fontColour);
}

void printInt(uint64_t num)
{
      char buffer[10];
      uintToBase(num, buffer, 10);
      printString(buffer);
}

void clear()
{
      clearScreen();
}

void staticputchar(char c)
{
      sys_staticwrite(&c, 1, BLACK, WHITE);
}

int print(char const *fmt, ...) {
      va_list arg;
      int length;

      va_start(arg, fmt);
      length = scan(fmt, arg);
      va_end(arg);
      return length;
}

static int scan(char const *fmt, va_list arg) {
      int int_temp;
      char char_temp;
      char *string_temp;

      char ch;
      int length = 0;

      char buffer[512];

      while ((ch = *fmt++)) {
            if ('%' == ch) {
                  switch (ch = *fmt++) {
                        /* %% - print out a single %    */
                        case '%':
                              putchar('%');
                              length++;
                              break;

                        /* %c: print out a character    */
                        case 'c':
                              char_temp = va_arg(arg, int);
                              putchar(char_temp);
                              length++;
                              break;

                        /* %s: print out a string       */
                        case 's':
                              string_temp = va_arg(arg, char *);
                              printString(string_temp);
                              length += strlen(string_temp);
                              break;

                        /* %d: print out an int         */
                        case 'd':
                              int_temp = va_arg(arg, int);
                              itoa(int_temp, buffer, 10);
                              printString(buffer);
                              length += strlen(buffer);
                              break;

                        /* %x: print out an int in hex  */
                        case 'x':
                              int_temp = va_arg(arg, int);
                              itoa(int_temp, buffer, 16);
                              printString("0x");
                              printString(buffer);
                              length += strlen(buffer);
                              break;
                  }
            } else {
                  putchar(ch);
                  length++;
            }
      }
      return length;
}