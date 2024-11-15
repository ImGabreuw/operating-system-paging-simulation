#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "memory_management_unit.h"
#include "logical_memory.h"
#include "physical_memory.h"
#include "process.h"
#include "process_manager.h"
#include "config_manager.h"
#include "disk.h"
#include "log.h"

int main(int argc, char const *argv[])
{
    log_init("simulador.log");

    // Inicializa o gerenciador de configurações e carrega as propriedades do simulador
    ConfigurationManager config_manager;
    load_properties(&config_manager, "simulador.properties");

    srand(time(NULL));

    // Cria o gerenciador de processos
    ProcessManager process_manager;
    int max_processes = 5;
    int quantum = QUANTUM; // em ms
    process_manager_init(&process_manager, max_processes, quantum);

    Disk disk;
    int disk_size = 100;
    int disk_access_delay = 5; // em ms
    disk_init(&disk, disk_size, disk_access_delay);

    MemoryManagementUnit mmu;
    int physical_memory_size = FRAME_SIZE * 10;
    int logical_memory_size = PAGE_SIZE * 10;
    mmu_init(&mmu, logical_memory_size, physical_memory_size, &disk, &process_manager);
    process_manager.mmu = &mmu;

    int num_processes = 5;
    Process *processes = (Process *)malloc(num_processes * sizeof(Process));
    for (int pid = 0; pid < num_processes; pid++)
    {
        process_create(&processes[pid], &mmu.logical_memory, pid, 1000 * pid, PROCESS_SIZE);
        mmu_load_process(&mmu, &processes[pid]);
    }

    // // Carrega os processos na memória
    // for (int i = 0; i < num_processes; i++)
    // {
    //     schedule_process(&process_manager, &processes[i]);
    // }

    // // Executa o escalonamento dos processos
    // while (true)
    // {
    //     bool has_running_process = false;

    //     for (int i = 0; i < process_manager.max_processes; i++)
    //     {
    //         if (process_manager.running_processes[i] != NULL)
    //         {
    //             has_running_process = true;
    //         }
    //     }

    //     if (!has_running_process)
    //     {
    //         break;
    //     }

    //     run_scheduled_processes(&process_manager);
    // }

    // // Libera os recursos alocados
    // for (int i = 0; i < num_processes; i++)
    // {
    //     process_free_table_page(&processes[i]);
    // }

    // logical_memory_free_pages(&mmu.logical_memory);
    // physical_memory_free_frames(&mmu.physical_memory);
    // free_properties(&config_manager);
    log_cleanup();

    return EXIT_SUCCESS;
}