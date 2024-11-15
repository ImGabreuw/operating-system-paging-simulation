#include "process_manager.h"

#include <stdio.h>
#include <stdlib.h>

#include "log.h"

int process_manager_init(ProcessManager *manager, int max_processes, int quantum)
{
    manager->max_processes = max_processes;
    manager->running_processes = (Process **)calloc(max_processes, sizeof(Process *));

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
        if (manager->running_processes[i] && manager->running_processes[i]->pid == pid)
        {
            free(manager->running_processes[i]);
            manager->running_processes[i] = NULL;
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

void run_scheduled_processes(ProcessManager *manager)
{
    Queue *queue = &manager->ready_queue;

    while (!queue_is_empty(queue))
    {
        Process *current = queue_dequeue(queue);
        log_message(LOG_INFO, "Executando processo %d...", current->pid);

        int access_idx = 0;
        for (int i = 8000; i < 8200; i++)
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
    for (int i = 0; i < manager->max_processes; i++)
    {
        if (manager->running_processes[i])
        {
            free(manager->running_processes[i]);
        }
    }

    queue_clear(&manager->ready_queue);
    free(manager->running_processes);
}
