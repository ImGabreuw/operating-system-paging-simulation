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
    int offset = virtual_address % process->logical_memory->page_size;

    if (page_number < 0 || page_number >= process->logical_memory->page_size)
    {
        log_message(LOG_WARNING, "Endereço lógico inválido: %d.", virtual_address);
        return -1;
    }

    int frame_number = get_frame_number(process->page_table, page_number);

    if (frame_number == -1)
    {
        mmu_handle_page_fault(mmu, process, page_number);

        frame_number = get_frame_number(process->page_table, page_number);
        if (frame_number == -1)
        {
            log_message(LOG_ERROR, "Falha ao traduzir endereço após tratar page fault.");
            return -1;
        }
    }

    return (frame_number * mmu->physical_memory.frame_size) + offset;
}

void mmu_handle_page_fault(MemoryManagementUnit *mmu, Process *process, int page_number)
{
    log_message(LOG_WARNING, "Page fault ocorrido no processo %d, página %d.", process->pid, page_number);

    Frame *frame = allocate_frame(&mmu->physical_memory);

    if (frame == NULL)
    {
        log_message(LOG_WARNING, "Memória física cheia. Executando substituição de página.");
        Process *allocated_process = get_allocated_process(mmu->process_manager, mmu->physical_memory.frames[replacement_index]);

        if (allocated_process == NULL)
        {
            log_message(LOG_ERROR, "Nenhum processo encontrado para substituição. Operação falhou.");
            return;
        }

        physical_memory_replace_frame(&mmu->physical_memory, mmu->disk, allocated_process);
        frame = allocate_frame(&mmu->physical_memory);
    }

    if (frame == NULL)
    {
        log_message(LOG_ERROR, "Falha ao alocar quadro após substituição.");
        return;
    }

    Page *page = disk_read_page(mmu->disk, page_number);
    if (page == NULL)
    {
        log_message(LOG_ERROR, "Erro ao ler página %d do disco.", page_number);
        return;
    }

    physical_memory_load_frame(&mmu->physical_memory, frame->frame_number, process, page);

    add_mapping(process->page_table, page_number, frame->frame_number);

    log_message(LOG_INFO, "Página %d do processo %d carregada no quadro %d.", page_number, process->pid, frame->frame_number);
}

void mmu_load_process(MemoryManagementUnit *mmu, Process *process)
{
    log_message(LOG_INFO, "Carregando processo %d na memória...", process->pid);

    for (int i = 0; i < process->logical_memory->size; i++)
    {
        Frame *frame = allocate_frame(&mmu->physical_memory);
        if (frame == NULL)
        {
            log_message(LOG_WARNING, "Memória cheia durante carregamento. Executando substituição.");
            Process *allocated_process = get_allocated_process(mmu->process_manager, mmu->physical_memory.frames[replacement_index]);
            physical_memory_replace_frame(&mmu->physical_memory, mmu->disk, allocated_process);
            frame = allocate_frame(&mmu->physical_memory);
        }

        if (frame == NULL)
        {
            log_message(LOG_ERROR, "Falha ao alocar quadro para página %d do processo %d.", i, process->pid);
            return;
        }

        Page *page = get_page(process->logical_memory, i);
        if (page == NULL)
        {
            log_message(LOG_ERROR, "Página %d não encontrada no processo %d.", i, process->pid);
            return;
        }

        physical_memory_load_frame(&mmu->physical_memory, frame->frame_number, process, page);
        add_mapping(process->page_table, page->page_number, frame->frame_number);

        log_message(LOG_INFO, "Página %d carregada no quadro %d.", page->page_number, frame->frame_number);
    }
}

void mmu_access_memory(MemoryManagementUnit *mmu, Process *process, int virtual_address)
{
    log_message(LOG_INFO, "Acessando endereço lógico %d no processo %d.", virtual_address, process->pid);

    int physical_address = mmu_translate_address(mmu, process, virtual_address);

    if (physical_address == -1)
    {
        log_message(LOG_ERROR, "Erro de acesso à memória após falha de página.");
        return;
    }

    log_message(LOG_INFO, "Endereço lógico %d traduzido para endereço físico %d.", virtual_address, physical_address);
}
