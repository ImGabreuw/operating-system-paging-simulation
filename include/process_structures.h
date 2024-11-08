#ifndef PROCESS_STRUCTURES
#define PROCESS_STRUCTURES

#include <stdio.h>
#include <stdbool.h>
#include "logicalMemory.h"

//PAGE TABLE ENTRY
typedef struct{
    int pageNumber;
    int frameNumber;
    bool isValid;
    bool isDirty;

}PageTableEntry;

int create_pte(PageTableEntry* pte,int pageNumber, int frameNumber);

void pagetable_mark_dirty(PageTableEntry* p_entry);

void pagetable_mark_clean(PageTableEntry* p_entry);

void validate(PageTableEntry* p_entry);

void invalidate(PageTableEntry* p_entry);


//PAGE TABLE
typedef struct{
    PageTableEntry** entries;
    int numberOfPages;
}PageTable;

int create_pta(PageTable* pta, int numberOfPages);

bool addMapping(PageTable *pagetable, int pageNum, int frameNum);

bool removeMapping(PageTable *pagetable,int pageNum);

int getFrameNumber(PageTable *pagetable,int pageNum);

bool isPageValid(PageTable *pagetable,int pageNum);

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
