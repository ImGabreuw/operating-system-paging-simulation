#include "memory_management_unit.h"

#include <stdio.h>

#include "log.h"

int translate_address(Process *process, int logicalAddress)
{
    int pageNum = logicalAddress / FRAME_SIZE;
    //Page *page = get_page(process->logical_memory, pageNum);

    int offset = logicalAddress % FRAME_SIZE;
    int frameNum = get_frame_number(process->page_table, pageNum);

    if(frameNum == -1) return frameNum;

    return frameNum * FRAME_SIZE + offset;
}
