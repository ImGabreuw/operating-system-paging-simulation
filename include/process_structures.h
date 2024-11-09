#ifndef PROCESS_STRUCTURES
#define PROCESS_STRUCTURES

#include <stdio.h>
#include <stdbool.h>

#include "logical_memory.h"
#include "page_table.h"

#define PROCESS_SIZE (8192) // Tamanho total do processo em bytes
#define NUMBER_OF_PAGES (PROCESS_SIZE / PAGE_SIZE)

//PROCESS
typedef struct {
    int pid;
    int size;
    int* accessSequence;
    int addressesCount;
    PageTable* pageTable;
    LogicalMemory* logicalMemory;
}Process;

int create_p(Process* p,int pid, int addressesCount, int size);

int translateAddress(Process* p, LogicalMemory* lm, int logicalAddress);
#endif
