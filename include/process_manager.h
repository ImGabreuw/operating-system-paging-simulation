#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

typedef struct MemoryManagementUnit MemoryManagementUnit;

#include "memory_management_unit.h"
#include "process_queue.h"
#include "frame.h"

typedef struct ProcessManager
{
    MemoryManagementUnit *mmu;
    Process **running_processes; // Array de processos em execução
    int max_processes;           // Capacidade máxima de processos
    Queue ready_queue;           // Fila de processos prontos
    int quantum;                 // Tempo máximo (em ms) para o Round Robin
} ProcessManager;

extern Process *processes;
extern int num_processes;

/**
 * Inicializa o ProcessManager.
 *
 * @param manager Ponteiro para a estrutura do ProcessManager.
 * @param max_processes Número máximo de processos simultâneos.
 * @param quantum Tempo de quantum em milissegundos.
 * @return 0 para sucesso e 1 para erro
 */
int process_manager_init(ProcessManager *manager, int max_processes, int quantum);

/**
 * Termina um processo.
 *
 * @param manager Ponteiro para a estrutura do ProcessManager.
 * @param pid Identificador único do processo a ser terminado.
 */
void terminate_process(ProcessManager *manager, int pid);

/**
 * Adiciona um processo à fila de prontos.
 *
 * @param manager Ponteiro para a estrutura do ProcessManager.
 * @param process Ponteiro para o processo a ser escalonado.
 */
void schedule_process(ProcessManager *manager, Process *process);

/**
 * Executa os processos da fila de prontos usando Round Robin.
 *
 * @param manager Ponteiro para a estrutura do ProcessManager.
 */
void run_scheduled_processes(ProcessManager *manager);

/**
 * Libera todos os recursos alocados pelo ProcessManager.
 *
 * @param manager Ponteiro para a estrutura do ProcessManager.
 */
void process_manager_cleanup(ProcessManager *manager);

/** 
 * Obtém o processo alocado no frame.
 * 
 * @param manager Ponteiro para a estrutura do ProcessManager.
 * @param frame Ponteiro para a estrutura do Frame alocado  
 * 
 * @return Processo alocado
 * 
*/
Process *get_allocated_process(ProcessManager *manager,Frame *frame);

#endif // PROCESS_MANAGER_H
