#include "logical_memory.h"

#include <stdlib.h>

int logical_memory_create(LogicalMemory *logical_memory, int size, int page_size, int process_size)
{
    if (logical_memory == NULL || page_size <= 0)
        return EXIT_FAILURE;

    logical_memory->size = size;
    logical_memory->page_size = page_size;
    logical_memory->pages = (Page **)malloc((process_size / page_size) * sizeof(Page *));

    if (logical_memory->pages == NULL)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

Page *get_page(LogicalMemory *logical_memory, int page_number)
{
    if (logical_memory == NULL || logical_memory->pages == NULL)
        return NULL;

    for (int i = 0; i < logical_memory->size; i++)
    {
        if (logical_memory->pages[i] && logical_memory->pages[i]->page_number == page_number)
        {
            return logical_memory->pages[i];
        }
    }
    return NULL;
}

Page *create_pages(LogicalMemory *logical_memory, int process_size)
{
    if (logical_memory == NULL || logical_memory->page_size <= 0)
        return NULL;

    int pages_needed = process_size / logical_memory->page_size;
    logical_memory->size = pages_needed;

    for (int i = 0; i < pages_needed; i++)
    {
        logical_memory->pages[i] = (Page *)malloc(sizeof(Page));
        if (logical_memory->pages[i] == NULL)
            return NULL;

        logical_memory->pages[i]->page_number = i;
        logical_memory->pages[i]->is_dirty = false;
        logical_memory->pages[i]->is_loaded = false;
        logical_memory->pages[i]->pid = -1; // ID do processo pode ser atualizado conforme necessário
    }

    return logical_memory->pages[0];
}
