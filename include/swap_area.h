#ifndef SWAP_AREA_H
#define SWAP_AREA_H

#include "page.h"

typedef struct
{
    int page_number; // Número da página no swap.
    Page *page;      // Ponteiro para a estrutura da página.
} SwappedPageEntry;

typedef struct
{
    int size;                  // Tamanho total da área de swap.
    SwappedPageEntry *entries; // Vetor de entradas de páginas.
    int *free_blocks;          // Vetor indicando blocos livres.
    int entry_count;           // Contador de entradas ocupadas.
    int free_block_count;      // Contador de blocos livres.
} SwapArea;

/**
 * Inicializa a área de swap.
 *
 * @param swap_area Ponteiro para a estrutura SwapArea.
 * @param size Tamanho total da área de swap (em blocos).
 */
void swap_area_init(SwapArea *swap_area, int size);

/**
 * Aloca um bloco na área de swap.
 *
 * @param swap_area Ponteiro para a estrutura SwapArea.
 * @param size Tamanho do bloco a ser alocado.
 * @return Número do bloco alocado ou -1 se não houver espaço.
 */
int swap_area_allocate_block(SwapArea *swap_area, int size);

/**
 * Lê um bloco da área de swap.
 *
 * @param swap_area Ponteiro para a estrutura SwapArea.
 * @param block_number Número do bloco a ser lido.
 * @return Ponteiro para a estrutura Page.
 */
Page *swap_area_read_block(SwapArea *swap_area, int block_number);

/**
 * Escreve um bloco na área de swap.
 *
 * @param swap_area Ponteiro para a estrutura SwapArea.
 * @param page Ponteiro para a estrutura Page a ser escrita.
 */
void swap_area_write_block(SwapArea *swap_area, Page *page);

/**
 * Libera um bloco na área de swap.
 *
 * @param swap_area Ponteiro para a estrutura SwapArea.
 * @param block_number Número do bloco a ser liberado.
 */
void swap_area_release_block(SwapArea *swap_area, int block_number);

#endif // SWAP_AREA_H
