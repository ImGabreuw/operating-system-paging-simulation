#include "process_structures.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "logical_memory.h"
#include "physical_memory.h"
#include "log.h"

#define MAX_ACCESSES 1000

// MMU: metodo de traducao

int translateAddress(Process *p, LogicalMemory *lm, int logicalAddress)
{
    int pageNum = logicalAddress / FRAME_SIZE;
    Page *page = get_page(lm, pageNum);
    if (page->is_loaded)
        return -1;
    int offset = logicalAddress % FRAME_SIZE;
    int frameNum = get_frame_number(p->page_table, pageNum);
    return frameNum * FRAME_SIZE + offset;
}