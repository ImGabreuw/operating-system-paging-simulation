#include "disk.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "log.h"

void disk_init(Disk *disk, int size, int access_delay)
{
    disk->size = size;
    disk->access_delay = access_delay;
    swap_area_init(&disk->swap_area, size);

    log_message(LOG_INFO, "Disco inicializado com sucesso.");
}

Page *disk_read_page(Disk *disk, int page_number)
{
    log_message(LOG_INFO, "Simulando atraso de leitura de %d ms...", disk->access_delay);
    sleep(disk->access_delay / 1000);

    return swap_area_read_block(&disk->swap_area, page_number);
}

void disk_write_page(Disk *disk, Page *page)
{
    log_message(LOG_INFO, "Simulando atraso de escrita de %d ms...", disk->access_delay);
    sleep(disk->access_delay / 1000);

    swap_area_write_block(&disk->swap_area, page);
}

int disk_allocate_swap_space(Disk *disk, int size)
{
    return swap_area_allocate_block(&disk->swap_area, size);
}

void disk_release_swap_space(Disk *disk, int swap_address)
{
    swap_area_release_block(&disk->swap_area, swap_address);
}
