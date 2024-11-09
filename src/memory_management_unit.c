#include "memory_management_unit.h"

#include <stdio.h>

#include "log.h"

int translate_address(Process *process, int logicalAddress)
{
    int pageNum = logicalAddress / FRAME_SIZE;
    Page *page = get_page(process->logical_memory, pageNum);

    if (page->is_loaded)
        return -1;

    int offset = logicalAddress % FRAME_SIZE;
    int frameNum = get_frame_number(process->page_table, pageNum);

    return frameNum * FRAME_SIZE + offset;
}
