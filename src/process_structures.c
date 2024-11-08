#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "process_structures.h"
#include "physicalMemory.h"
#include "logicalMemory.h"

#define MAX_ACCESSES (1000)


// Metodos de PageTableEntry

int create_pte(PageTableEntry *pte, int pageNumber, int frameNumber)
{
    pte->pageNumber = pageNumber;
    pte->frameNumber = frameNumber;
    pte->isDirty = false;
    pte->isValid = false;
    return 0;
}

void pagetable_mark_dirty(PageTableEntry *p_entry)
{
    p_entry->isDirty = true;
}

void pagetable_mark_clean(PageTableEntry *p_entry)
{
    p_entry->isDirty = false;
}

void validate(PageTableEntry *p_entry)
{
    p_entry->isValid = true;
}

void invalidate(PageTableEntry *p_entry)
{
    p_entry->isValid = false;
}

// Metodos de PageTable
int create_pta(PageTable *pta, int numberOfPages)
{
    pta->entries = (PageTableEntry **)malloc(sizeof(PageTableEntry *));
    pta->numberOfPages = numberOfPages;
    return 0;
}

bool addMapping(PageTable *pagetable, int pageNum, int frameNum)
{
    int entry_idx = 0;

    // Procurar a pagina esperada
    while (entry_idx < pagetable->numberOfPages && pagetable->entries[entry_idx]->pageNumber != pageNum)
        entry_idx++;

    // Inserir o novo registro
    if (entry_idx < pagetable->numberOfPages)
    {
        pagetable->entries[entry_idx]->frameNumber = frameNum;
        return true;
    }

    // nao encontrou a pagina
    return false;
}

bool removeMapping(PageTable *pagetable, int pageNum)
{

    // Procurar registro
    for (int entry_idx = 0; entry_idx < pagetable->numberOfPages; entry_idx++)
    {
        if (pagetable->entries[entry_idx]->pageNumber == pageNum)
        {
            pagetable->entries[entry_idx]->pageNumber = -1;
            pagetable->entries[entry_idx]->frameNumber = -1;
            return true; // encontrou e removeu
        }
    }
    return false; // nao encontrou
}

int getFrameNumber(PageTable *pagetable, int pageNum)
{
    // Procurar registro
    for (int entry_idx = 0; entry_idx < pagetable->numberOfPages; entry_idx++)
    {
        if (pagetable->entries[entry_idx]->pageNumber == pageNum)
        {
            return pagetable->entries[entry_idx]->frameNumber; // encontrou
        }
    }

    return -1; // nao encontrou
}

bool isPageValid(PageTable *pagetable, int pageNum)
{

    for (int entry_idx = 0; entry_idx < pagetable->numberOfPages; entry_idx++)
    {
        if (pagetable->entries[entry_idx]->pageNumber == pageNum)
        {
            return pagetable->entries[entry_idx]->isValid;
        }
    }

    return false;
}

// Metodos de Processo
int create_p(Process* p,int pid, int addressesCount, int size)
{
    p->pid = pid;
    p->size = size;
    p->pageTable = (PageTable*) malloc(sizeof(PageTable));
    p->logicalMemory = (LogicalMemory*) malloc(sizeof(LogicalMemory));
    p->addressesCount = addressesCount;
    p->accessSequence = (int *) calloc(MAX_ACCESSES,sizeof(int));

    return 0;
}

//MMU: metodo de traducao

int translateAddress(Process* p, LogicalMemory* lm, int logicalAddress){
    int pageNum = logicalAddress/FRAME_SIZE;
    Page* page= getPage(lm,pageNum);
    if(page->isLoaded) return -1;
    int offset = logicalAddress%FRAME_SIZE;
    int frameNum = getFrameNumber(p->pageTable,pageNum);
    return frameNum*FRAME_SIZE+offset;
}