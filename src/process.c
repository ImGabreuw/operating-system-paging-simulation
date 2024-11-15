#include "process.h"

#include <stdlib.h>

#include "log.h"

#define MAX_ACCESSES 1000

int process_create(Process *process, LogicalMemory *logical_memory, int pid, int addresses_count, int size)
{
    process->pid = pid;
    process->size = size;

    process->page_table = (PageTable *)malloc(sizeof(PageTable));
    page_table_create(process->page_table, NUMBER_OF_PAGES);

    log_message(LOG_INFO, "Page table created successfully with %d pages for process %d.", NUMBER_OF_PAGES, pid);

    process->quantum_time = QUANTUM;
    process->is_running = false;

    process->logical_memory = logical_memory;
    process->addresses_count = addresses_count;
    process->access_sequence = (int *)calloc(MAX_ACCESSES, sizeof(int));

    return 0;
}

void access_address(Process *process, int virtual_address)
{
    int page_number = virtual_address / PAGE_SIZE;
    if (!is_page_valid(process->page_table, page_number))
    {
        log_message(LOG_ERROR, "To implement page fault");
        return;
    }

    process->access_sequence[virtual_address] = 1;
}

void allocate_pages(Process *process)
{
    for (int i = 0; i < process->size; i++)
    {
        if (!is_page_valid(process->page_table, i))
        {
            log_message(LOG_ERROR, "To implement page allocation");
            return;
        }
    }
}

void process_free_table_page(Process *process)
{
    for (int i = 0; i < process->page_table->number_of_pages; i++)
    {
        free(process->page_table->entries[i]);
    }

    free(process->page_table->entries);
    free(process->page_table);
}