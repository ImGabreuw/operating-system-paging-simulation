#ifndef PROCESS_STRUCTURES
#define PROCESS_STRUCTURES

#include <stdio.h>
#include <stdbool.h>

#include "logical_memory.h"
#include "page_table.h"
#include "process.h"

int translateAddress(Process* p, LogicalMemory* lm, int logicalAddress);

#endif
