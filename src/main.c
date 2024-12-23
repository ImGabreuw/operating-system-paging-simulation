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

/*
Integrantes:
-Enzo Ribeiro-10418262
-Gabriel Ken Kazama Geronazzo-10418247
-Lucas Pires de Camargo Sarai-10418013
-Lucas Zanini da Silva-104173
CC - 04D - SO
*/

int main(int argc, char const *argv[])
{
    log_init("simulador.log");

    // Inicializa o gerenciador de configurações e carrega as propriedades do simulador
    ConfigurationManager config_manager;
    load_properties(&config_manager, "simulador.properties");

    srand(time(NULL));

    // Cria o gerenciador de processos
    ProcessManager process_manager;
    int max_processes = atoi(get_property(&config_manager, "processes.max"));
    int quantum = atoi(get_property(&config_manager, "processes.quantum")); // em ms
    process_manager_init(&process_manager, max_processes, quantum);

    Disk disk;
    int disk_size = atoi(get_property(&config_manager, "disk.size"));
    int disk_access_delay = atoi(get_property(&config_manager, "disk.delay")); // em ms
    disk_init(&disk, disk_size, disk_access_delay);

    MemoryManagementUnit mmu;
    int physical_memory_size = FRAME_SIZE * atoi(get_property(&config_manager, "memory.physical"));;
    int logical_memory_size = PAGE_SIZE * atoi(get_property(&config_manager, "memory.logical"));;
    mmu_init(&mmu, logical_memory_size, physical_memory_size, &disk, &process_manager);
    process_manager.mmu = &mmu;

    num_processes = 5;
    processes = (Process *)malloc(num_processes * sizeof(Process));
    for (int pid = 0; pid < num_processes; pid++)
    {
        process_create(&processes[pid], &mmu.logical_memory, &disk, pid, 1000 * pid, PROCESS_SIZE);
    }

    // Carrega os processos na memória
    for (int i = 0; i < num_processes; i++)
    {
        schedule_process(&process_manager, &processes[i]);
    }

    run_scheduled_processes(&process_manager, &mmu);

    // Libera os recursos alocados
    for (int i = 0; i < num_processes; i++)
    {
        process_free_table_page(&processes[i]);
    }

    logical_memory_free_pages(&mmu.logical_memory);
    physical_memory_free_frames(&mmu.physical_memory);
    free_properties(&config_manager);
    log_cleanup();

    return EXIT_SUCCESS;
}