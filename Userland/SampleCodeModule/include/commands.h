#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>
#include <shell.h>

//prints commands man
void help(int argc, char **args);

//print registers, done in assembly
void inforeg(int argc, char **args);

//recieves memory direction and prints 32 bytes after
void printmem(int argc, char **args);

//displays system time
void time(int argc, char **args);

//shows processor model and brand
void cpuInfo(int argc, char **args);

//shows processor temp
void cpuTemp(int argc, char **args);

//change username in shell
void changeUsername(int argc, char **argv);

//check zero exception
void checkZeroException(int argc, char **args);

//check invalid op exception
void checkInvalidOpcodeException(int argc, char **args);

//shows arguments
void showArgs(int argc, char **args);

void ps(int argc, char **args);

void testPriority(int argc, char **args);

void testScheduler(int argc, char **args);

void mem();

void loopCommand(int argc, char **args);

void kill(int argc, char **args);

void niceProcess(int argc, char **args);

void block(int argc, char **args);

void unblock(int argc, char **args);

#endif