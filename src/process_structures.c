#include "process_structures.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "logical_memory.h"
#include "physical_memory.h"
#include "log.h"

#define MAX_ACCESSES 1000

// Metodos de Processo
int create_p(Process *p, int pid, int addressesCount, int size)
{
    p->pid = pid;
    p->size = size;

    p->pageTable = (PageTable *)malloc(sizeof(PageTable));
    page_table_create(p->pageTable, NUMBER_OF_PAGES);

    log_message(LOG_INFO, "Page table created successfully with %d pages for process %d.", NUMBER_OF_PAGES, pid);

    p->logicalMemory = (LogicalMemory *)malloc(sizeof(LogicalMemory));
    p->addressesCount = addressesCount;
    p->accessSequence = (int *)calloc(MAX_ACCESSES, sizeof(int));

    return 0;
}

// MMU: metodo de traducao

int translateAddress(Process *p, LogicalMemory *lm, int logicalAddress)
{
    int pageNum = logicalAddress / FRAME_SIZE;
    Page *page = get_page(lm, pageNum);
    if (page->is_loaded)
        return -1;
    int offset = logicalAddress % FRAME_SIZE;
    int frameNum = get_frame_number(p->pageTable, pageNum);
    return frameNum * FRAME_SIZE + offset;
}