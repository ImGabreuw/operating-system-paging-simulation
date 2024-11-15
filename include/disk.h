#ifndef DISK_H
#define DISK_H

#include "swap_area.h"

#include "page.h"

typedef struct
{
    int size;           // Tamanho total do disco (em blocos).
    SwapArea swap_area; // Instância da área de swap.
    int access_delay;   // Tempo de atraso simulado para leitura/escrita (em ms).
} Disk;

/**
 * Inicializa o disco.
 *
 * @param disk Ponteiro para a estrutura Disk.
 * @param size Tamanho total do disco (em blocos).
 * @param access_delay Tempo de atraso simulado (em ms).
 */
void disk_init(Disk *disk, int size, int access_delay);

/**
 * Lê uma página do disco.
 *
 * @param disk Ponteiro para a estrutura Disk.
 * @param page_number Número da página a ser lida.
 * @return Ponteiro para a estrutura Page correspondente.
 */
Page *disk_read_page(Disk *disk, int page_number);

/**
 * Escreve uma página no disco.
 *
 * @param disk Ponteiro para a estrutura Disk.
 * @param page Ponteiro para a estrutura Page a ser escrita.
 */
void disk_write_page(Disk *disk, Page *page);

/**
 * Aloca espaço no swap.
 *
 * @param disk Ponteiro para a estrutura Disk.
 * @param size Tamanho do espaço a ser alocado.
 * @return Número do bloco alocado ou -1 em caso de falha.
 */
int disk_allocate_swap_space(Disk *disk, int size);

/**
 * Libera espaço no swap.
 *
 * @param disk Ponteiro para a estrutura Disk.
 * @param swap_address Endereço do bloco a ser liberado.
 */
void disk_release_swap_space(Disk *disk, int swap_address);

#endif // DISK_H
