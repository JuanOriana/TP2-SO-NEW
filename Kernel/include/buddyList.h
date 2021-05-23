#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdint.h>

typedef struct List
{
    uint8_t free;
    uint8_t level;
    struct List *prev, *next;
} List;

void listInit(List *list);

void listPush(List *list, List *entry);

void listRemove(List *entry);

List *listPop(List *list);

char isEmpty(List *list);

#endif