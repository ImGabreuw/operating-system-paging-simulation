#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include <stdbool.h>

#include "page_table_entry.h"

typedef struct
{
    PageTableEntry **entries;
    int number_of_pages;
} PageTable;

/**
 * Inicializa a tabela de páginas com um número especificado de entradas.
 *
 * @param page_table - ponteiro para a tabela a ser inicializada.
 * @param number_of_pages - número de páginas da tabela.
 * @return 0 em caso de sucesso.
 */
int page_table_create(PageTable *page_table, int number_of_pages);

/**
 * Adiciona um mapeamento de página para quadro na tabela de páginas.
 *
 * @param pagetable - ponteiro para a tabela de páginas.
 * @param page_number - número da página virtual.
 * @param frame_number - número do quadro físico.
 * @return 0 se o mapeamento foi adicionado com sucesso; caso contrário, 1.
 */
int add_mapping(PageTable *pagetable, int page_number, int frame_number);

/**
 * Remove um mapeamento da tabela de páginas.
 *
 * @param pagetable - ponteiro para a tabela de páginas.
 * @param page_number - número da página virtual a ser removida.
 * @return 0 se o mapeamento foi removido com sucesso; caso contrário, 1.
 */
int remove_mapping(PageTable *pagetable, int page_number);

/**
 * Obtém o número do quadro físico associado a uma página virtual.
 *
 * @param pagetable - ponteiro para a tabela de páginas.
 * @param page_number - número da página virtual.
 * @return número do quadro físico se a página estiver mapeada; caso contrário, -1.
 */
int get_frame_number(PageTable *pagetable, int page_number);

/**
 * Verifica se uma página está válida na tabela de páginas.
 *
 * @param pagetable - ponteiro para a tabela de páginas.
 * @param page_number - número da página virtual.
 * @return true se a página estiver válida; caso contrário, false.
 */
bool is_page_valid(PageTable *pagetable, int page_number);

#endif // PAGE_TABLE_H