#ifndef MEMORY_MANAGEMENT_UNIT_H
#define MEMORY_MANAGEMENT_UNIT_H

#include "logical_memory.h"
#include "physical_memory.h"
#include "process.h"

/**
 * @struct MemoryManagementUnit
 * @brief Estrutura que representa a unidade de gerenciamento de memória (MMU).
 */
typedef struct {
    LogicalMemory logical_memory;        /**< Memória lógica */
    PhysicalMemory physical_memory;      /**< Memória física */
    // Disk disk;                           /**< Disco para armazenamento secundário */
    // ProcessManager process_manager;      /**< Gerenciador de processos */
} MemoryManagementUnit;

/**
 * @brief Traduz um endereço lógico para um endereço físico.
 * @param virtual_address Endereço lógico a ser traduzido.
 * @return Endereço físico correspondente.
 */
int translate_address(Process *Process, int virtual_address);

/**
 * @brief Lida com a falta de página (page fault) e carrega a página necessária.
 * @param mmu Unidade de Gerenciamento de Memória.
 * @param page_number Número da página faltante.
 */
void handle_page_fault(MemoryManagementUnit *mmu, int page_number);

/**
 * @brief Carrega um novo processo na memória lógica e física.
 * @param mmu Unidade de Gerenciamento de Memória.
 * @param process Processo a ser carregado.
 */
void load_process(MemoryManagementUnit *mmu, Process *process);

/**
 * @brief Acessa a memória com um endereço lógico, acionando a falta de página se necessário.
 * @param mmu Unidade de Gerenciamento de Memória.
 * @param virtual_address Endereço lógico a ser acessado.
 */
void access_memory(MemoryManagementUnit *mmu, int virtual_address);

#endif /* MEMORY_MANAGEMENT_UNIT_H */
