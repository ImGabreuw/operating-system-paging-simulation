#include "process_manager.h"

#include <stdio.h>
#include <stdlib.h>

#include "log.h"

Process *processes = NULL;
int num_processes = 0;

int process_manager_init(ProcessManager *manager, int max_processes, int quantum)
{
    manager->max_processes = max_processes;
    manager->running_processes = (Process *)malloc(sizeof(Process *));

    if (!manager->running_processes)
    {
        log_message(LOG_ERROR, "Falha ao alocar memória para os processos em execução.");
        return EXIT_FAILURE;
    }

    manager->quantum = quantum;
    queue_init(&manager->ready_queue);

    log_message(LOG_INFO, "Gerenciador de processos inicializado com sucesso.");

    return EXIT_SUCCESS;
}

void terminate_process(ProcessManager *manager, int pid)
{
    for (int i = 0; i < manager->max_processes; i++)
    {
        if (manager->running_processes && manager->running_processes->pid == pid)
        {
            free(manager->running_processes);
            manager->running_processes = NULL;
            return;
        }
    }

    // Caso esteja na fila
    queue_clear(&manager->ready_queue);
}

void schedule_process(ProcessManager *manager, Process *process)
{
    queue_enqueue(&manager->ready_queue, process);
}

void run_scheduled_processes(ProcessManager *manager, MemoryManagementUnit *mmu)
{
    Queue *queue = &manager->ready_queue;

    int count = 0;
    int max_scheduling = 5;
    while (count++ < max_scheduling)
    {
        Process *current = queue_dequeue(queue);

        mmu_load_process(mmu, current);

        log_message(LOG_INFO, "Executando processo %d...", current->pid);
        manager->running_processes = current;

        int access_idx = 0;
        for (int i = 8000; i < 8050; i++)
        {
            mmu_access_memory(manager->mmu, current, i);
            current->access_sequence[access_idx++] = i;
            ++current->addresses_count;
        }

        current->quantum_time += manager->quantum;
        if (current->quantum_time < current->size)
        {
            queue_enqueue(queue, current);
        }
        else
        {
            log_message(LOG_INFO, "Processo %d concluído.\n", current->pid);
            free(current);
        }
    }
}

void process_manager_cleanup(ProcessManager *manager)
{
    free(manager->running_processes);

    queue_clear(&manager->ready_queue);
    free(manager->running_processes);
}

Process *get_allocated_process(ProcessManager *manager, Frame *frame)
{
    if (frame == NULL)
    {
        return NULL;
    }
    int allocated_process_pid = frame->allocated_process_pid;

    int search_pid;
    for (search_pid = 0; search_pid < manager->max_processes; search_pid++)
    {
        if (processes[search_pid].pid == allocated_process_pid)
        {
            log_message(LOG_INFO, "Processo %d encontrado!", search_pid);
            break;
        }
    }

    return &processes[search_pid];
}