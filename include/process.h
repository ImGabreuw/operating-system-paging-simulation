#ifndef PROCESS_H
#define PROCESS_H

#include "page_table.h"
#include "logical_memory.h"

#define PROCESS_SIZE (8192) // Tamanho total do processo em bytes
#define NUMBER_OF_PAGES (PROCESS_SIZE / PAGE_SIZE)

/**
 * @struct Process
 * @brief Representa um processo no simulador de paginação, incluindo campos para identificação, gerenciamento de páginas e simulação de acessos à memória.
 */
typedef struct
{
    int pid;                      /**< Identificador único do processo */
    int size;                     /**< Tamanho total do processo em páginas */
    PageTable *page_table;          /**< Tabela de páginas do processo */
    int *access_sequence;          /**< Sequência de acessos aos endereços de memória */
    int addresses_count;           /**< Contagem total de endereços acessados */
    LogicalMemory *logical_memory; /**< Memória lógica associada ao processo */
} Process;

/**
 * @brief Cria um novo processo.
 *
 * Inicializa o processo com o identificador `pid`, o número de endereços a serem acessados, e o tamanho do processo.
 * Aloca a memória necessária para a tabela de páginas e para a sequência de acessos.
 *
 * @param process Ponteiro para o processo a ser criado.
 * @param pid Identificador único do processo.
 * @param addresses_count Número de endereços que o processo irá acessar.
 * @param size Tamanho total do processo em páginas.
 * @param logical_memory Ponteiro para a estrutura da memória lógica
 * @return 0 se o processo for criado com sucesso.
 */
int process_create(Process *process, LogicalMemory *logical_memory, int pid, int addresses_count, int size);

/**
 * @brief Acessa um endereço virtual.
 *
 * Este método tenta acessar o endereço virtual fornecido, verificando se a página está carregada na memória.
 * Se a página não estiver presente, ocorre uma falha de página e a página é carregada da memória secundária.
 *
 * @param process Ponteiro para o processo.
 * @param virtual_address Endereço virtual a ser acessado.
 */
void access_address(Process *process, int virtual_address);

/**
 * @brief Aloca as páginas necessárias para o processo.
 *
 * Este método aloca as páginas necessárias do processo na memória física.
 */
void allocate_pages(Process *process);

void process_free_table_page(Process *process);

#endif /* PROCESS_H */
