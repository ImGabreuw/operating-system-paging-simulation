#include "memory_management_unit.h"

#include <stdio.h>
#include <stdlib.h>

#include "log.h"

void mmu_init(MemoryManagementUnit *mmu, int logical_size, int physical_size, Disk *disk, ProcessManager *process_manager)
{
    logical_memory_create(&mmu->logical_memory, logical_size, PAGE_SIZE, PROCESS_SIZE);
    physical_memory_create(&mmu->physical_memory, physical_size);
    mmu->disk = disk;
    mmu->process_manager = process_manager;

    log_message(LOG_INFO, "MMU inicializado com sucesso.");
}

int mmu_translate_address(MemoryManagementUnit *mmu, Process *process, int virtual_address)
{
    int page_number = virtual_address / process->logical_memory->page_size;
    int offset = virtual_address % mmu->logical_memory.page_size;

    // Verifica se o endereço é válido
    if (page_number >= process->logical_memory->size)
    {
        log_message(LOG_WARNING, "Endereço lógico inválido.");
        return EXIT_FAILURE;
    }

    // Procura o quadro na tabela de páginas
    int frame_number = get_frame_number(process->page_table, page_number);

    if (frame_number == -1)
    {
        mmu_handle_page_fault(mmu, process, page_number);
        frame_number = get_frame_number(process->page_table, page_number);

        if (frame_number == -1)
        {
            return EXIT_FAILURE;
        }
    }

    return (frame_number * mmu->physical_memory.frame_size) + offset;
}

void mmu_handle_page_fault(MemoryManagementUnit *mmu, Process *process, int page_number)
{
    log_message(LOG_WARNING, "Page fault ocorrido na página %d.", page_number);

    // Aloca um quadro na memória física
    Frame *frame = allocate_frame(&mmu->physical_memory);

    if (frame == NULL)
    {
        log_message(LOG_WARNING, "Memória física cheia. Substituição de página necessária.");
        physical_memory_replace_frame(&mmu->physical_memory, mmu->disk, process);
        frame = allocate_frame(&mmu->physical_memory);
    }

    // Carrega a página do disco
    Page *page = disk_read_page(mmu->disk, page_number);
    physical_memory_load_frame(&mmu->physical_memory, frame->frame_number, process, page);

    // Atualiza a tabela de páginas
    add_mapping(process->page_table, page_number, frame->frame_number);

    log_message(LOG_INFO, "Página %d carregada no quadro %d.", page_number, frame->frame_number);
}

void mmu_load_process(MemoryManagementUnit *mmu, Process *process)
{
    log_message(LOG_INFO, "Carregando processo %d...", process->pid);

    for (int i = 0; i < NUMBER_OF_PAGES; i++)
    {
        Frame *frame = allocate_frame(&mmu->physical_memory);
        if (frame == NULL)
        {
            log_message(LOG_WARNING, "Memória física cheia. Substituição de página necessária.");
            physical_memory_replace_frame(&mmu->physical_memory, mmu->disk, process);
            frame = allocate_frame(&mmu->physical_memory);
        }

        Page *page = get_page(process->logical_memory, i);
        physical_memory_load_frame(&mmu->physical_memory, frame->frame_number, process, page);
        add_mapping(process->page_table, page->page_number, frame->frame_number);

        log_message(LOG_INFO, "Página %d do processo %d carregada no quadro %d.", page->page_number, process->pid, frame->frame_number);
    }
}

void mmu_access_memory(MemoryManagementUnit *mmu, Process *process, int virtual_address)
{
    int physical_address = mmu_translate_address(mmu, process, virtual_address);

    if (physical_address == -1)
    {
        log_message(LOG_WARNING, "Falha de página ao acessar o endereço %d.", virtual_address);
        int page_number = virtual_address / mmu->logical_memory.page_size;
        mmu_handle_page_fault(mmu, process, page_number);
        physical_address = mmu_translate_address(mmu, process, virtual_address);
    }

    log_message(LOG_INFO, "Acesso à memória: Endereço lógico %d -> Endereço físico %d.", virtual_address, physical_address);
}
