#include <stdio.h>
#include <stdlib.h>

#include "process_structures.h"
#include "logical_memory.h"
#include "physical_memory.h"

#include "log.h"

#define PROCESS_SIZE (8192) // Tamanho total do processo em bytes
#define NUMBER_OF_PAGES (PROCESS_SIZE / PAGE_SIZE)
#define PHYSICAL_MEMORY_SIZE (16384) // Tamanho total da memória física
#define NUMBER_OF_FRAMES 4

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

    // Inicializa a memória física
    PhysicalMemory physical_memory;
    physical_memory_create(&physical_memory, NUMBER_OF_FRAMES * FRAME_SIZE);

    for (int i = 0; i < NUMBER_OF_FRAMES; i++)
    {
        physical_memory.frames[i] = (Frame *)malloc(NUMBER_OF_FRAMES * sizeof(Frame));
        frame_create(physical_memory.frames[i], i, FRAME_SIZE);
    }

    // Criação do processo
    Process process;
    create_p(&process, 1, 1000, PROCESS_SIZE);
    process.pageTable->numberOfPages = NUMBER_OF_PAGES;
    process.pageTable->entries = (PageTableEntry **)malloc(NUMBER_OF_PAGES * sizeof(PageTableEntry *));

    for (int i = 0; i < NUMBER_OF_PAGES; i++)
    {
        process.pageTable->entries[i] = (PageTableEntry *)malloc(NUMBER_OF_PAGES * sizeof(PageTableEntry));
        create_pte(process.pageTable->entries[i], i, -1);
    }

    // Divisão do processo em páginas
    LogicalMemory logical_memory;
    logical_memory_create(&logical_memory, NUMBER_OF_PAGES, PAGE_SIZE, PROCESS_SIZE);

    for (int i = 0; i < NUMBER_OF_PAGES; i++)
    {
        logical_memory.pages[i] = (Page *)malloc(sizeof(Page));
        page_create(logical_memory.pages[i], i, 1);
    }

    process.logicalMemory = &logical_memory;

    // Mapeamento das páginas para frames
    for (int i = 0; i < NUMBER_OF_PAGES; i++)
    {
        Frame *allocatedFrame = allocate_frame(&physical_memory);

        if (allocatedFrame != NULL)
        {
            logical_memory.pages[i]->is_loaded = true;

            // Adiciona o mapeamento na tabela de páginas
            addMapping(process.pageTable, logical_memory.pages[i]->page_number, allocatedFrame->frame_number);
            log_message(LOG_INFO, "Página %d mapeada para Frame %d", i, allocatedFrame->frame_number);
        }
        else
        {
            log_message(LOG_ERROR, "Não há frames livres suficientes para alocar a página %d", i);
            break;
        }
    }

    logical_memory_free_pages(&logical_memory);
    physical_memory_free_frames(&physical_memory);

    free(process.pageTable->entries);
    free(process.pageTable);

    log_message(LOG_INFO, "Processo %d criado com sucesso e mapeado para a memória física.", process.pid);

    // Semana 2
    // int access = 200;
    // int physicalAccess = translateAddress(&process,&logical_memory,access);
    // printf("Acesso logico %d corresponde a %d", access,physicalAccess);

    log_cleanup();

    return EXIT_SUCCESS;
}
