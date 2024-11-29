#include "physical_memory.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "logical_memory.h"
#include "process_manager.h"
#include "disk.h"
#include "log.h"

// Índice global para o algoritmo FIFO
int replacement_index = 0;

int physical_memory_create(PhysicalMemory *physical_memory, int size)
{
    if (size <= 0 || size % FRAME_SIZE != 0)
    {
        log_message(LOG_ERROR, "Invalid memory size. Must be a positive multiple of FRAME_SIZE.");
        return EXIT_FAILURE;
    }

    physical_memory->size = size;
    physical_memory->frame_size = FRAME_SIZE;
    physical_memory->free_frame_count = size / FRAME_SIZE;

    // Alocação do vetor de frames
    physical_memory->frames = (Frame **)malloc((physical_memory->free_frame_count) * sizeof(Frame *));
    if (!physical_memory->frames)
    {
        log_message(LOG_ERROR, "Failed to allocate memory for frames.");
        return EXIT_FAILURE;
    }

    // Inicialização dos frames
    for (int i = 0; i < physical_memory->free_frame_count; i++)
    {
        physical_memory->frames[i] = (Frame *)malloc(sizeof(Frame));
        if (!physical_memory->frames[i])
        {
            log_message(LOG_ERROR, "Failed to allocate memory for frame %d.", i);
            return EXIT_FAILURE;
        }
        frame_create(physical_memory->frames[i], i, FRAME_SIZE);
    }

    log_message(LOG_INFO, "Memória física criada com sucesso. Tamanho: %d bytes.", size);
    return EXIT_SUCCESS;
}

Frame *allocate_frame(PhysicalMemory *physical_memory)
{
    if (physical_memory->free_frame_count <= 0)
    {
        return NULL;
    }

    for (int i = 0; i < (physical_memory->size / FRAME_SIZE); i++)
    {
        if (!physical_memory->frames[i]->is_occupied)
        {
            physical_memory->frames[i]->is_occupied = true;
            physical_memory->free_frame_count--;
            return physical_memory->frames[i];
        }
    }

    return NULL;
}

int release_frame(PhysicalMemory *physical_memory, int frame_number)
{
    if (frame_number < 0 || frame_number >= (physical_memory->size / FRAME_SIZE))
    {
        log_message(LOG_ERROR, "Frame number %d is out of bounds.", frame_number);
        return EXIT_FAILURE;
    }

    Frame *frame = physical_memory->frames[frame_number];
    if (!frame->is_occupied)
    {
        log_message(LOG_WARNING, "Frame %d is already free.", frame_number);
        return EXIT_SUCCESS;
    }

    frame->is_occupied = false;
    physical_memory->free_frame_count++;
    clear_frame(frame);
    return EXIT_SUCCESS;
}

void physical_memory_replace_frame(PhysicalMemory *physical_memory, Disk *disk, Process *allocated_process)
{
    Frame *frame_to_replace = physical_memory->frames[replacement_index];
    replacement_index = (replacement_index + 1) % (physical_memory->size / FRAME_SIZE);

    int page_number = -1;
    for (int i = 0; i < allocated_process->page_table->number_of_pages; i++)
    {
        PageTableEntry *entry = allocated_process->page_table->entries[i];
        if (entry->frame_number == frame_to_replace->frame_number)
        {
            page_number = entry->page_number;
            break;
        }
    }

    if (page_number == -1)
    {
        log_message(LOG_ERROR, "Page not found for frame %d in process %d.", frame_to_replace->frame_number, allocated_process->pid);
        return;
    }

    // Salva a página no disco antes de substituir
    Page *page = get_page(allocated_process->logical_memory, page_number);
    if (page != NULL && page->is_loaded)
    {
        disk_write_page(disk, page);
        page->is_loaded = false;
        log_message(LOG_INFO, "Página %d do processo %d salva no disco antes de substituir.", page_number, allocated_process->pid);
    }

    // Libera o frame
    release_frame(physical_memory, frame_to_replace->frame_number);
    remove_mapping(allocated_process->page_table, page_number);

    log_message(LOG_INFO, "Quadro %d liberado para substituição.", frame_to_replace->frame_number);
}

void physical_memory_load_frame(PhysicalMemory *physical_memory, int frame_number, Process *process, Page *page)
{
    if (frame_number < 0 || frame_number >= (physical_memory->size / FRAME_SIZE))
    {
        log_message(LOG_ERROR, "Número do quadro %d é inválido.", frame_number);
        return;
    }

    Frame *frame = physical_memory->frames[frame_number];
    if (frame->is_occupied)
    {
        log_physical_memory_state(physical_memory);
        log_message(LOG_WARNING, "Substituindo conteúdo do quadro %d.", frame_number);
    }

    // Carrega a nova página no frame
    frame->is_occupied = true;
    frame->allocated_process_pid = process->pid;
    page->is_loaded = true;

    // Atualiza a tabela de páginas do processo
    add_mapping(process->page_table, page->page_number, frame_number);

    sleep(physical_memory->access_delay);

    log_message(LOG_INFO, "Página %d do processo %d carregada no quadro %d.", page->page_number, process->pid, frame_number);
}

void physical_memory_free_frames(PhysicalMemory *physical_memory)
{
    for (int i = 0; i < (physical_memory->size / FRAME_SIZE); i++)
    {
        free(physical_memory->frames[i]);
    }
    free(physical_memory->frames);

    log_message(LOG_INFO, "Memória física liberada.");
}

void log_physical_memory_state(PhysicalMemory *physical_memory)
{
    log_message(LOG_INFO, "Estado da Memória Física:");

    log_plain_message(" ----------- \n");
    for (int i = 0; i < (physical_memory->size / FRAME_SIZE); i++)
    {
        Frame *frame = physical_memory->frames[i];
        if (frame != NULL && frame->is_occupied)
        {
            bool found = false;

            for (int j = 0; j < num_processes; j++)
            {
                for (int k = 0; k < processes[j].page_table->number_of_pages; k++)
                {
                    if (processes[j].page_table->entries[k] == NULL)
                    {
                        continue;
                    }

                    if (processes[j].page_table->entries[k]->frame_number == frame->frame_number)
                    {
                        log_plain_message("| P%03d - %02d | # Frame %d -> Página %d do Processo %d\n",
                                          processes[j].page_table->entries[k]->page_number, frame->allocated_process_pid,
                                          frame->frame_number, processes[j].page_table->entries[k]->page_number, frame->allocated_process_pid);
                        found = true;
                        break;
                    }
                }

                if (found)
                    break;
            }
        }
        else
        {
            log_plain_message("| --------- |\n");
        }
    }
    log_plain_message(" ----------- \n");
}
