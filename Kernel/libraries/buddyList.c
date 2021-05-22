#include "buddyList.h"

void listInit(List *list)
{
    list->prev = list;
    list->next = list;
}

void listPush(List *list, List *entry)
{
    List *prev = list->prev;
    entry->prev = prev;
    entry->next = list;
    prev->next = entry;
    list->prev = entry;
}

void listRemove(List *entry)
{
    List *prev = entry->prev;
    List *next = entry->next;
    prev->next = next;
    next->prev = prev;
}

List *listPop(List *list)
{
    List *back = list->prev;
    if (back == list)
        return NULL;
    listRemove(back);
    return back;
}

/* Because these are circular lists, an "empty"
list is an entry where both links point to itself.
*/

char isEmpty(List *list)
{
    return list->prev == list;
}