#ifndef PIPES_H
#define PIPES_H

int pOpen(int pipeId);
int pClose(int pipeId);
int pRead(int pipeId);
int pWrite(int pipeId, char *str);
int putCharPipe(int pipeId, char c);
void dumpPipes();

#endif