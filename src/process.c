#include "process.h"

#include <stdlib.h>

#include "log.h"

#define MAX_ACCESSES 1000

/**
 * Cria um novo processo.
 * Inicializa o processo com o identificador `pid`, o número de endereços a serem acessados, e o tamanho do processo.
 * Aloca a memória necessária para a tabela de páginas e para a sequência de acessos.
 */
int process_create(Process *process, LogicalMemory *logical_memory, int pid, int addresses_count, int size)
{
    process->pid = pid;
    process->size = size;

    process->page_table = (PageTable *)malloc(sizeof(PageTable));
    page_table_create(process->page_table, NUMBER_OF_PAGES);

    log_message(LOG_INFO, "Page table created successfully with %d pages for process %d.", NUMBER_OF_PAGES, pid);

    process->logical_memory = logical_memory;
    process->addresses_count = addresses_count;
    process->access_sequence = (int *)calloc(MAX_ACCESSES, sizeof(int));

    return 0;
}

/**
 * Acessa um endereço virtual.
 *
 * Este método tenta acessar o endereço virtual fornecido, verificando se a página está carregada na memória.
 * Se a página não estiver presente, ocorre uma falha de página e a página é carregada da memória secundária.
 */
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

/**
 * Aloca as páginas necessárias para o processo.
 *
 * Este método aloca as páginas necessárias do processo na memória física.
 */
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
