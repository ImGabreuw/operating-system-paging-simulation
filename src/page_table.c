#include "page_table.h"

#include "log.h"

int page_table_create(PageTable *page_table, int number_of_pages)
{
    if (!page_table)
    {
        log_message(LOG_ERROR, "Page table pointer cannot be NULL.");
        return EXIT_FAILURE;
    }

    if (number_of_pages <= 0)
    {
        log_message(LOG_ERROR, "Number of pages must be a positive integer.");
        return EXIT_FAILURE;
    }
    page_table->number_of_pages = number_of_pages;

    page_table->entries = (PageTableEntry **)malloc(number_of_pages * sizeof(PageTableEntry *));
    if (page_table->entries == NULL)
    {
        log_message(LOG_ERROR, "Out of memory. Unable to allocate page table entries.");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < number_of_pages; i++)
    {
        PageTableEntry *entry = malloc(sizeof(PageTableEntry));

        if (!entry)
        {
            free(page_table->entries);
            log_message(LOG_ERROR, "Out of memory. Unable to allocate page table entry.");
            return EXIT_FAILURE;
        }

        page_table_entry_create(entry, i, -1);
        page_table->entries[i] = entry;
    }

    return EXIT_SUCCESS;
}

int add_mapping(PageTable *page_table, int page_number, int frame_number)
{
    for (int i = 0; i < page_table->number_of_pages; i++)
    {
        if (page_table->entries[i]->page_number == page_number)
        {
            page_table->entries[i]->frame_number = frame_number;
            page_table->entries[i]->is_valid = true; // Valida a entrada
            return EXIT_SUCCESS;
        }
    }
    log_message(LOG_WARNING, "Página %d não encontrada na tabela de páginas.", page_number);
    return EXIT_FAILURE;
}

int remove_mapping(PageTable *pagetable, int page_number)
{
    for (int i = 0; i < pagetable->number_of_pages; i++)
    {
        if (pagetable->entries[i]->page_number == page_number)
        {
            pagetable->entries[i]->page_number = -1;
            pagetable->entries[i]->frame_number = -1;

            invalidate(pagetable->entries[i]);

            return EXIT_SUCCESS;
        }
    }

    return EXIT_FAILURE;
}

int get_frame_number(PageTable *pagetable, int page_number)
{
    for (int i = 0; i < pagetable->number_of_pages; i++)
    {
        if (pagetable->entries[i]->page_number == page_number)
        {
            return pagetable->entries[i]->frame_number;
        }
    }

    return -1;
}

bool is_page_valid(PageTable *pagetable, int page_number)
{
    for (int i = 0; i < pagetable->number_of_pages; i++)
    {
        if (pagetable->entries[i]->page_number == page_number)
        {
            return pagetable->entries[i]->is_valid;
        }
    }

    return false;
}
