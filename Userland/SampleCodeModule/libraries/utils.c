// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <buffer.h>
#include <stringLib.h>
#include <utils.h>
#include <systemCalls.h>

//sacada de nvconsole
uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base)
{
      char *p = buffer;
      char *p1, *p2;
      uint32_t digits = 0;

      //Calculate characters for each digit
      do
      {
            uint32_t remainder = value % base;
            *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
            digits++;
      } while (value /= base);

      // Terminate string in buffer.
      *p = 0;

      //Reverse string in buffer.
      p1 = buffer;
      p2 = p - 1;
      while (p1 < p2)
      {
            char tmp = *p1;
            *p1 = *p2;
            *p2 = tmp;
            p1++;
            p2--;
      }

      return digits;
}

//sacada de nvconsole
uint32_t uintToBaseWL(uint64_t value, char *buffer, uint32_t base, uint32_t lenght)
{
      char *p = buffer;
      char *p1, *p2;
      uint32_t digits = 0;

      //Calculate characters for each digit
      do
      {
            uint32_t remainder = value % base;
            *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
            digits++;
      } while (value /= base);

      while (digits < lenght)
      {
            *p++ = '0';
            digits++;
      }

      // Terminate string in buffer.
      *p = 0;

      //Reverse string in buffer.
      p1 = buffer;
      p2 = p - 1;
      while (p1 < p2)
      {
            char tmp = *p1;
            *p1 = *p2;
            *p2 = tmp;
            p1++;
            p2--;
      }

      return digits;
}

uint64_t strToHex(char *str, int *error)
{
      uint64_t num = 0, val;
      *error = 0;
      int len = strlen(str);
      len--;
      for (int i = 0; str[i] != 0; i++)
      {
            if (str[i] >= '0' && str[i] <= '9')
            {
                  val = str[i] - '0';
            }
            else if (str[i] >= 'a' && str[i] <= 'f')
            {
                  val = str[i] - 'a' + 10;
            }
            else if (str[i] >= 'A' && str[i] <= 'F')
            {
                  val = str[i] - 'A' + 10;
            }
            else
            {
                  *error = 1;
                  return 0;
            }

            num += val * pow(16, len);
            len--;
      }
      return num;
}

uint64_t pow(uint64_t x, uint64_t y)
{
      if (y == 0)
            return 1;
      int aux = x;
      for (int i = 1; i < y; i++)
      {
            x *= aux;
      }
      return x;
}

uint8_t BSDToInt(uint8_t num)
{
      uint8_t low = num & 0x0F;
      uint8_t high = num & 0xF0;
      high >>= 4;
      return high * 10 + low;
}

// first call: string to parse, result for first parsing and delimiter
// next calls: call with NULL as string
// if no more tokens are to return, NULL is returned
// to reset, call with result = NULL
char *strtok(char *string, char *result, const char delim)
{
      static int currentIndex = 0;
      static char ogString[BUFFER_SIZE] = {0};

      if (result == 0)
      {
            currentIndex = 0;
            for (int i = 0; i < BUFFER_SIZE && ogString[i] != 0; i++)
            {
                  ogString[i] = 0;
            }
            return 0;
      }

      for (int i = 0; i < BUFFER_SIZE && result[i] != 0; i++)
      {
            result[i] = 0;
      }

      if (string != 0)
      {
            for (int i = 0; i < BUFFER_SIZE && string[i] != 0; i++)
            {
                  ogString[i] = string[i];
            }
      }
      else
      {
            if (currentIndex == BUFFER_SIZE || ogString[currentIndex] == 0)
            {
                  return 0;
            }
            for (; currentIndex < BUFFER_SIZE && ogString[currentIndex] != 0 && ogString[currentIndex] == delim; currentIndex++)
                  ;
            if (currentIndex == BUFFER_SIZE || ogString[currentIndex] == 0)
            {
                  return 0;
            }
            for (int i = 0; ogString[currentIndex] != 0 && ogString[currentIndex] != delim; currentIndex++, i++)
            {
                  result[i] = ogString[currentIndex];
            }
      }
      return result;
}

int64_t strToInt(char *str)
{
      int number = 0;
      int mult = 1;
      int n = strlen(str);
      n = (int)n < 0 ? -n : n; /* quick absolute value check  */
      /* for each character in array */
      while (n--)
      {
            /* if not digit or '-', check if number > 0, break or continue */
            if ((str[n] < '0' || str[n] > '9') && str[n] != '-')
            {
                  if (number)
                        break;
                  else
                        continue;
            }

            if (str[n] == '-')
            { /* if '-' if number, negate, break */
                  if (number)
                  {
                        number = -number;
                        break;
                  }
            }
            else
            { /* convert digit to numeric value   */
                  number += (str[n] - '0') * mult;
                  mult *= 10;
            }
      }

      return number;
}

// int main(int argc, const char** args) {
//   if(argc <= 1) return 0;

//   const char* input = args[1];
//   int total = 0;
//   int len = strlen(input);

//   for(int i = len-1; i+1 > 0; i--) {
//     int power = len - i - 1;
//     total += pow(10, power) * intFromAscii(input[i]);
//   }

//   if(input[0] == '-') total = total * -1;

//   return total;
// }

uint8_t stringcmp(char *str1, char *str2)
{
      int i = 0;
      while (str1[i] != 0 && str2[i] != 0 && str1[i] == str2[i])
      {
            i++;
      }
      if (str1[i] == str2[i])
      {
            return 0;
      }
      return str1[i] < str2[i] ? -1 : 1;
}

void cleanBuffer(t_buffer *buffer)
{
      for (int i = 0; i < BUFFER_SIZE; i++)
      {
            buffer->buffer[i] = 0;
      }
      buffer->index = 0;
}

void cleanString(char *str)
{
      for (int i = 0; str[i] != 0; i++)
      {
            str[i] = 0;
      }
}

int strlen(char *str)
{
      int size = 0;
      for (int i = 0; str[i] != 0; i++)
      {
            size++;
      }
      return size;
}

void *memcpy(void *destination, const void *source, uint64_t length)
{
      /*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
      uint64_t i;

      if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
          (uint64_t)source % sizeof(uint32_t) == 0 &&
          length % sizeof(uint32_t) == 0)
      {
            uint32_t *d = (uint32_t *)destination;
            const uint32_t *s = (const uint32_t *)source;

            for (i = 0; i < length / sizeof(uint32_t); i++)
                  d[i] = s[i];
      }
      else
      {
            uint8_t *d = (uint8_t *)destination;
            const uint8_t *s = (const uint8_t *)source;

            for (i = 0; i < length; i++)
                  d[i] = s[i];
      }

      return destination;
}

int isNum(char *str)
{
      int index = 0;
      if (str[index] == '.')
      {
            return 0;
      }
      if (str[index] == '-')
      {
            index++;
      }
      if (!IS_DIGIT(str[index]))
      {
            return 0;
      }
      for (; str[index] != 0 && str[index] != '.'; index++)
      {
            if (!IS_DIGIT(str[index]))
            {
                  return 0;
            }
      }
      if (str[index] == '.')
      {
            index++;
            for (; str[index] != 0; index++)
            {
                  if (!IS_DIGIT(str[index]))
                  {
                        return 0;
                  }
            }
      }
      return 1;
}

void strToDouble(char *numStr, int *error, double *result)
{
      *result = 0;
      int i = 0, k, sign = 0;
      double commaOffset = 0;
      char integerPart[BUFFER_SIZE] = {0};

      if (numStr[i] == '-')
      {
            sign = 1;
            i++;
      }

      for (k = 0; numStr[i] != 0 && numStr[i] != '.'; i++, k++)
      {
            integerPart[k] = numStr[i];
      }
      *result += strToInt(integerPart);
      if (numStr[i] == '.')
      {
            i++;
            for (; numStr[i] != 0; i++, commaOffset++)
            {
                  *result *= 10;
                  *result += numStr[i] - '0';
            }
            *result /= pow(10, commaOffset);
      }
      if (sign)
      {
            *result *= -1;
      }
}

// https://www.geeksforgeeks.org/convert-floating-point-number-string/
void doubleToString(char *res, double total, int afterpoint)
{
      int sign = 0;
      if (total < 0)
      {
            res[0] = '-';
            sign = 1;
            total *= -1;
      }
      // Extract integer part
      int ipart = (int)total;

      // Extract floating part
      float fpart = total - (double)ipart;

      // convert integer part to string
      int i = intToStr(ipart, res + sign, 1);

      // check for display option after point
      if (afterpoint != 0)
      {
            res[i + sign] = '.'; // add dot

            // Get the value of fraction part upto given no.
            // of points after dot. The third parameter
            // is needed to handle cases like 233.007
            fpart = fpart * pow(10, afterpoint);

            intToStr((int)fpart, res + sign + i + 1, afterpoint);
      }
}
// https://www.geeksforgeeks.org/convert-floating-point-number-string/
void reverse(char *str, int len)
{
      int i = 0, j = len, temp;
      while (i < j)
      {
            temp = str[i];
            str[i] = str[j];
            str[j] = temp;
            i++;
            j--;
      }
}
// https://www.geeksforgeeks.org/convert-floating-point-number-string/
// Converts a given integer x to string str[].
// d is the number of digits required in the output.
// If d is more than the number of digits in x,
// then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
      int i = 0;
      while (x)
      {
            str[i++] = (x % 10) + '0';
            x = x / 10;
      }

      // If number of digits required is more, then
      // add 0s at the beginning
      while (i < d)
            str[i++] = '0';

      reverse(str, i);
      str[i] = '\0';
      return i;
}

char *itoa(int value, char *buffer, int base)
{
      // invalid input
      if (base < 2 || base > 32)
            return buffer;

      // consider absolute value of number
      int n = ABS(value);

      int i = 0;
      while (n)
      {
            int r = n % base;

            if (r >= 10)
                  buffer[i++] = 65 + (r - 10);
            else
                  buffer[i++] = 48 + r;

            n = n / base;
      }

      // if number is 0
      if (i == 0)
            buffer[i++] = '0';

      // If base is 10 and value is negative, the resulting string
      // is preceded with a minus sign (-)
      // With any other base, value is always considered unsigned
      if (value < 0 && base == 10)
            buffer[i++] = '-';

      buffer[i] = '\0'; // null terminate string

      // reverse the string and return it
      reverse(buffer, i - 1);
      return buffer;
}

int ticksElapsed()
{
      return syscall(TICKS_ELAPSED, 0, 0, 0, 0, 0, 0);
}

void waitCycles(int cycles)
{
      int goal = ticksElapsed() + cycles;
      while (ticksElapsed() <= goal)
            ;
}

int tokenizeBuffer(char token, char **dest, char *source, int max)
{
      int index = 0;

      if (*source != token && *source != '\0')
            dest[index++] = source;

      while (*source != '\0')
      {
            if (*source == token)
            {
                  *source = 0;
                  if (*(source + 1) != token && (*(source + 1) != '\0'))
                  {
                        if (index >= max)
                              return index;
                        dest[index++] = source + 1;
                  }
            }
            source++;
      }
      return index;
}

void *memoryset(void *b, int c, int len)
{
      unsigned char *p = b;
      while (len > 0)
      {
            *p = c;
            p++;
            len--;
      }
      return (b);
}

int check_vowel(char a)
{
      if (a >= 'A' && a <= 'Z')
            a = a + 'a' - 'A'; /* Converting to lower case or use a = a + 32 */

      if (a == 'a' || a == 'e' || a == 'i' || a == 'o' || a == 'u')
            return 1;

      return 0;
}