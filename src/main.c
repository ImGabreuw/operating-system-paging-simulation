#include <stdio.h>
#include <stdlib.h>

#include "physicalMemory.h"
#include "process_structures.h"
#include "logical_memory.h"
#include "page.h"

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

    // Inicializa a memória física
    PhysicalMemory physicalMemory;
    create_pm(&physicalMemory, NUMBER_OF_FRAMES * 4096);

    for (int i = 0; i < NUMBER_OF_FRAMES; i++)
    {
        physicalMemory.frames[i] = (Frame *)malloc(NUMBER_OF_FRAMES * sizeof(Frame));
        create_fr(physicalMemory.frames[i], i, FRAME_SIZE);
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
    LogicalMemory logicalMemory;
    logical_memory_create(&logicalMemory, NUMBER_OF_PAGES, PAGE_SIZE, PROCESS_SIZE);

    for (int i = 0; i < NUMBER_OF_PAGES; i++)
    {
        logicalMemory.pages[i] = (Page *)malloc(sizeof(Page));
        page_create(logicalMemory.pages[i], i, 1);
    }

    process.logicalMemory = &logicalMemory;

    // Mapeamento das páginas para frames
    for (int i = 0; i < NUMBER_OF_PAGES; i++)
    {
        Frame *allocatedFrame = allocateFrame(&physicalMemory);

        if (allocatedFrame != NULL)
        {
            logicalMemory.pages[i]->is_loaded = true;

            // Adiciona o mapeamento na tabela de páginas
            addMapping(process.pageTable, logicalMemory.pages[i]->page_number, allocatedFrame->frameNumber);
            printf("Página %d mapeada para Frame %d\n", i, allocatedFrame->frameNumber);
        }
        else
        {
            printf("Erro: Não há frames livres suficientes para alocar a página %d\n", i);
            break;
        }
    }

    // Liberando a memória alocada
    for (int i = 0; i < NUMBER_OF_PAGES; i++)
    {
        free(logicalMemory.pages[i]);
    }
    free(logicalMemory.pages);

    for (int i = 0; i < NUMBER_OF_FRAMES; i++)
    {
        free(physicalMemory.frames[i]);
    }
    free(physicalMemory.frames);

    free(process.pageTable->entries);
    free(process.pageTable);

    printf("Processo %d criado com sucesso e mapeado para a memória física.\n", process.pid);

    // Semana 2
    // int access = 200;
    // int physicalAccess = translateAddress(&process,&logicalMemory,access);
    // printf("Acesso logico %d corresponde a %d", access,physicalAccess);

    return EXIT_SUCCESS;
}
