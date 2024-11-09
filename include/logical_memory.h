#ifndef LOGICAL_MEMORY_H
#define LOGICAL_MEMORY_H

#include "page.h"

/**
 * Representa a memória lógica usada pelo simulador de paginação.
 */
typedef struct
{
    int size;      /**< Tamanho total da memória lógica em número de páginas */
    int page_size; /**< Tamanho individual de cada página */
    Page **pages;  /**< Array de ponteiros para as páginas */
} LogicalMemory;

/**
 * Inicializa a estrutura de memória lógica.
 * @param logical_memory Ponteiro para a memória lógica a ser inicializada
 * @param size Número total de páginas na memória lógica
 * @param page_size Tamanho de cada página em bytes
 * @param process_size Tamanho total do processo para alocação de páginas
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int logical_memory_create(LogicalMemory *logical_memory, int size, int page_size, int process_size);

/**
 * Retorna a página associada ao número de página especificado.
 * @param logical_memory Ponteiro para a estrutura de memória lógica
 * @param page_number Número da página a ser buscada
 * @return Ponteiro para a página encontrada, ou NULL se não encontrada
 */
Page *get_page(LogicalMemory *logical_memory, int page_number);

/**
 * Cria e aloca páginas para o processo com base no tamanho do processo.
 * @param logical_memory Ponteiro para a estrutura de memória lógica
 * @param process_size Tamanho total do processo
 * @return Ponteiro para a primeira página criada, ou NULL em caso de erro
 */
Page *create_pages(LogicalMemory *logical_memory, int process_size);

/**
 * Desaloca todas as páginas da memória lógica.
 * @param logical_memory Ponteiro para a estrutura de memória lógica
 */
void logical_memory_free_pages(LogicalMemory *logical_memory);

#endif
