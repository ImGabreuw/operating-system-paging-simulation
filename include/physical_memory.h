#ifndef PHYSICAL_MEMORY_H
#define PHYSICAL_MEMORY_H

#include "frame.h"

typedef struct PhysicalMemory
{
    int size;
    int frame_size;
    Frame **frames;
    int free_frame_count;
    int access_delay;
} PhysicalMemory;

/**
 * @brief Inicializa a memória física com tamanho e tamanho de frames.
 * @param physical_memory Ponteiro para a estrutura PhysicalMemory.
 * @param size Tamanho total da memória física.
 * @return Retorna 0 se a criação for bem-sucedida.
 */
int physical_memory_create(PhysicalMemory *physical_memory, int size);

/**
 * @brief Aloca e retorna um frame livre na memória física.
 * @param physical_memory Ponteiro para a estrutura PhysicalMemory.
 * @return Ponteiro para o frame alocado ou NULL se não houver frames livres.
 */
Frame *allocate_frame(PhysicalMemory *physical_memory);

/**
 * @brief Retorna o número de frames livres na memória física.
 * @param physical_memory Ponteiro para a estrutura PhysicalMemory.
 * @return Número de frames livres.
 */
int get_free_frame_count(PhysicalMemory *physical_memory);

/**
 * @brief Libera um frame especificado pelo número, tornando-o reutilizável.
 * @param physical_memory Ponteiro para a estrutura PhysicalMemory.
 * @param frame_number Número do frame a ser liberado.
 * @return true se a liberação for bem-sucedida, caso contrário false.
 */
int release_frame(PhysicalMemory *physical_memory, int frame_number);

/**
 * @brief Escreve dados no frame especificado.
 * @param physical_memory Ponteiro para a estrutura PhysicalMemory.
 * @param frame_number Número do frame onde os dados serão escritos.
 * @param data Dados a serem escritos no frame.
 * @return true se a operação for bem-sucedida, caso contrário false.
 */
int write_to_frame(PhysicalMemory *physical_memory, int frame_number, char *data);

/**
 * @brief Lê e retorna os dados do frame especificado.
 * @param physical_memory Ponteiro para a estrutura PhysicalMemory.
 * @param frame_number Número do frame de onde os dados serão lidos.
 * @return Ponteiro para os dados do frame ou NULL se não encontrado.
 */
char *read_from_frame(PhysicalMemory *physical_memory, int frame_number);

#endif // PHYSICAL_MEMORY_H
