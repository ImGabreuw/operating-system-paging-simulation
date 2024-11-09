#ifndef PAGE_TABLE_ENTRY_H
#define PAGE_TABLE_ENTRY_H

#include <stdbool.h>
#include <stdlib.h>

// Estrutura para representar uma entrada na tabela de páginas
typedef struct
{
    int page_number;
    int frame_number;
    bool is_valid;
    bool is_dirty;
} PageTableEntry;

/**
 * Inicializa uma nova entrada de tabela de páginas.
 *
 * @param page_table_entry - ponteiro para a entrada a ser inicializada.
 * @param page_number - número da página virtual.
 * @param frame_number - número do quadro físico.
 * @return 0 em caso de sucesso.
 */
int page_table_entry_create(PageTableEntry *page_table_entry, int page_number, int frame_number);

/**
 * Marca a entrada como "suja", indicando que foi modificada.
 *
 * @param page_table_entry - ponteiro para a entrada a ser marcada como suja.
 */
void pagetable_mark_dirty(PageTableEntry *page_table_entry);

/**
 * Marca a entrada como "limpa", indicando que não possui alterações pendentes.
 *
 * @param page_table_entry - ponteiro para a entrada a ser marcada como limpa.
 */
void pagetable_mark_clean(PageTableEntry *page_table_entry);

/**
 * Marca a entrada como válida, sinalizando que a página está na memória.
 *
 * @param page_table_entry - ponteiro para a entrada a ser marcada como válida.
 */
void validate(PageTableEntry *page_table_entry);

/**
 * Marca a entrada como inválida, indicando que a página não está carregada.
 *
 * @param page_table_entry - ponteiro para a entrada a ser marcada como inválida.
 */
void invalidate(PageTableEntry *page_table_entry);

#endif // PAGE_TABLE_ENTRY_H
