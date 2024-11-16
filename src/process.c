#include "process.h"

#include <stdlib.h>

#include "log.h"

#define MAX_ACCESSES 1000

int process_create(Process *process, LogicalMemory *logical_memory,Disk* disk, int pid, int addresses_count, int size)
{
    process->pid = pid;
    process->size = size;

    process->page_table = (PageTable *)malloc(sizeof(PageTable));
    page_table_create(process->page_table, NUMBER_OF_PAGES);

    log_message(LOG_INFO, "Tabela de páginas criada com sucesso com %d páginas para o processo %d.", NUMBER_OF_PAGES, pid);

    for (int i = 0; i < NUMBER_OF_PAGES; i++)
    {
        logical_memory->pages[i] = (Page *)malloc(sizeof(Page));
        page_create(logical_memory->pages[i], i, process->pid);
        disk_write_page(disk, logical_memory->pages[i]);

    }

    process->quantum_time = QUANTUM;
    process->is_running = false;

    process->logical_memory = logical_memory;
    process->addresses_count = addresses_count;
    process->access_sequence = (int *)calloc(MAX_ACCESSES, sizeof(int));

    log_message(LOG_INFO, "Processo '%d' criado com sucesso;", pid);

    return EXIT_SUCCESS;
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