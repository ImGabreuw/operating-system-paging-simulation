#include <stdio.h>
#include <stdlib.h>
#include "logicalMemory.h"
#include "physicalMemory.h"
#include "process_structures.h"


#define PROCESS_SIZE (8192) // Tamanho total do processo em bytes
#define NUMBER_OF_PAGES (PROCESS_SIZE / PAGE_SIZE)
#define PHYSICAL_MEMORY_SIZE (16384) // Tamanho total da memória física
#define NUMBER_OF_FRAMES 4

//#include "log.h"

/*
Integrantes:

Enzo Ribeiro                    - 10418262
Gabriel Ken Kazama Geronazzo    - 10418247
Lucas Pires de Camargo Sarai    - 10418013
Lucas Zanini da Silva           - 10417361

Código fonte: https://github.com/ImGabreuw/operating-system-paging-simulation
*/


int main(int argc, char const *argv[]) {
    // Inicializa a memória física
    PhysicalMemory physicalMemory;
    physicalMemory.size = NUMBER_OF_FRAMES;
    physicalMemory.frames = malloc(NUMBER_OF_FRAMES * sizeof(Frame*));
    physicalMemory.freeFrameCount = NUMBER_OF_FRAMES;

    for (int i = 0; i < NUMBER_OF_FRAMES; i++) {
        physicalMemory.frames[i] = (Frame*) malloc(sizeof(Frame));
        physicalMemory.frames[i]->frameNumber = i;
        physicalMemory.frames[i]->isOccupied = false;
    }

    // Criação do processo
    Process process;
    process.pid = 1;
    process.size = PROCESS_SIZE;
    process.pageTable = (PageTable*) malloc(sizeof(PageTable));
    process.pageTable->entries = malloc(NUMBER_OF_PAGES * sizeof(PageTableEntry*));
    process.pageTable->numberOfPages = NUMBER_OF_PAGES;

    for (int i = 0; i < NUMBER_OF_PAGES; i++) {
        process.pageTable->entries[i] = NULL; // Inicializa as entradas como vazias
    }

    // Divisão do processo em páginas
    LogicalMemory logicalMemory;
    logicalMemory.size = NUMBER_OF_PAGES;
    logicalMemory.pages = malloc(NUMBER_OF_PAGES * sizeof(Page*));

    for (int i = 0; i < NUMBER_OF_PAGES; i++) {
        logicalMemory.pages[i] = (Page*) malloc(sizeof(Page));
        logicalMemory.pages[i]->pageNumber = i;
        logicalMemory.pages[i]->frameNumber = -1; // Nenhum frame alocado inicialmente
        logicalMemory.pages[i]->isDirty = false;
    }

    process.logicalMemory = &logicalMemory;

    // Mapeamento das páginas para frames
    for (int i = 0; i < NUMBER_OF_PAGES; i++) {
        Frame* allocatedFrame = allocateFrame(&physicalMemory);
        
        if (allocatedFrame != NULL) {
            logicalMemory.pages[i]->frameNumber = allocatedFrame->frameNumber;

            // Adiciona o mapeamento na tabela de páginas
            addMapping(process.pageTable, logicalMemory.pages[i]->pageNumber, allocatedFrame->frameNumber);
            printf("Página %d mapeada para Frame %d\n", i, allocatedFrame->frameNumber);
        } else {
            printf("Erro: Não há frames livres suficientes para alocar a página %d\n", i);
            break;
        }
    }

    // Liberando a memória alocada
    for (int i = 0; i < NUMBER_OF_PAGES; i++) {
        free(logicalMemory.pages[i]);
    }
    free(logicalMemory.pages);

    for (int i = 0; i < NUMBER_OF_FRAMES; i++) {
        free(physicalMemory.frames[i]);
    }
    free(physicalMemory.frames);

    free(process.pageTable->entries);
    free(process.pageTable);

    printf("Processo %d criado com sucesso e mapeado para a memória física.\n", process.pid);

    printf("\nTraduzindo Pagina %d...\n", 1);
    int frameNumber = getFrameNumber(process.pageTable,1);
    if(frameNumber > -1){
        printf("Frame correspondente: %d\n",frameNumber);
    } else printf("Nao houve correspondencia!\n");
    
    printf("Teste de Page Fault: Pagina %d existe? ",50);
    if(getPage(&logicalMemory,50) == NULL) printf("PAGE FAULT!\n");
    else printf("SIM!\n");

    return EXIT_SUCCESS;
}