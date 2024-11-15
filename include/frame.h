#ifndef FRAME_H
#define FRAME_H

#include <stdbool.h>

#define FRAME_SIZE (4096)

typedef struct Frame
{
    int frame_number;
    int size;
    char *data;
    bool is_occupied;
    int allocated_process_pid;
} Frame;

/**
 * @brief Inicializa um frame com número, tamanho e marca como não ocupado.
 * @param frame Ponteiro para a estrutura Frame.
 * @param frame_number Número único do frame na memória.
 * @param frame_size Tamanho do frame em bytes.
 * @return Retorna 0 se a criação for bem-sucedida.
 */
int frame_create(Frame *frame, int frame_number, int frame_size);

/**
 * @brief Retorna os dados armazenados no frame.
 * @param frame Ponteiro para a estrutura Frame.
 * @return Ponteiro para os dados armazenados.
 */
char *get_data(Frame *frame);

/**
 * @brief Define os dados no frame especificado.
 * @param frame Ponteiro para a estrutura Frame.
 * @param data Ponteiro para os dados a serem definidos.
 */
void set_data(Frame *frame, char *data);

/**
 * @brief Limpa o conteúdo do frame e define como não ocupado.
 * @param frame Ponteiro para a estrutura Frame.
 */
void clear_frame(Frame *frame);

#endif // FRAME_H
