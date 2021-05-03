// #include <memoryManager.h>
// #include <stdint.h>
// #include <stdlib.h>
// #include <unistd.h>

// // Reference: https://github.com/evanw/buddy-malloc/blob/master/buddy-malloc.c

// /*
//  * This file implements a buddy memory allocator, which is an allocator that
//  * allocates memory within a fixed linear address range. It spans the address
//  * range with a binary tree that tracks free space. Both "malloc" and "free"
//  * are O(log N) time where N is the maximum possible number of allocations.
//  *
//  * The "buddy" term comes from how the tree is used. When memory is allocated,
//  * nodes in the tree are split recursively until a node of the appropriate size
//  * is reached. Every split results in two child nodes, each of which is the
//  * buddy of the other. When a node is freed, the node and its buddy can be
//  * merged again if the buddy is also free. This makes the memory available
//  * for larger allocations again.
//  */

// /*
//  * Every allocation needs an 8-byte header to store the allocation size while
//  * staying 8-byte aligned. The address returned by "malloc" is the address
//  * right after this header (i.e. the size occupies the 8 bytes before the
//  * returned address).
//  */
// #define HEADER_SIZE 8

// #define MIN_ALLOC_LOG_2 4 //16 B min aloc
// #define MIN_ALLOC ((size_t)1 << MIN_ALLOC_LOG2)

// #define MAX_ALLOC_LOG2 30 //1GB MB max aloc
// #define MAX_LEVELS (MAX_ALLOC_LOG2 - MIN_ALLOC_LOG_2)

// /*
//  * Free lists are stored as circular doubly-linked lists. Every possible
//  * allocation size has an associated free list that is threaded through all
//  * currently free blocks of that size. That means MIN_ALLOC must be at least
//  * "sizeof(List)". MIN_ALLOC is currently 16 bytes, so this will be true for
//  * both 32-bit and 64-bit.
//  */
// typedef struct List
// {
//     struct List *prev, *next;
// } List;

// /*
//  * Each bucket corresponds to a certain allocation size and stores a free list
//  * for that size. The bucket at index 0 corresponds to an allocation size of
//  * MAX_ALLOC (i.e. the whole address space).
//  */
// static List buckets[MAX_LEVELS];

// /*
//  * This is the starting address of the address range for this allocator. Every
//  * returned allocation will be an offset of this pointer from 0 to maxMemSize.
//  */
// static List *base;
// static uint64_t maxMemSize;
// static uint8_t levels;

// void initMemBuddy(void *memBase, uint64_t totalSize)
// {
//     if (memBase == NULL)
//         return;

//     base = (List *)memBase;
//     maxMemSize = totalSize;

//     /*
//     * Los allocs se hacen en potencias de 2 arrancando de MIN_ALLOC y terminando en
//     * totalSize inclusive. Cada allocation size tiene un bucket que guarda una lista
//     * para ese tamano de alloc.
//     *
//     * Dado un indice del bucket list, el tamano de las alocaciones en ese bucket se encuentra con
//     * "(size_t)1 << (MAX_ALLOC_LOG2 - bucket)".
//     */
//     levels = (int)log2(totalSize) - MIN_ALLOC_LOG_2 + 1;

//     if (levels > MAX_LEVELS) //Upper bound definido por tamano de array
//         levels = MAX_LEVELS;

//     //Inicialmente todas las listas estan vacias EXCEPTO la de Max level (que corresponde a la memoria completa libre)
//     for (size_t i = 0; i < levels; i++)
//     {
//         listInit(&buckets[i]);
//         buckets[i].free = 0;
//         buckets[i].level = i;
//     }

//     addLevelNode(&buckets[levels - 1], base, levels - 1);
// }

// addLevelNode(List *list, List *node, uint8_t level)
// {
//     node->free = 1;
//     node->level = level;
//     listPush(list, node);
// }

// void *mallocCustBuddy(uint32_t bytes)
// {
//     /*
//    * Make sure it's possible for an allocation of this size to succeed. There's
//    * a hard-coded limit on the maximum allocation size because of the way this
//    * allocator works.
//    */
//     int realBytesNeeded = bytes + sizeof(List);

//     if (bytes == 0 || realBytesNeeded > maxMemSize + 1)
//     {
//         return NULL;
//     }

//     uint8_t minLevel = findIdealLevel(realBytesNeeded);

//     if ((uint8_t actualLevel = getAvailableLevle(minLevel)) == -1)
//     {
//         //No free level big enough :C
//         return NULL;
//     }

//     List *retNode;

//     //Keep fragmenting until I find my ideal size
//     for (retNode = listPop(&levelsList[availableLevel]); minLevel < availableLevel; availableLevel--)
//     {
//         retNode->level--;
//         addToLevel(&levelsList[availableLevel - 1], findBuddyFromNode(node), availableLevel - 1);
//     }

//     retNode->isFree = 0;

//     return (void *)(retNode + 1);
// }

// void freeCustBuddy(void *ptr)
// {
//     if (ptr == NULL)
//         return;

//     List *listPtr = (list_t *)ptr - 1;

//     listPtr->free = 1;

//     List *buddy = findBuddyFromNode(listPtr);

//     //If i can join buddys, do so
//     while (listPtr->level != levels - 1 && buddy->level == listPtr->level && buddy->free)
//     {
//         listRemove(buddy);
//         listPtr = getAdress(listPtr);
//         listPtr->level++;
//         buddy = getBuddy(listPtr);
//     }

//     listPush(&levelsList[listPtr->level], listPtr);
// }

// /*
//  * Initialize a list to empty. Because these are circular lists, an "empty"
//  * list is an entry where both links point to itself. This makes insertion
//  * and removal simpler because they don't need any branches.
//  */
// static void listInit(List *list)
// {
//     list->prev = list;
//     list->next = list;
// }

// /*
//  * Append the provided entry to the end of the list. This assumes the entry
//  * isn't in a list already because it overwrites the linked list pointers.
//  */
// static void listPush(List *list, List *entry)
// {
//     List *prev = list->prev;
//     entry->prev = prev;
//     entry->next = list;
//     prev->next = entry;
//     list->prev = entry;
// }

// /*
//  * Remove the provided entry from whichever list it's currently in. This
//  * assumes that the entry is in a list. You don't need to provide the list
//  * because the lists are circular, so the list's pointers will automatically
//  * be updated if the first or last entries are removed.
//  */
// static void listRemove(List *entry)
// {
//     List *prev = entry->prev;
//     List *next = entry->next;
//     prev->next = next;
//     next->prev = prev;
// }

// /*
//  * Remove and return the first entry in the list or NULL if the list is empty.
//  */
// static List *listPop(List *list)
// {
//     List *back = list->prev;
//     if (back == list)
//         return NULL;
//     listRemove(back);
//     return back;
// }

// static char listHasNone(List *list)
// {
//     return list->prev == list;
// }

// static uint8_t findIdealLevel(uint32_t bytes)
// {
//     uint8_t aux = log2(bytes);

//     //If smaller than min, then put it in the min
//     if (aux < MIN_ALLOC_LOG_2)
//         return 0;

//     aux -= MIN_ALLOC_LOG_2;

//     if (bytes && !(bytes & (bytes - 1))) //Si entra exactamente lo guardo en aux, si no es en aux + 1
//         return aux;

//     return aux + 1;
// }

// static int getFirstAvailableLevel(uint8_t minLevel)
// {
//     uint8_t minAvailableLevel;
//     //Primer nivel libre mayor a mi nivel minimo
//     for (minAvailableLevel = minLevel; minAvailableLevel < levels && listHasNone(&levelsList[minAvailableLevel]); minAvailableLevel++)
//         ;
//     //Me pase?
//     if (minAvailableLevel >= levels)
//         return -1;

//     return minAvailableLevel;
// }

// static List *findBuddyFromNode(List *node)
// {
//     uint8_t level = node->level;
//     void *currentOffset = (void *)node - (void *)base;
//     void *newOffset = currentOffset ^ BIN_POW(MIN_ALLOC_LOG_2 + level);

//     return (List *)(newOffset + (void *)base);
// }

// //www.archives.uce.com/105da903ea7e4ea183d6d3022e77e3a7?v=0c4be5ffdca74b0688ff3495045ab63e
// static List *getAdress(List *node)
// {
//     uint8_t level = node->level;
//     void *mask = BIN_POW(MIN_ALLOC_LOG_2 + level);
//     mask = ~mask;

//     // Esencialmente "partiendo" el offset
//     void *currentOffset = (void *)node - (void *)base;
//     void *newOffset = currentOffset & mask;

//     return (List *)(newOffset + (void *)base);
// }