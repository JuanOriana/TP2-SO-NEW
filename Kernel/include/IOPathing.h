#ifndef IOPATHING_H_
#define IOPATHING_H_

#include <stdint.h>
#include <colours.h>

void pathPrint(char *string, uint8_t length, t_colour bgColour, t_colour fontColour);
int pathGetChar();

#endif