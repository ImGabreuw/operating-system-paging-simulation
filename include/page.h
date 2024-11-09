#ifndef PAGE_H
#define PAGE_H

#include <stdbool.h>

/**
 * Representa uma página virtual associada a um processo.
 */
typedef struct
{
    int page_number; /**< Número da página no espaço virtual do processo */
    int pid;         /**< Identificador do processo proprietário da página */
    bool is_dirty;   /**< Indicador de modificação da página */
    bool is_loaded;  /**< Indicador de carregamento da página na RAM */
} Page;

/**
 * Inicializa uma página virtual com o número da página e o ID do processo.
 * @param page Ponteiro para a página a ser inicializada
 * @param page_number Número da página no espaço virtual do processo
 * @param pid Identificador do processo proprietário
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int page_create(Page *page, int page_number, int pid);

/**
 * Marca a página como suja (modificada).
 * @param page Ponteiro para a página a ser marcada como suja
 */
void page_mark_dirty(Page *page);

/**
 * Marca a página como limpa (não modificada).
 * @param page Ponteiro para a página a ser marcada como limpa
 */
void page_mark_clean(Page *page);

#endif
