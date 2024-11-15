#include "swap_area.h"

#include <stdlib.h>
#include <stdio.h>

#include "log.h"

void swap_area_init(SwapArea *swap_area, int size)
{
    swap_area->size = size;
    swap_area->entries = (SwappedPageEntry *)malloc(size * sizeof(SwappedPageEntry));
    swap_area->free_blocks = (int *)malloc(size * sizeof(int));
    swap_area->entry_count = 0;
    swap_area->free_block_count = size;

    for (int i = 0; i < size; i++)
    {
        swap_area->free_blocks[i] = 1; // Todos os blocos inicialmente livres.
    }
}

int swap_area_allocate_block(SwapArea *swap_area, int size)
{
    for (int i = 0; i < swap_area->size; i++)
    {
        if (swap_area->free_blocks[i])
        {
            swap_area->free_blocks[i] = 0;
            swap_area->free_block_count--;
            return i;
        }
    }
    return -1; // Nenhum bloco livre disponível.
}

Page *swap_area_read_block(SwapArea *swap_area, int block_number)
{
    if (block_number >= swap_area->size || swap_area->free_blocks[block_number])
    {
        return NULL;
    }

    return swap_area->entries[block_number].page;
}

void swap_area_write_block(SwapArea *swap_area, Page *page)
{
    int block = swap_area_allocate_block(swap_area, 1);

    if (block == -1)
    {
        log_message(LOG_ERROR, "Nenhum bloco disponível para escrita.");
        return;
    }
    
    swap_area->entries[block].page_number = page->page_number;
    swap_area->entries[block].page = page;
}

void swap_area_release_block(SwapArea *swap_area, int block_number)
{
    if (block_number >= swap_area->size || swap_area->free_blocks[block_number])
    {
        log_message(LOG_ERROR, "Tentativa de liberar um bloco inválido.");
        return;
    }

    swap_area->free_blocks[block_number] = 1;
    swap_area->free_block_count++;
}
