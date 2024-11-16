#ifndef MEMORY_MANAGEMENT_UNIT_H
#define MEMORY_MANAGEMENT_UNIT_H

typedef struct PhysicalMemory PhysicalMemory;
#include "process_manager.h"
typedef struct ProcessManager ProcessManager;


#include "logical_memory.h"
#include "disk.h"
#include "physical_memory.h"

typedef struct MemoryManagementUnit
{
    LogicalMemory logical_memory;    // Memória lógica.
    PhysicalMemory physical_memory;  // Memória física.
    Disk *disk;                      // Disco (memória secundária).
    ProcessManager *process_manager; // Gerenciador de processos.
} MemoryManagementUnit;

/**
 * Inicializa a MMU.
 *
 * @param mmu Ponteiro para a estrutura MemoryManagementUnit.
 * @param logical_size Tamanho da memória lógica.
 * @param physical_size Tamanho da memória física.
 * @param disk Ponteiro para o disco.
 * @param process_manager Ponteiro para o gerenciador de processos.
 */
void mmu_init(MemoryManagementUnit *mmu, int logical_size, int physical_size, Disk *disk, ProcessManager *process_manager);

/**
 * Traduz um endereço lógico para um endereço físico.
 *
 * @param mmu Ponteiro para a estrutura MemoryManagementUnit.
 * @param virtual_address Endereço lógico a ser traduzido.
 * @return Endereço físico correspondente ou -1 se houver falha de página.
 */
int mmu_translate_address(MemoryManagementUnit *mmu, Process *process, int virtual_address);

/**
 * Lida com uma falta de página (page fault).
 *
 * @param mmu Ponteiro para a estrutura MemoryManagementUnit.
 * @param page_number Número da página faltante.
 */
void mmu_handle_page_fault(MemoryManagementUnit *mmu, Process *process, int page_number);

/**
 * Carrega um processo na memória.
 *
 * @param mmu Ponteiro para a estrutura MemoryManagementUnit.
 * @param process Ponteiro para a estrutura Process.
 */
void mmu_load_process(MemoryManagementUnit *mmu, Process *process);

/**
 * Acessa a memória a partir de um endereço lógico.
 *
 * @param mmu Ponteiro para a estrutura MemoryManagementUnit.
 * @param virtual_address Endereço lógico a ser acessado.
 */
void mmu_access_memory(MemoryManagementUnit *mmu, Process *process, int virtual_address);


#endif // MEMORY_MANAGEMENT_UNIT_H
