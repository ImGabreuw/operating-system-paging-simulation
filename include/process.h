#ifndef PROCESS_H
#define PROCESS_H

#include <stdbool.h>

#include "page_table.h"
#include "logical_memory.h"
#include "disk.h"

#define QUANTUM 1000 // tempo em ms
#define PROCESS_SIZE (20480) // Tamanho total do processo em bytes
#define NUMBER_OF_PAGES (PROCESS_SIZE / PAGE_SIZE)

/**
 * @struct Process
 * @brief Representa um processo no simulador de paginação, incluindo campos para identificação, gerenciamento de páginas e simulação de acessos à memória.
 */
typedef struct
{
    int pid;                       // Identificador único do processo
    int size;                      // Tamanho total do processo em páginas
    PageTable *page_table;         // Tabela de páginas do processo
    int *access_sequence;          // Sequência de acessos aos endereços de memória
    int addresses_count;           // Contagem total de endereços acessados
    int quantum_time;              // Tempo restante no quantum
    bool is_running;               // Indicador se o processo está em execução
    LogicalMemory *logical_memory; // Memória lógica associada ao processo
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
int process_create(Process *process, LogicalMemory *logical_memory, Disk* disk, int pid, int addresses_count, int size);

void process_free_table_page(Process *process);

#endif /* PROCESS_H */
