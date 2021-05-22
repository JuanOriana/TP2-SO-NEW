// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com// #include <memoryManager.h>
#include "memoryManager.h"
#include "stringLib.h"
#include "listADT.h"
#include <unistd.h>
#include <utils.h>

// Reference: https://github.com/evanw/buddy-malloc/blob/master/buddy-malloc.c

#define MIN_ALLOC_LOG_2 4 //16 B min aloc
#define MIN_ALLOC ((size_t)1 << MIN_ALLOC_LOG2)

#define MAX_ALLOC_LOG2 32
#define MAX_LEVELS (MAX_ALLOC_LOG2 - MIN_ALLOC_LOG_2)

#define BIN_POW(x) (1 << (x))

/*
 * This is the starting address of the address range for this allocator. Every
 * returned allocation will be an offset of this pointer from 0 to maxMemSize.
 */
static List *base;
static uint64_t maxMemSize;
static uint8_t levels;
static List levelsList[MAX_LEVELS];

static uint8_t findIdealLevel(uint32_t bytes);
static List *findBuddy(List *node);
static List *getAddress(List *node);
static int getFirstAvailableLevel(uint8_t minLevel);
static void addNodeToLevel(List *list, List *node, uint8_t level);

void memInit(char *memBase, unsigned long memSize)
{
    if (memBase == NULL)
        return;

    base = (List *)memBase;
    maxMemSize = memSize;

    /*
    * Los allocs se hacen en potencias de 2 arrancando de MIN_ALLOC y terminando en
    * totalSize inclusive. Cada allocation size tiene un bucket que guarda una lista
    * para ese tamano de alloc.
    *
    * Dado un indice del bucket list, el tamano de las alocaciones en ese bucket se encuentra con
    * "(size_t)1 << (MAX_ALLOC_LOG2 - bucket)".
    */
    levels = (int)log2(memSize) - MIN_ALLOC_LOG_2 + 1;

    if (levels > MAX_LEVELS) //Upper bound definido por tamano de array
        levels = MAX_LEVELS;

    //Inicialmente todas las listas estan vacias EXCEPTO la de Max level (que corresponde a la memoria completa libre)
    for (size_t i = 0; i < levels; i++)
    {
        listInit(&levelsList[i]);
        levelsList[i].free = 0;
        levelsList[i].level = i;
    }

    addNodeToLevel(&levelsList[levels - 1], base, levels - 1);
}

/*
 * Every allocation needs an 8-byte header to store the allocation size while
 * staying 8-byte aligned. The address returned by "malloc" is the address
 * right after this header (i.e. the size occupies the 8 bytes before the
 * returned address).
 */

void *mallocCust(unsigned long nbytes)
{
    int realBytesNeeded = nbytes + sizeof(List);

    if (nbytes == 0 || realBytesNeeded > maxMemSize + 1)
    {
        return NULL;
    }

    uint8_t minLevel = findIdealLevel(realBytesNeeded);
    uint8_t actualLevel = getFirstAvailableLevel(minLevel);

    if (actualLevel == -1)
    {
        return NULL;
    }

    List *retNode;

    //Keep fragmenting until I find my ideal size
    for (retNode = listPop(&levelsList[actualLevel]); minLevel < actualLevel; actualLevel--)
    {
        retNode->level--;
        addNodeToLevel(&levelsList[actualLevel - 1], findBuddy(retNode), actualLevel - 1);
    }

    retNode->free = 0;

    return (void *)(retNode + 1);
}

void freeCust(void *ap)
{
    if (ap == NULL)
        return;

    List *listPtr = (List *)ap - 1;

    listPtr->free = 1;

    List *buddy = findBuddy(listPtr);

    //If i can join buddys, do so
    while (listPtr->level != levels - 1 && buddy->level == listPtr->level && buddy->free)
    {
        listRemove(buddy);
        listPtr = getAddress(listPtr);
        listPtr->level++;
        buddy = findBuddy(listPtr);
    }

    listPush(&levelsList[listPtr->level], listPtr);
}

static void printBlock(List *block, int idx)
{
    print("        Block number: %d\n", idx);
    print("            state: free\n");
}

void dumpMM()
{
    List *p, *aux;
    uint32_t index = 0;
    uint32_t availableSpace = 0;

    print("\nMEMORY DUMP (Buddy)\n");
    print("------------------------------------------------\n");
    print("Levels with free blocks:\n");
    print("-------------------------------\n");
    for (int i = levels - 1; i >= 0; i--)
    {
        p = &levelsList[i];
        if (!isEmpty(p))
        {
            print("    Level: %d\n", i + MIN_ALLOC_LOG_2);
            print("    Free blocks of size: 2^%d\n", i + MIN_ALLOC_LOG_2);

            for (aux = p->next, index = 1; aux != p; index++, aux = aux->next)
            {
                if (aux->free)
                {
                    printBlock(aux, index);
                    availableSpace += index * BIN_POW(i + MIN_ALLOC_LOG_2);
                }
            }

            print("-------------------------------\n");
        }
    }

    print("Available Space: %d\n", availableSpace);
}

static uint8_t findIdealLevel(uint32_t bytes)
{
    uint8_t aux = log2(bytes);

    //If smaller than min, then put it in the min
    if (aux < MIN_ALLOC_LOG_2)
        return 0;

    aux -= MIN_ALLOC_LOG_2;

    if (bytes && !(bytes & (bytes - 1))) //Perfect fit aux, else aux + 1
        return aux;

    return aux + 1;
}

static int getFirstAvailableLevel(uint8_t minLevel)
{
    uint8_t actualLevel;

    for (actualLevel = minLevel; actualLevel < levels && isEmpty(&levelsList[actualLevel]); actualLevel++)
        ;

    if (actualLevel > levels)
        return -1;

    return actualLevel;
}

static List *findBuddy(List *node)
{
    uint8_t level = node->level;
    uintptr_t currentOffset = (uintptr_t)node - (uintptr_t)base;
    uintptr_t newOffset = currentOffset ^ BIN_POW(MIN_ALLOC_LOG_2 + level);

    return (List *)(newOffset + (uintptr_t)base);
}

//www.archives.uce.com/105da903ea7e4ea183d6d3022e77e3a7?v=0c4be5ffdca74b0688ff3495045ab63e
static List *getAddress(List *node)
{
    uint8_t level = node->level;
    uintptr_t mask = BIN_POW(MIN_ALLOC_LOG_2 + level);
    mask = ~mask;

    uintptr_t currentOffset = (uintptr_t)node - (uintptr_t)base;
    uintptr_t newOffset = currentOffset & mask;

    return (List *)(newOffset + (uintptr_t)base);
}

static void addNodeToLevel(List *list, List *node, uint8_t level)
{
    node->free = 1;
    node->level = level;
    listPush(list, node);
}