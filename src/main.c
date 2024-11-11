#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "memory_management_unit.h"
#include "logical_memory.h"
#include "physical_memory.h"
#include "process.h"

#include "log.h"

#define PHYSICAL_MEMORY_SIZE (16384) // Tamanho total da memória física
#define NUMBER_OF_FRAMES (25)

/*
Integrantes:

Enzo Ribeiro                    - 10418262
Gabriel Ken Kazama Geronazzo    - 10418247
Lucas Pires de Camargo Sarai    - 10418013
Lucas Zanini da Silva           - 10417361

Código fonte: https://github.com/ImGabreuw/operating-system-paging-simulation
*/

int main(int argc, char const *argv[])
{
    log_init("simulador.log");
    srand(time(NULL));

    // Inicializa a memória física
    PhysicalMemory physical_memory;
    physical_memory_create(&physical_memory, NUMBER_OF_FRAMES * FRAME_SIZE);
    physical_memory.access_delay = 0.75;

    for (int i = 0; i < NUMBER_OF_FRAMES; i++)
    {
        frame_create(physical_memory.frames[i], i, FRAME_SIZE);
    }

    // Divisão do processo em páginas
    LogicalMemory logical_memory;
    logical_memory_create(&logical_memory, NUMBER_OF_PAGES, PAGE_SIZE, PROCESS_SIZE);

    for (int i = 0; i < NUMBER_OF_PAGES; i++)
    {
        logical_memory.pages[i] = (Page *)malloc(sizeof(Page));
        page_create(logical_memory.pages[i], i, 1);
    }

    // Criação dos processos
    Process* processes = (Process*) malloc(5 * sizeof(Process));
    int process_pid;
    for(process_pid = 1; process_pid <= 5; process_pid++){
        process_create(&processes[process_pid-1], &logical_memory, process_pid, 1000*process_pid, PROCESS_SIZE);
    }

    // Mapeamento das páginas para frame
    for (int i = 0; i < NUMBER_OF_PAGES; i++)
    {
        process_pid= rand()%5;
        Frame *allocatedFrame = allocate_frame(&physical_memory);

        if (allocatedFrame != NULL)
        {
            logical_memory.pages[i]->is_loaded = true;

            // Adiciona o mapeamento na tabela de páginas
            add_mapping(processes[process_pid].page_table, logical_memory.pages[process_pid]->page_number, allocatedFrame->frame_number);
            log_message(LOG_INFO, "Processo %d: Página %d mapeada para Frame %d",processes[process_pid].pid, i, allocatedFrame->frame_number);
            sleep(physical_memory.access_delay);
        }
        else
        {
            log_message(LOG_ERROR, "Não há frames livres suficientes para alocar a página %d", i);
            break;
        }
    }

    //Testando traducoes de endereco logico para fisico
    int access_idx = 0;
    for (int i = 8000; i < 8200; i++)
    {
        process_pid= rand()%5;
        int physicalAddress = translate_address(&processes[process_pid],i);
        if(physicalAddress != -1)log_message(LOG_INFO,"Processo %d: Logico = %d, Físico = %d",processes[process_pid].pid, i, physicalAddress);
        else log_message(LOG_ERROR,"Processo %d: PAGE FAULT! Pagina %d nao esta alocada na memoria fisica!",processes[process_pid].pid,i/FRAME_SIZE);
        sleep(physical_memory.access_delay);
        processes[process_pid].access_sequence[access_idx++] = i;
    }
        
    logical_memory_free_pages(&logical_memory);
    physical_memory_free_frames(&physical_memory);
    for (process_pid = 0; process_pid < 5; process_pid++){
        process_free_table_page(&processes[process_pid]);
        log_message(LOG_INFO, "Processo %d criado com sucesso e mapeado para a memória física.", processes[process_pid].pid);
    }
    //Exibindo a lista de acessos de cada processo
    for(process_pid = 0; process_pid < 5; process_pid++){
        log_message(LOG_INFO,"Lista de acessos do processo %d:\n", processes[process_pid].pid);
        for(access_idx = 0; access_idx<processes[process_pid].addresses_count; access_idx++){
            if(processes[process_pid].access_sequence[access_idx] !=0)log_message(LOG_INFO,"%d, ",processes[process_pid].access_sequence[access_idx]);
        }
        log_message(LOG_INFO,"\n");
    }
    log_cleanup();

    return EXIT_SUCCESS;
}
